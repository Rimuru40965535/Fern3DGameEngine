/**
* @file 04-Inverse_test.cpp
* @brief 仿射矩阵求逆测试
* @datail
* 
* @author 半人马座beta星
* @date 2026.9.26
* 
*/

#include "Fern3DGameEngine.h"

using namespace Fern;

// 打印矩阵
void PrintMatrix(const Matrix4x4& m, const std::string& name) {
    std::cout << name << ":" << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(12) << std::setprecision(6) << std::fixed
                << m.values[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 验证 M * M⁻¹ 是否接近单位矩阵
bool VerifyIdentity(const Matrix4x4& m, double eps = 1e-9) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (std::fabs(m.values[i][j] - expected) > eps) {
                return false;
            }
        }
    }
    return true;
}

// 验证两个矩阵是否接近相等
bool VerifyEqual(const Matrix4x4& a, const Matrix4x4& b, double eps = 1e-9) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (std::fabs(a.values[i][j] - b.values[i][j]) > eps) {
                return false;
            }
        }
    }
    return true;
}

int main() {

    std::cout << std::setprecision(6) << std::fixed;

    // ========== 测试1：单位矩阵 ==========
    std::cout << "========== 测试1：单位矩阵 ==========" << std::endl;
    Matrix4x4 identity;  // 默认构造为单位矩阵
    Matrix4x4 inv1 = ~identity;
    PrintMatrix(inv1, "单位矩阵的逆");
    std::cout << "结果: " << (VerifyIdentity(inv1) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试2：纯平移 ==========
    std::cout << "========== 测试2：纯平移 ==========" << std::endl;
    Matrix4x4 trans = Matrix4x4::Translation(3, -2, 5);
    PrintMatrix(trans, "平移矩阵");
    Matrix4x4 inv2 = ~trans;
    PrintMatrix(inv2, "平移矩阵的逆");
    std::cout << "结果: " << (VerifyIdentity(trans * inv2) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试3：纯旋转 ==========
    std::cout << "========== 测试3：纯旋转 ==========" << std::endl;
    Matrix4x4 rot = Matrix4x4::RotationY(0.7) * Matrix4x4::RotationX(0.3);
    PrintMatrix(rot, "旋转矩阵");
    Matrix4x4 inv3 = ~rot;
    PrintMatrix(inv3, "旋转矩阵的逆");
    std::cout << "结果: " << (VerifyIdentity(rot * inv3) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试4：纯缩放 ==========
    std::cout << "========== 测试4：纯缩放 ==========" << std::endl;
    Matrix4x4 scale = Matrix4x4::Scale(2.0, 3.0, 0.5);
    PrintMatrix(scale, "缩放矩阵");
    Matrix4x4 inv4 = ~scale;
    PrintMatrix(inv4, "缩放矩阵的逆");
    std::cout << "结果: " << (VerifyIdentity(scale * inv4) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试5：复合变换 ==========
    std::cout << "========== 测试5：复合变换 ==========" << std::endl;
    Matrix4x4 compound = Matrix4x4::Translation(1, 2, 3)
        * Matrix4x4::RotationY(0.5)
        * Matrix4x4::Scale(2.0, 1.5, 0.8);
    PrintMatrix(compound, "复合矩阵");
    Matrix4x4 inv5 = ~compound;
    PrintMatrix(inv5, "复合矩阵的逆");
    std::cout << "结果: " << (VerifyIdentity(compound * inv5) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试6：相机模型矩阵 ==========
    std::cout << "========== 测试6：相机模型矩阵 ==========" << std::endl;

    // 构造一个相机模型矩阵：先平移到相机位置，再旋转
    Vector3D cameraPos(1.5, 1.5, 1.5);
    Matrix4x4 cameraModel = Matrix4x4::Translation(cameraPos)
        * Matrix4x4::RotationY(0.5)
        * Matrix4x4::RotationX(-0.3);
    PrintMatrix(cameraModel, "相机模型矩阵");

    // 观察矩阵 = 相机模型矩阵的逆
    Matrix4x4 viewMatrix = ~cameraModel;
    PrintMatrix(viewMatrix, "观察矩阵");

    // 验证：viewMatrix * cameraModel = 单位矩阵
    std::cout << "结果: " << (VerifyIdentity(viewMatrix * cameraModel) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试7：变换点验证 ==========
    std::cout << "========== 测试7：变换点验证 ==========" << std::endl;

    Vector3D point(1, 2, 3);
    Vector3D transformed = compound * point;
    Vector3D recovered = inv5 * transformed;

    std::cout << "原始点: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
    std::cout << "变换后: (" << transformed.x << ", " << transformed.y << ", " << transformed.z << ")" << std::endl;
    std::cout << "逆变换: (" << recovered.x << ", " << recovered.y << ", " << recovered.z << ")" << std::endl;

    bool pointOk = std::fabs(recovered.x - point.x) < 1e-6
        && std::fabs(recovered.y - point.y) < 1e-6
        && std::fabs(recovered.z - point.z) < 1e-6;
    std::cout << "结果: " << (pointOk ? "通过" : "失败") << std::endl;

    return 0;
}