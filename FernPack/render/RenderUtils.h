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

} 