/**
* @file FernPack/render/RenderTarget.h
* @brief 渲染目标
* @details
* 渲染目标的抽象，封装像素绘制、清屏、呈现。
* 持有窗口引用，提供绘制接口。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/10/1
*
*/
#pragma once

namespace Fern::Render {

    /**
    * @brief 渲染目标
    * @details
    * 渲染目标的抽象，封装像素绘制、清屏、呈现。
    * 持有窗口引用，提供绘制接口。
    */
    class RenderTarget {
    protected:
        Window* window;     ///< 窗口指针

    public:
        /**
        * @brief 构造函数
        * @param Window& w 窗口引用
        */
        RenderTarget(Window& w) : window(&w) {}

        /**
        * @brief 清除屏幕
        */
        void Clear() {
            window->Clear();
        }

        /**
        * @brief 交换缓冲区
        */
        void Present() {
            window->Present();
        }

        /**
        * @brief 绘制像素
        * @param int x 像素 x 坐标
        * @param int y 像素 y 坐标
        * @param Color 颜色
        */
        void DrawPixel(int x, int y, const Color& color) {
            if (x < 0 || x >= window->Width()) return;
            if (y < 0 || y >= window->Height()) return;
            putpixel(x, y, color.ToEasyXColor());
        }

        int Width() const { return window->Width(); }
        int Height() const { return window->Height(); }
    };

} 