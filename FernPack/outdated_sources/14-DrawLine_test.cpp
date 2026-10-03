/**
* @file 14-DrawLine_test.cpp
* @brief 直线绘制算法单元测试
* @details
* 验证 DrawLine 函数的正确性，包括深度测试、深度插值、边界处理。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/10/3
*
*/
#include "Fern3DGameEngine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace Fern;

// 辅助函数：判断两个 double 是否接近
bool NearlyEqual(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // 创建测试窗口
    Window window(800, 600, "DrawLine Test", !EX_SHOWCONSOLE);

    // ========== 测试1：空指针保护 ==========
    std::cout << "========== 测试1：空指针保护 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 空指针调用，不应该崩溃
        Render::DrawLine(nullptr, &depthBuff,
            Vector3D(0, 0, 0.5), Vector3D(100, 100, 0.5), Color::Red());
        Render::DrawLine(&target, nullptr,
            Vector3D(0, 0, 0.5), Vector3D(100, 100, 0.5), Color::Red());
        Render::DrawLine(nullptr, nullptr,
            Vector3D(0, 0, 0.5), Vector3D(100, 100, 0.5), Color::Red());

        std::cout << "空指针调用: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试2：起点终点相同 ==========
    std::cout << "========== 测试2：起点终点相同 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 起点终点相同，应该绘制一个点
        Render::DrawLine(&target, &depthBuff,
            Vector3D(400, 300, 0.5), Vector3D(400, 300, 0.5), Color::Red());

        bool depthUpdated = NearlyEqual(depthBuff.GetDepth(400, 300), 0.5);

        std::cout << "深度缓冲更新为 0.5: " << (depthUpdated ? "v" : "x") << std::endl;
        std::cout << "结果: " << (depthUpdated ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试3：水平线 ==========
    std::cout << "========== 测试3：水平线 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 从 (100, 300) 到 (200, 300)
        Render::DrawLine(&target, &depthBuff,
            Vector3D(100, 300, 0.5), Vector3D(200, 300, 0.5), Color::Red());

        // 验证深度缓冲被更新
        bool startOk = NearlyEqual(depthBuff.GetDepth(100, 300), 0.5);
        bool endOk = NearlyEqual(depthBuff.GetDepth(200, 300), 0.5);

        std::cout << "起点深度更新: " << (startOk ? "v" : "x") << std::endl;
        std::cout << "终点深度更新: " << (endOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (startOk && endOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：垂直线 ==========
    std::cout << "========== 测试4：垂直线 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 从 (400, 100) 到 (400, 200)
        Render::DrawLine(&target, &depthBuff,
            Vector3D(400, 100, 0.5), Vector3D(400, 200, 0.5), Color::Red());

        bool startOk = NearlyEqual(depthBuff.GetDepth(400, 100), 0.5);
        bool endOk = NearlyEqual(depthBuff.GetDepth(400, 200), 0.5);

        std::cout << "起点深度更新: " << (startOk ? "v" : "x") << std::endl;
        std::cout << "终点深度更新: " << (endOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (startOk && endOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：对角线 ==========
    std::cout << "========== 测试5：对角线 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 从 (100, 100) 到 (200, 200)
        Render::DrawLine(&target, &depthBuff,
            Vector3D(100, 100, 0.5), Vector3D(200, 200, 0.5), Color::Red());

        bool startOk = NearlyEqual(depthBuff.GetDepth(100, 100), 0.5);
        bool endOk = NearlyEqual(depthBuff.GetDepth(200, 200), 0.5);

        std::cout << "起点深度更新: " << (startOk ? "v" : "x") << std::endl;
        std::cout << "终点深度更新: " << (endOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (startOk && endOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：深度插值 ==========
    std::cout << "========== 测试6：深度插值 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 从 (100, 300) 到 (200, 300)，深度从 0.2 到 0.8
        Render::DrawLine(&target, &depthBuff,
            Vector3D(100, 300, 0.2), Vector3D(200, 300, 0.8), Color::Red());

        // 起点深度应该是 0.2
        bool startOk = NearlyEqual(depthBuff.GetDepth(100, 300), 0.2, 1e-5);
        // 终点深度应该是 0.8
        bool endOk = NearlyEqual(depthBuff.GetDepth(200, 300), 0.8, 1e-5);
        // 中点深度应该在 0.2 和 0.8 之间
        double midDepth = depthBuff.GetDepth(150, 300);
        bool midOk = (midDepth > 0.4 && midDepth < 0.6);

        std::cout << "起点深度: " << depthBuff.GetDepth(100, 300) << " (期望 0.2)" << std::endl;
        std::cout << "终点深度: " << depthBuff.GetDepth(200, 300) << " (期望 0.8)" << std::endl;
        std::cout << "中点深度: " << midDepth << " (期望 0.4~0.6)" << std::endl;
        std::cout << "结果: " << (startOk && endOk && midOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试7：深度测试遮挡 ==========
    std::cout << "========== 测试7：深度测试遮挡 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 先画一条深度 0.3 的线
        Render::DrawLine(&target, &depthBuff,
            Vector3D(100, 300, 0.3), Vector3D(200, 300, 0.3), Color::Red());

        // 再画一条深度 0.8 的线（更远，应该被遮挡）
        Render::DrawLine(&target, &depthBuff,
            Vector3D(100, 300, 0.8), Vector3D(200, 300, 0.8), Color::Green());

        // 深度应该保持 0.3
        bool unchanged = NearlyEqual(depthBuff.GetDepth(150, 300), 0.3);

        std::cout << "深度保持 0.3: " << (unchanged ? "v" : "x") << std::endl;
        std::cout << "结果: " << (unchanged ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试8：负方向绘制 ==========
    std::cout << "========== 测试8：负方向绘制 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 从 (200, 200) 到 (100, 100)（反方向）
        Render::DrawLine(&target, &depthBuff,
            Vector3D(200, 200, 0.5), Vector3D(100, 100, 0.5), Color::Red());

        bool startOk = NearlyEqual(depthBuff.GetDepth(200, 200), 0.5);
        bool endOk = NearlyEqual(depthBuff.GetDepth(100, 100), 0.5);

        std::cout << "起点深度更新: " << (startOk ? "v" : "x") << std::endl;
        std::cout << "终点深度更新: " << (endOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (startOk && endOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    return 0;
}