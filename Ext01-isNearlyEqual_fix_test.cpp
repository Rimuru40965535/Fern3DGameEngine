/**
* @file 12-isNearlyEqual_fix_test.cpp
* @brief isNearlyEqual 修复后的回归测试
* @details
* 验证 isNearlyEqual 修复后，受影响的三个函数是否正常工作：
* - Vector3D::operator==
* - Matrix4x4::operator~
* - Matrix4x4::operator*
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/9/30
*
*/
#include "Fern3DGameEngine.h"

#include <cmath>

using namespace Fern;

// ========== 测试1：isNearlyEqual 真值表 ==========

void Test_isNearlyEqual() {
    std::cout << "========== 测试1：isNearlyEqual 真值表 ==========" << std::endl;

    // 相等
    bool t1 = Math::isNearlyEqual(1.0, 1.0);
    bool t2 = Math::isNearlyEqual(0.0, 0.0);
    bool t3 = Math::isNearlyEqual(-1.0, -1.0);

    // 接近
    bool t4 = Math::isNearlyEqual(1.0, 1.0000001);
    bool t5 = Math::isNearlyEqual(0.0, 1e-7);
    bool t6 = Math::isNearlyEqual(-1.0, -1.0000001);

    // 远离
    bool f1 = Math::isNearlyEqual(1.0, 2.0);
    bool f2 = Math::isNearlyEqual(0.0, 1.0);
    bool f3 = Math::isNearlyEqual(-1.0, 1.0);

    std::cout << "相等测试:" << std::endl;
    std::cout << "  isNearlyEqual(1.0, 1.0) = " << (t1 ? "true" : "false") << " (期望 true)" << std::endl;
    std::cout << "  isNearlyEqual(0.0, 0.0) = " << (t2 ? "true" : "false") << " (期望 true)" << std::endl;
    std::cout << "  isNearlyEqual(-1.0, -1.0) = " << (t3 ? "true" : "false") << " (期望 true)" << std::endl;

    std::cout << "接近测试:" << std::endl;
    std::cout << "  isNearlyEqual(1.0, 1.0000001) = " << (t4 ? "true" : "false") << " (期望 true)" << std::endl;
    std::cout << "  isNearlyEqual(0.0, 1e-7) = " << (t5 ? "true" : "false") << " (期望 true)" << std::endl;
    std::cout << "  isNearlyEqual(-1.0, -1.0000001) = " << (t6 ? "true" : "false") << " (期望 true)" << std::endl;

    std::cout << "远离测试:" << std::endl;
    std::cout << "  isNearlyEqual(1.0, 2.0) = " << (f1 ? "true" : "false") << " (期望 false)" << std::endl;
    std::cout << "  isNearlyEqual(0.0, 1.0) = " << (f2 ? "true" : "false") << " (期望 false)" << std::endl;
    std::cout << "  isNearlyEqual(-1.0, 1.0) = " << (f3 ? "true" : "false") << " (期望 false)" << std::endl;

    bool allOk = t1 && t2 && t3 && t4 && t5 && t6 && !f1 && !f2 && !f3;
    std::cout << "结果: " << (allOk ? "v 通过" : "x 失败") << std::endl << std::endl;
}

// ========== 测试2：Vector3D::operator== ==========

void Test_Vector3D_Equal() {
    std::cout << "========== 测试2：Vector3D::operator== ==========" << std::endl;

    Vector3D a(1, 2, 3);
    Vector3D b(1, 2, 3);
    Vector3D c(1, 2, 3.0000001);
    Vector3D d(4, 5, 6);


    std::cout << "a == b (相同向量) = " << ((a == b) ? "true" : "false") << " (期望 true)" << std::endl;
    std::cout << "a == c (接近向量) = " << ((a == c) ? "true" : "false") << " (期望 true)" << std::endl;
    std::cout << "a == d (不同向量) = " << ((a == d) ? "true" : "false") << " (期望 false)" << std::endl;

    bool allOk = (a == b) && (a == c) && !(a == d);
    std::cout << "结果: " << (allOk ? "v 通过" : "x 失败") << std::endl << std::endl;
}

// ========== 测试3：Matrix4x4::operator~ ==========

void Test_Matrix_Inverse() {
    std::cout << "========== 测试3：Matrix4x4::operator~ ==========" << std::endl;

    // 复合变换：平移 × 旋转 × 缩放
    Matrix4x4 m = Matrix4x4::Translation(1, 2, 3)
        * Matrix4x4::RotationY(0.5)
        * Matrix4x4::Scale(2.0, 1.5, 0.8);

    Matrix4x4 inv = ~m;
    Matrix4x4 result = m * inv;

    // 验证 result 接近单位矩阵
    bool identityOk = true;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (!Math::isNearlyEqual(result.values[i][j], expected, 1e-6)) {
                identityOk = false;
                std::cout << "  result[" << i << "][" << j << "] = " << result.values[i][j]
                    << " (期望 " << expected << ")" << std::endl;
            }
        }
    }

    std::cout << "M * M^-1 接近单位矩阵: " << (identityOk ? "true" : "false") << std::endl;
    std::cout << "结果: " << (identityOk ? "v 通过" : "x 失败") << std::endl << std::endl;
}

// ========== 测试4：Matrix4x4::operator* ==========

void Test_Matrix_PointTransform() {
    std::cout << "========== 测试4：Matrix4x4::operator* ==========" << std::endl;

    // 纯平移
    Matrix4x4 trans = Matrix4x4::Translation(1, 2, 3);
    Vector3D p(0, 0, 0);
    Vector3D p_trans = trans * p;

    bool transOk = Math::isNearlyEqual(p_trans.x, 1.0, 1e-6)
        && Math::isNearlyEqual(p_trans.y, 2.0, 1e-6)
        && Math::isNearlyEqual(p_trans.z, 3.0, 1e-6);

    std::cout << "平移变换: (" << p_trans.x << ", " << p_trans.y << ", " << p_trans.z << ")"
        << " (期望 1, 2, 3)" << std::endl;

    // 纯缩放
    Matrix4x4 scale = Matrix4x4::Scale(2.0, 3.0, 4.0);
    Vector3D p2(1, 1, 1);
    Vector3D p_scale = scale * p2;

    bool scaleOk = Math::isNearlyEqual(p_scale.x, 2.0, 1e-6)
        && Math::isNearlyEqual(p_scale.y, 3.0, 1e-6)
        && Math::isNearlyEqual(p_scale.z, 4.0, 1e-6);

    std::cout << "缩放变换: (" << p_scale.x << ", " << p_scale.y << ", " << p_scale.z << ")"
        << " (期望 2, 3, 4)" << std::endl;

    // 旋转
    Matrix4x4 rot = Matrix4x4::RotationY(3.14159 / 2.0);
    Vector3D p3(1, 0, 0);
    Vector3D p_rot = rot * p3;

    bool rotOk = Math::isNearlyEqual(p_rot.x, 0.0, 1e-5)
        && Math::isNearlyEqual(p_rot.y, 0.0, 1e-5)
        && Math::isNearlyEqual(p_rot.z, -1.0, 1e-5);

    std::cout << "旋转变换: (" << p_rot.x << ", " << p_rot.y << ", " << p_rot.z << ")"
        << " (期望 0, 0, -1)" << std::endl;

    bool allOk = transOk && scaleOk && rotOk;
    std::cout << "结果: " << (allOk ? "v 通过" : "x 失败") << std::endl << std::endl;
}

// ========== 主函数 ==========

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    Test_isNearlyEqual();
    Test_Vector3D_Equal();
    Test_Matrix_Inverse();
    Test_Matrix_PointTransform();

    return 0;
}