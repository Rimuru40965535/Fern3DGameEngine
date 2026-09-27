/**
* @file 05-RotateTest.cpp
* @brief 矩阵转置测试
* 
* @author 半人马座beta星
* @date 2026-9-27
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

// 计算行列式（用于测试）
double Determinant(const Matrix4x4& m) {
    // 简化版：仅用于测试，使用展开法
    double det = 0;
    // ... 实际实现略，测试中可以直接比较转置前后的行列式
    // 这里用一个简单的方式：对于仿射矩阵，det = 3×3 部分的 det
    double a = m.values[0][0], b = m.values[0][1], c = m.values[0][2];
    double d = m.values[1][0], e = m.values[1][1], f = m.values[1][2];
    double g = m.values[2][0], h = m.values[2][1], i = m.values[2][2];
    det = a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
    return det;
}

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // ========== 测试1：单位矩阵转置 ==========
    std::cout << "========== 测试1：单位矩阵转置 ==========" << std::endl;
    Matrix4x4 identity;
    Matrix4x4 transposedIdentity = !identity;
    PrintMatrix(transposedIdentity, "单位矩阵的转置");
    std::cout << "结果: " << (VerifyEqual(identity, transposedIdentity) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试2：非对称矩阵转置 ==========
    std::cout << "========== 测试2：非对称矩阵转置 ==========" << std::endl;
    Matrix4x4 m = Matrix4x4::Translation(1, 2, 3) * Matrix4x4::RotationY(0.5);
    PrintMatrix(m, "原始矩阵");
    Matrix4x4 mt = !m;
    PrintMatrix(mt, "转置矩阵");
    std::cout << "结果: " << (VerifyEqual(m, !mt) ? "通过（双重转置恢复原矩阵）" : "失败") << std::endl << std::endl;

    // ========== 测试3：转置的行列式 ==========
    std::cout << "========== 测试3：转置的行列式 ==========" << std::endl;
    double detM = Determinant(m);
    double detMT = Determinant(mt);
    std::cout << "原矩阵行列式: " << detM << std::endl;
    std::cout << "转置矩阵行列式: " << detMT << std::endl;
    std::cout << "结果: " << (std::fabs(detM - detMT) < 1e-9 ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试4：转置的逆 = 逆的转置 ==========
    std::cout << "========== 测试4：转置的逆 = 逆的转置 ==========" << std::endl;
    Matrix4x4 invM = ~m;          // 逆
    Matrix4x4 invMT = ~mt;        // 转置的逆
    Matrix4x4 invM_T = !invM;     // 逆的转置
    PrintMatrix(invMT, "转置的逆");
    PrintMatrix(invM_T, "逆的转置");
    std::cout << "结果: " << (VerifyEqual(invMT, invM_T) ? "通过" : "失败") << std::endl << std::endl;

    // ========== 测试5：法线变换验证 ==========
    std::cout << "========== 测试5：法线变换验证 ==========" << std::endl;

    // 定义一个非均匀缩放矩阵
    Matrix4x4 scale = Matrix4x4::Scale(2.0, 1.0, 1.0);
    PrintMatrix(scale, "非均匀缩放矩阵");

    // 定义一个切向量和法向量（法向量垂直于切向量）
    Vector3D tangent(1, -1, 0);
    Vector3D normal(1, 1, 0);   // 与 tangent 点积为 0

    std::cout << "原始切向量: (" << tangent.x << ", " << tangent.y << ", " << tangent.z << ")" << std::endl;
    std::cout << "原始法向量: (" << normal.x << ", " << normal.y << ", " << normal.z << ")" << std::endl;

    // 变换切向量（方向变换，齐次分量补 0）
    Vector3D transformedTangent = scale^(tangent);
    std::cout << "变换后切向量: (" << transformedTangent.x << ", " << transformedTangent.y << ", " << transformedTangent.z << ")" << std::endl;

    // 方法1：直接用法线乘缩放矩阵（错误做法）
    Vector3D wrongNormal = scale^(normal);
    double wrongDot = transformedTangent * wrongNormal;
    std::cout << "错误法线: (" << wrongNormal.x << ", " << wrongNormal.y << ", " << wrongNormal.z << ")" << std::endl;
    std::cout << "与切向量点积: " << wrongDot << " (应该为 0，但实际不为 0)" << std::endl;

    // 方法2：用逆转置矩阵变换法线（正确做法）
    Matrix4x4 normalMatrix = !(~scale);
    Vector3D correctNormal = normalMatrix^(normal);
    double correctDot = transformedTangent * correctNormal;
    std::cout << "正确法线: (" << correctNormal.x << ", " << correctNormal.y << ", " << correctNormal.z << ")" << std::endl;
    std::cout << "与切向量点积: " << correctDot << " (应该为 0)" << std::endl;

    std::cout << "结果: " << (std::fabs(correctDot) < 1e-9 ? "通过（逆转置矩阵正确）" : "失败") << std::endl;

    return 0;
}