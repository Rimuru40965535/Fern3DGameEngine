/**
* @file Pipe03.cpp
* @brief 三角形填充算法联调测试
* @details
* 使用 FastFillTriangle 渲染一个旋转的纯色实体立方体。
* 本文件为DeepSeek阅读代码以后独立生成的测试文件。
* 在这个文件中我需要把渲染管线的各个组分排列好，未来自己实现多线程的时候在那里组装。
* @author DeepSeek
* @date 2026/10/6
*
*/
#include "Fern3DGameEngine.h"
#include <vector>

using namespace Fern;

int main() {
    int screenWidth = 1200;
    int screenHeight = 900;

    Window window(screenWidth, screenHeight, "Pipe03 - Solid Cube", !EX_SHOWCONSOLE);
    Render::RenderTarget target(window);
    Render::DepthBuff depthBuff(window);

    // 创建相机
    Scene::Camera camera;
    camera.SetPosition(Vector3D(0, 0, -5));
    camera.SetFov(Math::PI / 3.0);
    camera.SetNearPlane(0.1);
    camera.SetFarPlane(100.0);

    // 创建立方体
    Scene::Mesh cubeMesh = Scene::CreateCubeMesh();
    Scene::RenderObject cube(&cubeMesh, Color::White());

    // 主循环
    double angle = 0.0;
    //double angle = 0.01 * 1 ;

    while (!window.ShouldClose()) {
        window.PullEvents();
        target.Clear();
        depthBuff.Reset();

        // 1. 更新旋转
        angle += 0.01;
        cube.SetRotation(Vector3D(angle * 0.5, angle, 0));

        // 2. 计算矩阵
        Matrix4x4 model = cube.GetModelMatrix();
        Matrix4x4 view = ~camera.GetModelMatrix();
        Matrix4x4 proj = camera.GetProjMatrix(window);

        // 3. 变换所有顶点到屏幕空间
        std::vector<Vector3D> screenVerts;
        screenVerts.reserve(cubeMesh.vertices.size());

        for (const auto& v : cubeMesh.vertices) {
            Vector3D worldPos = model * v;                      // 模型 → 世界
            Vector3D viewPos = view * worldPos;                  // 世界 → 相机
            Vector3D ndcPos = proj * viewPos;                    // 相机 → NDC
            Vector3D screenPos = window.windowMatrix * ndcPos;   // NDC → 屏幕
            screenVerts.push_back(screenPos);
        }

        // 4. 绘制所有三角形
        for (const auto& surface : cubeMesh.surfaces) {
            Render::FastFillTriangle(
                &target,
                &depthBuff,
                screenVerts[surface.verticeA],
                screenVerts[surface.verticeB],
                screenVerts[surface.verticeC],
                surface.color
            );
        }

        
        // 5. 绘制所有边（可选，用于调试）
        for (const auto& edge : cubeMesh.edges) {
            Render::DrawLine(
                &target,
                &depthBuff,
                screenVerts[edge.verticeStart],
                screenVerts[edge.verticeEnd],
                edge.color
            );
        }
        

        // 6. 呈现
        target.Present();
        Sleep(16);
    }

    return 0;
}