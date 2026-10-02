/**
* @file 14-RenderUtils_test.cpp
* @brief 渲染工具单元测试
* @details
* 验证 RenderTarget 类和 DrawWithDepthTest 函数的正确性。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/10/1
*
*/
#include "Fern3DGameEngine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace Fern;

// 辅助函数：判断两个 double 是否接近
bool NearlyEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // 创建测试窗口
    Window window(800, 600, "RenderUtils Test", !EX_SHOWCONSOLE);

    // ========== 测试1：RenderTarget 构造 ==========
    std::cout << "========== 测试1：RenderTarget 构造 ==========" << std::endl;
    {
        Render::RenderTarget target(window);

        bool widthOk = (target.Width() == 800);
        bool heightOk = (target.Height() == 600);

        std::cout << "宽度: " << target.Width() << " (期望 800)" << std::endl;
        std::cout << "高度: " << target.Height() << " (期望 600)" << std::endl;
        std::cout << "结果: " << (widthOk && heightOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试2：DrawPixel 有效坐标 ==========
    std::cout << "========== 测试2：DrawPixel 有效坐标 ==========" << std::endl;
    {
        Render::RenderTarget target(window);

        // 这些调用不应该崩溃
        target.DrawPixel(0, 0, Color::Red());
        target.DrawPixel(400, 300, Color::Green());
        target.DrawPixel(799, 599, Color::Blue());

        std::cout << "有效坐标绘制: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试3：DrawPixel 越界坐标 ==========
    std::cout << "========== 测试3：DrawPixel 越界坐标 ==========" << std::endl;
    {
        Render::RenderTarget target(window);

        // 这些调用不应该崩溃，也不应该绘制
        target.DrawPixel(-1, 0, Color::Red());
        target.DrawPixel(0, -1, Color::Red());
        target.DrawPixel(800, 0, Color::Red());
        target.DrawPixel(0, 600, Color::Red());

        std::cout << "越界坐标绘制: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试4：Clear 和 Present ==========
    std::cout << "========== 测试4：Clear 和 Present ==========" << std::endl;
    {
        Render::RenderTarget target(window);

        target.Clear();
        target.Present();

        std::cout << "Clear 和 Present 调用: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试5：DrawWithDepthTest 首次绘制 ==========
    std::cout << "========== 测试5：DrawWithDepthTest 首次绘制 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 第一次绘制，深度为 0.5，缓冲为 1.5
        Render::DrawWithDepthTest(&target, &depthBuff, 100, 100, 0.5, Color::Red());

        bool depthUpdated = NearlyEqual(depthBuff.GetDepth(100, 100), 0.5);

        std::cout << "深度缓冲更新为 0.5: " << (depthUpdated ? "v" : "x") << std::endl;
        std::cout << "结果: " << (depthUpdated ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：DrawWithDepthTest 更近的像素 ==========
    std::cout << "========== 测试6：DrawWithDepthTest 更近的像素 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 先绘制深度 0.5
        Render::DrawWithDepthTest(&target, &depthBuff, 100, 100, 0.5, Color::Red());

        // 再绘制深度 0.3（更近）
        Render::DrawWithDepthTest(&target, &depthBuff, 100, 100, 0.3, Color::Green());

        bool depthUpdated = NearlyEqual(depthBuff.GetDepth(100, 100), 0.3);

        std::cout << "深度缓冲更新为 0.3: " << (depthUpdated ? "v" : "x") << std::endl;
        std::cout << "结果: " << (depthUpdated ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试7：DrawWithDepthTest 更远的像素 ==========
    std::cout << "========== 测试7：DrawWithDepthTest 更远的像素 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 先绘制深度 0.3
        Render::DrawWithDepthTest(&target, &depthBuff, 100, 100, 0.3, Color::Red());

        // 再绘制深度 0.8（更远）
        Render::DrawWithDepthTest(&target, &depthBuff, 100, 100, 0.8, Color::Green());

        bool depthUnchanged = NearlyEqual(depthBuff.GetDepth(100, 100), 0.3);

        std::cout << "深度缓冲保持 0.3: " << (depthUnchanged ? "v" : "x") << std::endl;
        std::cout << "结果: " << (depthUnchanged ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试8：DrawWithDepthTest 空指针保护 ==========
    std::cout << "========== 测试8：DrawWithDepthTest 空指针保护 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 空指针调用，不应该崩溃
        Render::DrawWithDepthTest(nullptr, &depthBuff, 100, 100, 0.5, Color::Red());
        Render::DrawWithDepthTest(&target, nullptr, 100, 100, 0.5, Color::Red());
        Render::DrawWithDepthTest(nullptr, nullptr, 100, 100, 0.5, Color::Red());

        std::cout << "空指针调用: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    return 0;
}