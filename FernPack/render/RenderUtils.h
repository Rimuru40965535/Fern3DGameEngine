/**
* @file FernPack/render/RenderUtils.h
* @brief 渲染算法工具集
* @details
* 存放渲染管线的各个算法实现，作为纯函数供渲染调度器调用。
*
* @author 半人马座beta星 （AIGC：DeepSeek）
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
            DrawWithDepthTest(target, depthBuff, x0, y0, (start.z < end.z ? start.z : end.z), color);
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
            DrawWithDepthTest(target, depthBuff, curx, cury, curdepth, color);

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
} 