/**
* @file 03--Rainbow_Window.cpp
* @brief 彩虹窗口
* @detail
* 
* @author 半人马座beta星
* @date 2026.7.16
*/

#include "Fern3DGameEngine.h"

int main() {



    // 1. 创建窗口 (宽800, 高600)
    Fern::Window window(800, 600, "Fern Engine - Rainbow Gradient",EX_SHOWCONSOLE);


    // 2. 绘制彩虹渐变
    //    使用双重循环遍历所有像素

    for (int y = 0; y < window.Height(); ++y) {
        for (int x = 0; x < window.Width(); ++x) {
            // 将像素坐标映射到 [0, 1] 范围
            float fx = static_cast<float>(x) / window.Width();
            float fy = static_cast<float>(y) / window.Height();

            // ---- 核心：生成 HSV 彩虹颜色 ----
            // 色相 (Hue) 从左到右从 0 到 360 度变化
            float hue = fx * 360.0f;
            // 饱和度 (Saturation) 固定为 1.0 (全彩)
            float saturation = 1.0f;
            // 明度 (Value) 从下到上从 0.3 到 1.0 变化 (让底部暗一些，顶部亮一些)
            float value = 0.3f + 0.7f * fy;

            // 将 HSV 转换为 RGB (使用标准转换算法)
            // 为了清晰，我们内联实现这个转换，避免额外依赖
            float c = value * saturation;
            float x_component = c * (1.0f - std::fabs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
            float m = value - c;

            float r, g, b;
            int segment = static_cast<int>(hue / 60.0f) % 6;
            switch (segment) {
            case 0: r = c; g = x_component; b = 0.0f; break;
            case 1: r = x_component; g = c; b = 0.0f; break;
            case 2: r = 0.0f; g = c; b = x_component; break;
            case 3: r = 0.0f; g = x_component; b = c; break;
            case 4: r = x_component; g = 0.0f; b = c; break;
            default: r = c; g = 0.0f; b = x_component; break;
            }

            // 转换为 0-255 范围的 uint8_t
            uint8_t red = static_cast<uint8_t>((r + m) * 255.0f);
            uint8_t green = static_cast<uint8_t>((g + m) * 255.0f);
            uint8_t blue = static_cast<uint8_t>((b + m) * 255.0f);

            // 使用我们的 Color 结构体构造颜色
            Fern::Color pixelColor(red, green, blue);

            // 绘制像素
            window.DrawPixel(x, y, pixelColor);
        }

    }

    // 3. 将后台缓冲区刷新到屏幕 (所有像素一次性显示)
    window.Present();


    // 4. 保持窗口打开，直到用户按下 ESC 键或关闭窗口

    while (!window.ShouldClose()) {
        window.PullEvents();
        Sleep(10); // 小休眠，降低 CPU 占用
    }


    return 0;
}