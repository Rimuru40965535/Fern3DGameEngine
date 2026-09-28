/**
* @file 09-Projection_test.cpp
* @brief 透视投影矩阵单元测试
* @details
* 验证 GeneratePerspectiveMatrix 函数的正确性。
*
* @author 半人马座beta星（AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#include "Fern3DGameEngine.h"

#include <cmath>

using namespace Fern;

// 辅助函数：判断两个 double 是否接近
bool NearlyEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

// 辅助函数：判断两个 Vector3D 是否接近
bool NearlyEqual(const Vector3D& a, const Vector3D& b, double eps = 1e-9) {
    return NearlyEqual(a.x, b.x, eps)
        && NearlyEqual(a.y, b.y, eps)
        && NearlyEqual(a.z, b.z, eps);
}

// 辅助函数：用投影矩阵变换一个点，并做透视除法
Vector3D ApplyProjection(const Matrix4x4& proj, const Vector3D& point) {
    double x = proj.values[0][0] * point.x;
    double y = proj.values[1][1] * point.y;
    double z = proj.values[2][2] * point.z + proj.values[2][3];
    double w = proj.values[3][2] * point.z;

    if (std::fabs(w) < 1e-10) return Vector3D(0, 0, 0);

    return Vector3D(x / w, y / w, z / w);
}

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // ========== 测试1：无效参数 ==========
    std::cout << "========== 测试1：无效参数 ==========" << std::endl;
    {
        Matrix4x4 m1 = GeneratePerspectiveMatrix(800, 600, 1.0472, -0.1, 100);
        Matrix4x4 m2 = GeneratePerspectiveMatrix(800, 600, 1.0472, 10.0, 5.0);

        bool m1Ok = (m1.values[0][0] == 1.0 && m1.values[1][1] == 1.0);
        bool m2Ok = (m2.values[0][0] == 1.0 && m2.values[1][1] == 1.0);

        std::cout << "__near < 0 时返回单位矩阵: " << (m1Ok ? "v" : "x") << std::endl;
        std::cout << "__far < __near 时返回单位矩阵: " << (m2Ok ? "v" : "x") << std::endl;
        std::cout << "结果: " << (m1Ok && m2Ok ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试2：正方形窗口 ==========
    std::cout << "========== 测试2：正方形窗口 ==========" << std::endl;
    {
        double fov = 1.0472;
        Matrix4x4 proj = GeneratePerspectiveMatrix(800, 800, fov, 0.1, 100);

        double expected = 1.0 / Math::Tangent(fov / 2);
        bool xOk = NearlyEqual(proj.values[0][0], expected);
        bool yOk = NearlyEqual(proj.values[1][1], expected);

        std::cout << "x_halfrange 正确: " << (xOk ? "v" : "x") << std::endl;
        std::cout << "y_halfrange 正确: " << (yOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (xOk && yOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试3：宽屏窗口 ==========
    std::cout << "========== 测试3：宽屏窗口 ==========" << std::endl;
    {
        double fov = 1.0472;
        Matrix4x4 proj = GeneratePerspectiveMatrix(1600, 900, fov, 0.1, 100);

        double radius = Math::Tangent(fov / 2);
        double expectedY = 1.0 / radius;
        double expectedX = 1.0 / (radius * 1600.0 / 900.0);

        bool xOk = NearlyEqual(proj.values[0][0], expectedX);
        bool yOk = NearlyEqual(proj.values[1][1], expectedY);
        bool xSmaller = (proj.values[0][0] < proj.values[1][1]);

        std::cout << "x 缩放系数正确: " << (xOk ? "v" : "x") << std::endl;
        std::cout << "y 缩放系数正确: " << (yOk ? "v" : "x") << std::endl;
        std::cout << "x 缩放小于 y 缩放: " << (xSmaller ? "v" : "x") << std::endl;
        std::cout << "结果: " << (xOk && yOk && xSmaller ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：竖屏窗口 ==========
    std::cout << "========== 测试4：竖屏窗口 ==========" << std::endl;
    {
        double fov = 1.0472;
        Matrix4x4 proj = GeneratePerspectiveMatrix(900, 1600, fov, 0.1, 100);

        double radius = Math::Tangent(fov / 2);
        double expectedX = 1.0 / radius;
        double expectedY = 1.0 / (radius * 1600.0 / 900.0);

        bool xOk = NearlyEqual(proj.values[0][0], expectedX);
        bool yOk = NearlyEqual(proj.values[1][1], expectedY);
        bool ySmaller = (proj.values[1][1] < proj.values[0][0]);

        std::cout << "x 缩放系数正确: " << (xOk ? "v" : "x") << std::endl;
        std::cout << "y 缩放系数正确: " << (yOk ? "v" : "x") << std::endl;
        std::cout << "y 缩放小于 x 缩放: " << (ySmaller ? "v" : "x") << std::endl;
        std::cout << "结果: " << (xOk && yOk && ySmaller ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：近平面映射 ==========
    std::cout << "========== 测试5：近平面映射 ==========" << std::endl;
    {
        double __near = 0.1;
        double __far = 100.0;
        Matrix4x4 proj = GeneratePerspectiveMatrix(800, 600, 1.0472, __near, __far);

        Vector3D ndc = ApplyProjection(proj, Vector3D(0, 0, __near));

        bool zOk = NearlyEqual(ndc.z, 0.0);

        std::cout << "近平面 z_ndc: " << ndc.z << " (期望 0)" << std::endl;
        std::cout << "结果: " << (zOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：远平面映射 ==========
    std::cout << "========== 测试6：远平面映射 ==========" << std::endl;
    {
        double __near = 0.1;
        double __far = 100.0;
        Matrix4x4 proj = GeneratePerspectiveMatrix(800, 600, 1.0472, __near, __far);

        Vector3D ndc = ApplyProjection(proj, Vector3D(0, 0, __far));

        bool zOk = NearlyEqual(ndc.z, 1.0);

        std::cout << "远平面 z_ndc: " << ndc.z << " (期望 1)" << std::endl;
        std::cout << "结果: " << (zOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试7：近平面右上角 ==========
    std::cout << "========== 测试7：近平面右上角 ==========" << std::endl;
    {
        double __near = 0.1;
        double __far = 100.0;
        double fov = 1.0472;
        double radius = Math::Tangent(fov / 2);

        Matrix4x4 proj = GeneratePerspectiveMatrix(800, 600, fov, __near, __far);

        // 近平面右上角：x = radius * aspect * __near, y = radius * __near
        double aspect = 800.0 / 600.0;
        double x = radius * aspect * __near;
        double y = radius * __near;

        Vector3D ndc = ApplyProjection(proj, Vector3D(x, y, __near));

        bool xOk = NearlyEqual(ndc.x, 1.0);
        bool yOk = NearlyEqual(ndc.y, 1.0);
        bool zOk = NearlyEqual(ndc.z, 0.0);

        std::cout << "x_ndc: " << ndc.x << " (期望 1)" << std::endl;
        std::cout << "y_ndc: " << ndc.y << " (期望 1)" << std::endl;
        std::cout << "z_ndc: " << ndc.z << " (期望 0)" << std::endl;
        std::cout << "结果: " << (xOk && yOk && zOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试8：远平面左下角 ==========
    std::cout << "========== 测试8：远平面左下角 ==========" << std::endl;
    {
        double __near = 0.1;
        double __far = 100.0;
        double fov = 1.0472;
        double radius = Math::Tangent(fov / 2);

        Matrix4x4 proj = GeneratePerspectiveMatrix(800, 600, fov, __near, __far);

        // 远平面左下角：x = -radius * aspect * __far, y = -radius * __far
        double aspect = 800.0 / 600.0;
        double x = -radius * aspect * __far;
        double y = -radius * __far;

        Vector3D ndc = ApplyProjection(proj, Vector3D(x, y, __far));

        bool xOk = NearlyEqual(ndc.x, -1.0);
        bool yOk = NearlyEqual(ndc.y, -1.0);
        bool zOk = NearlyEqual(ndc.z, 1.0);

        std::cout << "x_ndc: " << ndc.x << " (期望 -1)" << std::endl;
        std::cout << "y_ndc: " << ndc.y << " (期望 -1)" << std::endl;
        std::cout << "z_ndc: " << ndc.z << " (期望 1)" << std::endl;
        std::cout << "结果: " << (xOk && yOk && zOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    return 0;
}