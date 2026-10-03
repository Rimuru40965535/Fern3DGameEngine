/**
* @file Pipe02.cpp
* @brief 直线绘制联调测试
* @details
* 使用 DrawLine 渲染一个旋转的立方体线框。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/10/3
*
*/
#include "Fern3DGameEngine.h"
#include <vector>

using namespace Fern;

int main() {
    int screenWidth = 1200;
    int screenHeight = 900;

    Window window(screenWidth, screenHeight, "Pipe02 - Wireframe Cube", !EX_SHOWCONSOLE);
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

    while (!window.ShouldClose()) {
        window.PullEvents();
        target.Clear();
        depthBuff.Reset();

        // 1. 更新旋转
        angle += 0.01;
        cube.SetRotation(Vector3D(0, angle, 0));

        // 2. 计算矩阵
        Matrix4x4 model = cube.GetModelMatrix();
        Matrix4x4 view = ~camera.GetModelMatrix();
        Matrix4x4 proj = camera.GetProjMatrix(window);
        Matrix4x4 mvp = model * view * proj;

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

        // 4. 绘制所有边
        for (const auto& edge : cubeMesh.edges) {
            Render::DrawLine(
                &target,
                &depthBuff,
                screenVerts[edge.verticeStart],
                screenVerts[edge.verticeEnd],
                edge.color
            );
        }

        // 5. 呈现
        target.Present();
        Sleep(16);
    }

    return 0;
}