/**
* @file 15-FastFillTriangle_test.cpp
* @brief 三角形填充算法单元测试
* @details
* 验证 FastFillTriangle 函数的正确性，包括深度测试、深度插值、边界处理。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/10/6
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
    Window window(1200, 900, "FastFillTriangle Test", !EX_SHOWCONSOLE);

    // ========== 测试1：空指针保护 ==========
    std::cout << "========== 测试1：空指针保护 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        Vector3D A(100, 100, 0.5);
        Vector3D B(200, 100, 0.5);
        Vector3D C(150, 200, 0.5);

        // 空指针调用，不应该崩溃
        Render::FastFillTriangle(nullptr, &depthBuff, A, B, C, Color::Red());
        Render::FastFillTriangle(&target, nullptr, A, B, C, Color::Red());
        Render::FastFillTriangle(nullptr, nullptr, A, B, C, Color::Red());

        std::cout << "空指针调用: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试2：退化三角形（三点共线） ==========
    std::cout << "========== 测试2：退化三角形 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 三点共线
        Vector3D A(100, 100, 0.5);
        Vector3D B(200, 200, 0.5);
        Vector3D C(300, 300, 0.5);

        Render::FastFillTriangle(&target, &depthBuff, A, B, C, Color::Red());

        std::cout << "三点共线: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试3：普通三角形 ==========
    std::cout << "========== 测试3：普通三角形 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        Vector3D A(100, 100, 0.5);
        Vector3D B(200, 100, 0.5);
        Vector3D C(150, 200, 0.5);

        Render::FastFillTriangle(&target, &depthBuff, A, B, C, Color::Red());

        // 验证中心点深度被更新
        bool centerOk = NearlyEqual(depthBuff.GetDepth(150, 150), 0.5);

        std::cout << "中心点深度更新: " << (centerOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (centerOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：深度插值 ==========
    std::cout << "========== 测试4：深度插值 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        Vector3D A(100, 100, 0.2);
        Vector3D B(200, 100, 0.8);
        Vector3D C(150, 200, 0.5);

        Render::FastFillTriangle(&target, &depthBuff, A, B, C, Color::Red());

        // 验证底部点深度接近 0.5
        double bottomDepth = depthBuff.GetDepth(150, 190);
        bool bottomOk = NearlyEqual(bottomDepth, 0.5, 0.1);

        std::cout << "底部点深度: " << bottomDepth << " (期望 0.5)" << std::endl;
        std::cout << "结果: " << (bottomOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：深度测试遮挡 ==========
    std::cout << "========== 测试5：深度测试遮挡 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 先画一个深度 0.3 的三角形
        Vector3D A1(100, 100, 0.3);
        Vector3D B1(200, 100, 0.3);
        Vector3D C1(150, 200, 0.3);
        Render::FastFillTriangle(&target, &depthBuff, A1, B1, C1, Color::Red());

        // 再画一个深度 0.8 的三角形（更远，应该被遮挡）
        Vector3D A2(100, 100, 0.8);
        Vector3D B2(200, 100, 0.8);
        Vector3D C2(150, 200, 0.8);
        Render::FastFillTriangle(&target, &depthBuff, A2, B2, C2, Color::Green());

        // 深度应该保持 0.3
        bool unchanged = NearlyEqual(depthBuff.GetDepth(150, 150), 0.3);

        std::cout << "深度保持 0.3: " << (unchanged ? "v" : "x") << std::endl;
        std::cout << "结果: " << (unchanged ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：水平边 ==========
    std::cout << "========== 测试6：水平边 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 顶点 y 值有相同的
        Vector3D A(100, 100, 0.5);
        Vector3D B(200, 100, 0.5);
        Vector3D C(150, 200, 0.5);

        Render::FastFillTriangle(&target, &depthBuff, A, B, C, Color::Red());

        // 验证不会崩溃
        std::cout << "水平边处理: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试7：垂直边 ==========
    std::cout << "========== 测试7：垂直边 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 顶点 x 值有相同的
        Vector3D A(100, 100, 0.5);
        Vector3D B(100, 200, 0.5);
        Vector3D C(200, 150, 0.5);

        Render::FastFillTriangle(&target, &depthBuff, A, B, C, Color::Red());

        std::cout << "垂直边处理: v" << std::endl;
        std::cout << "结果: v 通过" << std::endl << std::endl;
    }

    // ========== 测试8：反方向顶点顺序 ==========
    std::cout << "========== 测试8：反方向顶点顺序 ==========" << std::endl;
    {
        Render::RenderTarget target(window);
        Render::DepthBuff depthBuff(window);

        // 顶点顺序不同，但仍然是同一个三角形
        Vector3D A(200, 100, 0.5);
        Vector3D B(100, 100, 0.5);
        Vector3D C(150, 200, 0.5);

        Render::FastFillTriangle(&target, &depthBuff, A, B, C, Color::Red());

        bool centerOk = NearlyEqual(depthBuff.GetDepth(150, 150), 0.5);

        std::cout << "中心点深度更新: " << (centerOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (centerOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    return 0;
}