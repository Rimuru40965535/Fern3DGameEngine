/**
* @file 11-MinimalPipeline.cpp
* @brief 最小可用渲染管线
* @details
* 验证从顶点变换到光栅化的完整流程。
* 暂不包含深度测试和近平面裁剪。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#include "Fern3DGameEngine.h"

using namespace Fern;

// ========== 临时光栅化函数 ==========

/**
* @brief 判断点是否在三角形内（同侧法）
*/
inline int CrossProduct2D(int Ax, int Ay, int Bx, int By, int Px, int Py) {
    return (Bx - Ax) * (Py - Ay) - (By - Ay) * (Px - Ax);
}

/**
* @brief 判断点是否在三角形内部
*/
inline bool PointInTriangle(int Ax, int Ay, int Bx, int By, int Cx, int Cy, int Px, int Py) {
    int d1 = CrossProduct2D(Ax, Ay, Bx, By, Px, Py);
    int d2 = CrossProduct2D(Bx, By, Cx, Cy, Px, Py);
    int d3 = CrossProduct2D(Cx, Cy, Ax, Ay, Px, Py);

    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);

    return !(hasPos && hasNeg);
}

/**
* @brief 填充三角形（包围盒遍历法，无深度测试）
*/
void FastDrawFillTriangle(
    const Vector3D& A,
    const Vector3D& B,
    const Vector3D& C,
    const Color& color,
    Window* window
) {
    int Ax = (int)(A.x + 0.5);
    int Ay = (int)(A.y + 0.5);
    int Bx = (int)(B.x + 0.5);
    int By = (int)(B.y + 0.5);
    int Cx = (int)(C.x + 0.5);
    int Cy = (int)(C.y + 0.5);

    // 计算包围盒
    int xMin = Ax, xMax = Ax;
    if (Bx < xMin) xMin = Bx;
    if (Bx > xMax) xMax = Bx;
    if (Cx < xMin) xMin = Cx;
    if (Cx > xMax) xMax = Cx;

    int yMin = Ay, yMax = Ay;
    if (By < yMin) yMin = By;
    if (By > yMax) yMax = By;
    if (Cy < yMin) yMin = Cy;
    if (Cy > yMax) yMax = Cy;

    // 遍历包围盒
    for (int y = yMin; y <= yMax; ++y) {
        for (int x = xMin; x <= xMax; ++x) {
            if (PointInTriangle(Ax, Ay, Bx, By, Cx, Cy, x, y)) {
                window->DrawPixel(x, y, color);
            }
        }
    }
}


// ========== 主函数 ==========

int main() {
    int screenWidth = 1200;
    int screenHeight = 900;

    Window window(screenWidth, screenHeight, "Minimal Pipeline", !EX_SHOWCONSOLE);

    // 创建相机
    Scene::Camera camera;
    camera.SetPosition(Vector3D(0, 0, -5));
    camera.SetFov(Math::PI / 3.0);
    camera.SetNearPlane(0.1);
    camera.SetFarPlane(100.0);

    // 创建立方体
    Scene::Mesh cubeMesh = Scene::CreateCubeMesh();
    Scene::RenderObject cube(&cubeMesh, Color::Red());

    // 主循环
    double angle = 0.0;

    while (!window.ShouldClose()) {
        window.Clear();
        window.PullEvents();

        // 1. 更新旋转
        angle += 0.01;
        cube.SetRotation(Vector3D(0, angle, 0));

        // 2. 计算观察矩阵和投影矩阵
        Matrix4x4 model = cube.GetModelMatrix();
        Matrix4x4 view = ~camera.GetModelMatrix();
        Matrix4x4 proj = camera.GetProjMatrix(window);

        // 3. 变换所有顶点到屏幕空间
        std::vector<Vector3D> screenVerts;
        screenVerts.reserve(cubeMesh.vertices.size());

        Matrix4x4 transformMatrix = model * view * proj * window.windowMatrix;

        for (const auto& v : cubeMesh.vertices) {
            Vector3D screenPos = transformMatrix * v; //一步到位
            screenVerts.push_back(screenPos);
        }

        // 4. 绘制所有三角形
        for (const auto& surface : cubeMesh.surfaces) {
            FastDrawFillTriangle(
                screenVerts[surface.verticeA],
                screenVerts[surface.verticeB],
                screenVerts[surface.verticeC],
                cube.GetColor(),
                &window
            );
        }

        // 5. 呈现
        window.Present();
        Sleep(16);
    }

    return 0;
}