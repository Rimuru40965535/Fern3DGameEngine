/**
* @file 10-Camera_test.cpp
* @brief 相机单元测试
* @details
* 验证 Camera 类的构造、投影参数访问、观察矩阵、投影矩阵，以及多态行为。
*
* @author 半人马座beta星 （AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#include "Fern3DGameEngine.h"

#include <cmath>

using namespace Fern;
using namespace Fern::Scene;

// 辅助函数：判断两个 double 是否接近
bool NearlyEqual(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

// 辅助函数：判断两个 Vector3D 是否接近
bool NearlyEqual(const Vector3D& a, const Vector3D& b, double eps = 1e-6) {
    return NearlyEqual(a.x, b.x, eps)
        && NearlyEqual(a.y, b.y, eps)
        && NearlyEqual(a.z, b.z, eps);
}

// 辅助函数：判断两个 Matrix4x4 是否接近
bool NearlyEqual(const Matrix4x4& a, const Matrix4x4& b, double eps = 1e-6) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (!NearlyEqual(a.values[i][j], b.values[i][j], eps)) {
                return false;
            }
        }
    }
    return true;
}

// 测试用的派生类
class TestCamera : public Scene::Camera {
public:
    int updateCount = 0;
    bool destroyed = false;

    void Update() override {
        updateCount++;
    }

    ~TestCamera() {
        destroyed = true;
    }
};

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // ========== 测试1：默认构造 ==========
    std::cout << "========== 测试1：默认构造 ==========" << std::endl;
    {
        Scene::Camera cam;

        bool fovOk = NearlyEqual(cam.GetFov(), Math::PI / 3.0);
        bool nearOk = NearlyEqual(cam.GetNearPlane(), 0.1);
        bool farOk = NearlyEqual(cam.GetFarPlane(), 100.0);

        std::cout << "fov = " << cam.GetFov() << " (期望 " << Math::PI / 3.0 << ")" << std::endl;
        std::cout << "__near = " << cam.GetNearPlane() << " (期望 0.1)" << std::endl;
        std::cout << "__far = " << cam.GetFarPlane() << " (期望 100)" << std::endl;
        std::cout << "结果: " << (fovOk && nearOk && farOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试2：带参构造 ==========
    std::cout << "========== 测试2：带参构造 ==========" << std::endl;
    {
        Scene::Camera cam(0.8, 0.5, 200.0);

        bool fovOk = NearlyEqual(cam.GetFov(), 0.8);
        bool nearOk = NearlyEqual(cam.GetNearPlane(), 0.5);
        bool farOk = NearlyEqual(cam.GetFarPlane(), 200.0);

        std::cout << "fov = " << cam.GetFov() << " (期望 0.8)" << std::endl;
        std::cout << "__near = " << cam.GetNearPlane() << " (期望 0.5)" << std::endl;
        std::cout << "__far = " << cam.GetFarPlane() << " (期望 200)" << std::endl;
        std::cout << "结果: " << (fovOk && nearOk && farOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试3：继承自 Object ==========
    std::cout << "========== 测试3：继承自 Object ==========" << std::endl;
    {
        Scene::Camera cam;

        cam.SetPosition(Vector3D(1, 2, 3));
        cam.SetRotation(Vector3D(0, 0.5, 0));
        cam.SetScale(Vector3D(1, 1, 1));

        bool posOk = NearlyEqual(cam.GetPosition(), Vector3D(1, 2, 3));
        bool rotOk = NearlyEqual(cam.GetRotation(), Vector3D(0, 0.5, 0));
        bool scaleOk = NearlyEqual(cam.GetScale(), Vector3D(1, 1, 1));

        Matrix4x4 expected = Matrix4x4::Translation(1, 2, 3)
            * Matrix4x4::Rotation(Vector3D(0, 0.5, 0))
            * Matrix4x4::Scale(1, 1, 1);
        bool modelOk = NearlyEqual(cam.GetModelMatrix(), expected);

        std::cout << "位置正确: " << (posOk ? "v" : "x") << std::endl;
        std::cout << "旋转正确: " << (rotOk ? "v" : "x") << std::endl;
        std::cout << "缩放正确: " << (scaleOk ? "v" : "x") << std::endl;
        std::cout << "模型矩阵正确: " << (modelOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (posOk && rotOk && scaleOk && modelOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：投影参数访问 ==========
    std::cout << "========== 测试4：投影参数访问 ==========" << std::endl;
    {
        Scene::Camera cam;

        cam.SetFov(1.2);
        cam.SetNearPlane(0.2);
        cam.SetFarPlane(500.0);

        bool fovOk = NearlyEqual(cam.GetFov(), 1.2);
        bool nearOk = NearlyEqual(cam.GetNearPlane(), 0.2);
        bool farOk = NearlyEqual(cam.GetFarPlane(), 500.0);

        std::cout << "fov = " << cam.GetFov() << " (期望 1.2)" << std::endl;
        std::cout << "__near = " << cam.GetNearPlane() << " (期望 0.2)" << std::endl;
        std::cout << "__far = " << cam.GetFarPlane() << " (期望 500)" << std::endl;
        std::cout << "结果: " << (fovOk && nearOk && farOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：观察矩阵（相机在原点） ==========
    std::cout << "========== 测试5：观察矩阵（相机在原点） ==========" << std::endl;
    {
        Scene::Camera cam;

        Matrix4x4 view = ~cam.GetModelMatrix();
        Matrix4x4 identity = Matrix4x4::Identity();

        bool viewOk = NearlyEqual(view, identity);

        std::cout << "观察矩阵为单位矩阵: " << (viewOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (viewOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：观察矩阵（相机平移） ==========
    std::cout << "========== 测试6：观察矩阵（相机平移） ==========" << std::endl;
    {
        Scene::Camera cam;
        cam.SetPosition(Vector3D(0, 0, 5));

        Matrix4x4 view = ~cam.GetModelMatrix();
        Matrix4x4 expected = Matrix4x4::Translation(0, 0, -5);

        bool viewOk = NearlyEqual(view, expected);

        std::cout << "观察矩阵为逆平移: " << (viewOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (viewOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试7：投影矩阵 ==========
    std::cout << "========== 测试7：投影矩阵 ==========" << std::endl;
    {
        Window window(800, 600, "Test");
        Scene::Camera cam;

        Matrix4x4 proj = cam.GetProjMatrix(window);

        // 验证投影矩阵的 z 映射
        // 近平面 z = 0.1 映射到 z_ndc = 0
        // 远平面 z = 100 映射到 z_ndc = 1
        double __near = cam.GetNearPlane();
        double __far = cam.GetFarPlane();

        double zNear = proj.values[2][2] * __near + proj.values[2][3];
        double wNear = proj.values[3][2] * __near;
        double zndcNear = zNear / wNear;

        double zFar = proj.values[2][2] * __far + proj.values[2][3];
        double wFar = proj.values[3][2] * __far;
        double zndcFar = zFar / wFar;

        bool nearOk = NearlyEqual(zndcNear, 0.0);
        bool farOk = NearlyEqual(zndcFar, 1.0);

        std::cout << "近平面 z_ndc: " << zndcNear << " (期望 0)" << std::endl;
        std::cout << "远平面 z_ndc: " << zndcFar << " (期望 1)" << std::endl;
        std::cout << "结果: " << (nearOk && farOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试8：多态 ==========
    std::cout << "========== 测试8：多态 ==========" << std::endl;
    {
        Object* obj = new TestCamera();

        obj->Update();
        obj->Update();
        obj->Update();

        TestCamera* derived = dynamic_cast<TestCamera*>(obj);
        bool polyOk = (derived != nullptr && derived->updateCount == 3);

        std::cout << "Update 调用次数: " << (derived ? derived->updateCount : -1) << " (期望 3)" << std::endl;
        std::cout << "结果: " << (polyOk ? "v 通过" : "x 失败") << std::endl << std::endl;

        delete obj;
    }

    // ========== 测试9：虚析构 ==========
    std::cout << "========== 测试9：虚析构 ==========" << std::endl;
    {
        TestCamera* derived = new TestCamera();
        Object* obj = derived;

        delete obj;

        std::cout << "派生类析构函数被调用: " << (derived->destroyed ? "v" : "x") << std::endl;
        std::cout << "结果: " << (derived->destroyed ? "v 通过" : "x 失败（内存泄漏）") << std::endl << std::endl;
    }

    return 0;
}