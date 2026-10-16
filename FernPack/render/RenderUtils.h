/**
* @file FernPack/render/RenderUtils.h
* @brief 渲染算法工具集
* @details
* 存放渲染管线的各个算法实现，作为纯函数供渲染调度器调用。
*
* 文档中有详细的原理实现、设计取舍（除部分函数）
* @author 半人马座beta星 (AIGC:DeepSeek)
* @date 2026/10/1
*
*/
#pragma once

namespace Fern::Render {

    /**
    * @brief 深度测试绘制
    * @details
    * 如果当前深度小于深度缓冲中记录的深度，说明当前像素更近，
    * 通过测试，绘制像素并更新深度缓冲；否则跳过。
    *
    * @param RenderTarget* target 渲染目标
    * @param DepthBuff* depthBuff 深度缓冲
    * @param int x 像素 x 坐标
    * @param int y 像素 y 坐标
    * @param double depth 当前像素的深度
    * @param Color color 当前像素的颜色
    */
    inline void DrawWithDepthTest(
        RenderTarget* target,
        DepthBuff* depthBuff,
        int x, int y, double depth,
        const Color& color
    ) {
        if (!target || !depthBuff) return;

        if (depth < depthBuff->GetDepth(x, y)) {
            depthBuff->SetDepth(x, y, depth);
            target->DrawPixel(x, y, color);
        }
    }


    /**
    * @brief 绘制直线
    * @details
    * 使用 Bresenham 算法绘制直线，并为每个像素插值深度。
    * 通过深度测试决定像素是否被绘制。
    * 该算法由DeepSeek生成。未来在记录原理之后计划重写该函数并使用现有单元测试源文件测试。
    *
    * @param RenderTarget* target 渲染目标
    * @param DepthBuff* depthBuff 深度缓冲
    * @param Vector3D start 起点（屏幕坐标 + 深度）
    * @param Vector3D end 终点（屏幕坐标 + 深度）
    * @param Color color 直线颜色
    */
    inline void DrawLine(
        RenderTarget* target,
        DepthBuff* depthBuff,
        Vector3D start,
        Vector3D end,
        const Color& color
    ) {
        if (!target || !depthBuff) return;

        // 1. 转换为整数坐标
        int x0 = (int)(start.x);
        int y0 = (int)(start.y);
        int x1 = (int)(end.x);
        int y1 = (int)(end.y);

        float z0 = (float)start.z;
        float z1 = (float)end.z;

        // 2. 如果起点和终点相同，直接绘制一个点
        if (x0 == x1 && y0 == y1) {
            DrawWithDepthTest(target, depthBuff, x0, y0, (start.z < end.z ? start.z : end.z)-0.0005, color);
            return;
        }

        // 3. Bresenham 算法初始化
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        int curx = x0;
        int cury = y0;

        // 4. 计算深度增量（基于总步数）
        int steps = (dx > dy) ? dx : dy;
        float dz = (steps > 0) ? (z1 - z0) / steps : 0.0f;
        float curdepth = z0;

        // 5. Bresenham 主循环
        while (true) {
            // 绘制当前像素（带深度测试）
            DrawWithDepthTest(target, depthBuff, curx, cury, curdepth-0.001, color);

            // 到达终点，退出循环
            if (curx == x1 && cury == y1) break;

            int e2 = 2 * err;

            // 步进 x
            if (e2 > -dy) {
                err -= dy;
                curx += sx;
                curdepth += dz;
            }

            // 步进 y
            if (e2 < dx) {
                err += dx;
                cury += sy;
                curdepth += dz;
            }
        }
    }

    /**
    * @brief 屏幕顶点
    * @details 顶点封装为数据体，一可以隐式调用构造函数化整，二便于指针在扫描线算法中指向顶点整体，防止顶点错乱
    */
    struct ScreenVerts {
        int x;
        int y;
        double screendepth;
        //ScreenVerts() {}
        ScreenVerts(const Vector3D& A) :x(A.x), y(A.y),screendepth(A.z) {}
        bool operator<(const ScreenVerts& A) const {
            if (this->y < A.y)return true;
            if (this->y == A.y) {
                if (this->x < A.x)return true;
            }
            return false;
        }
        bool operator>(const ScreenVerts& A)const {
            if (this->y > A.y)return true;
            if (this->y == A.y) {
                if (this->x > A.x)return true;
            }
            return false;
        }
        bool operator==(const ScreenVerts& A) const {
            return (this->x == A.x && this->y == A.y);
        }
    };

    /**
    * @brief 快速在屏幕上填充纯色三角形
    * @details
    * 采用扫描线算法，快速在屏幕上绘制三角形投影
    *
    * @param RenderTarget* target 渲染目标
    * @param DepthBuff* depthbuff 深度缓冲
    * @param Vector3D A 屏幕点A
    * @param Vector3D B 屏幕点B
    * @param Vector3D C 屏幕点C
    * @param Color color 直线颜色
    */
    inline void FastFillTriangle(
        RenderTarget* target,
        DepthBuff* depthbuff,
        Vector3D& A,
        Vector3D& B,
        Vector3D& C,
        const Color& color
    ) {

        // 0、顶点化整

        ScreenVerts screenA(A), screenB(B), screenC(C);

        // 三点共线时平面会退化为直线
        //if (screenA.x == screenB.x && screenB.x == screenC.x)return;
        //if (screenA.y == screenB.y && screenB.y == screenC.y)return;
        // 任意两点共点，则平面退化为直线
        if (screenA == screenB)return;
        if (screenB == screenC)return;
        if (screenC == screenA)return;


        //1、排序顶点
        ScreenVerts* V1, *V2, *V3;
        ScreenVerts* pLeftStart;
        ScreenVerts* pRightStart;
        ScreenVerts* pLeftEnd;
        ScreenVerts* pRightEnd;

        //左上角点默认y值最靠上，在y相同的时候x最靠左
        V1 = &(screenA < screenB ? screenA : screenB);
        V1 = (*V1 < screenC ? V1 : &screenC);

        //右下角点默认y值最靠下，在y相同的时候x最靠右
        V3 = &(screenA > screenB ? screenA : screenB);
        V3 = (*V3 > screenC ? V3 : &screenC);

        // 最后一个角点要确保和其余两个不相同，至少可以确定y轴不会超过其他两个顶点的范围。
        V2 = &screenA;
        if (V2 == V1 || V2 == V3)V2 = &screenB;
        if (V2 == V1 || V2 == V3)V2 = &screenC;

        //如果此时有点和左上角点y值相同，说明顶边水平，保持右上角不变即可
        //如果此时没有点和左上角点的y值相同，说明顶边不水平
        pLeftStart = V1;
        if (V1->y == V2->y) {
            pRightStart = V2;
            pLeftEnd = V3;
            pRightEnd = V3;
        }
        else {
            pRightStart = V1;
            pLeftEnd = (V2->x < V3->x ? V2 : V3);
            pRightEnd = (V2->x < V3->x ? V3 : V2);
        }



        // 2、准备循环
        int LineLeft;
        int LineRight;
        double LineLeftDepth;
        double LineRightDepth;
        double CurrentDepth;

        

        // 3、绘制循环
        int y = V1->y;
        while(1) {

            LineLeft = Math::LiniarInterpolation(pLeftStart->x,pLeftEnd->x,Math::SolveLiniarInterpolation(pLeftStart->y,pLeftEnd->y,y));
            LineRight = Math::LiniarInterpolation(pRightStart->x, pRightEnd->x, Math::SolveLiniarInterpolation(pRightStart->y, pRightEnd->y, y));

            LineLeftDepth = Math::LiniarInterpolation(pLeftStart->screendepth, pLeftEnd->screendepth, Math::SolveLiniarInterpolation(pLeftStart->y, pLeftEnd->y, y));
            LineRightDepth = Math::LiniarInterpolation(pRightStart->screendepth, pRightEnd->screendepth, Math::SolveLiniarInterpolation(pRightStart->y, pRightEnd->y, y));

            if (LineLeft > LineRight) { int t = LineLeft;LineLeft = LineRight;LineRight = t; double m = LineLeftDepth;LineLeftDepth = LineRightDepth;LineRightDepth = m; }
            

            // 绘制一行
            for (int x = LineLeft;x <= LineRight;x++) {

                CurrentDepth = Math::LiniarInterpolation(LineLeftDepth, LineRightDepth, Math::SolveLiniarInterpolation(LineLeft, LineRight, x));
                DrawWithDepthTest(target, depthbuff, x, y, CurrentDepth, color);

            }

            // 检查y值是否触及顶点，如果触及就替换一边的起点终点。
            if (y == pLeftEnd->y) {//左边触及顶点
                if (y == pRightEnd->y)break;//两边同时触及终点，要么是同一点，要么底边水平，而且V3是必然触发的，不会造成死循环。
                pLeftStart = pLeftEnd;
                pLeftEnd = pRightEnd;
            }
            else if (y == pRightEnd->y) {//只有右边触及顶点
                pRightStart = pRightEnd;
                pRightEnd = pLeftEnd;
            }
            
            y++;

            
        }
        
    }
} 