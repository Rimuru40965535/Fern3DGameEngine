/**
* @file 08-RenderObject_test.cpp
* @brief 可渲染物体单元测试
* @details
* 验证 RenderObject 类的构造、网格访问、颜色访问、可见性，以及多态行为。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#include "Fern3DGameEngine.h"


using namespace Fern;
using namespace Fern::Scene;

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

// 辅助函数：判断两个 Matrix4x4 是否接近
bool NearlyEqual(const Matrix4x4& a, const Matrix4x4& b, double eps = 1e-9) {
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
class TestRenderObject : public RenderObject {
public:
    int updateCount = 0;
    bool destroyed = false;

    void Update() override {
        updateCount++;
    }

    ~TestRenderObject() {
        destroyed = true;
    }
};

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // ========== 测试1：默认构造 ==========
    std::cout << "========== 测试1：默认构造 ==========" << std::endl;
    {
        RenderObject obj;

        bool meshNull = (obj.GetMesh() == nullptr);
        bool colorWhite = (obj.GetColor().getARGB() == Color::White().getARGB());
        bool visibleTrue = (obj.IsVisible() == true);

        std::cout << "网格为空: " << (meshNull ? "v" : "x") << std::endl;
        std::cout << "颜色为白色: " << (colorWhite ? "v" : "x") << std::endl;
        std::cout << "可见为真: " << (visibleTrue ? "v" : "x") << std::endl;
        std::cout << "结果: " << (meshNull && colorWhite && visibleTrue ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试2：带参构造 ==========
    std::cout << "========== 测试2：带参构造 ==========" << std::endl;
    {
        Mesh mesh;
        RenderObject obj(&mesh, Color::Red());

        bool meshOk = (obj.GetMesh() == &mesh);
        bool colorOk = (obj.GetColor().getARGB() == Color::Red().getARGB());

        std::cout << "网格指针正确: " << (meshOk ? "v" : "x") << std::endl;
        std::cout << "颜色为红色: " << (colorOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (meshOk && colorOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试3：继承自 Object ==========
    std::cout << "========== 测试3：继承自 Object ==========" << std::endl;
    {
        RenderObject obj;

        obj.SetPosition(Vector3D(1, 2, 3));
        obj.SetRotation(Vector3D(0, 0.5, 0));
        obj.SetScale(Vector3D(2, 2, 2));

        bool posOk = NearlyEqual(obj.GetPosition(), Vector3D(1, 2, 3));
        bool rotOk = NearlyEqual(obj.GetRotation(), Vector3D(0, 0.5, 0));
        bool scaleOk = NearlyEqual(obj.GetScale(), Vector3D(2, 2, 2));

        Matrix4x4 expected = Matrix4x4::Translation(1, 2, 3)
            * Matrix4x4::Rotation(Vector3D(0, 0.5, 0))
            * Matrix4x4::Scale(2, 2, 2);
        bool modelOk = NearlyEqual(obj.GetModelMatrix(), expected);

        std::cout << "位置正确: " << (posOk ? "v" : "x") << std::endl;
        std::cout << "旋转正确: " << (rotOk ? "v" : "x") << std::endl;
        std::cout << "缩放正确: " << (scaleOk ? "v" : "x") << std::endl;
        std::cout << "模型矩阵正确: " << (modelOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (posOk && rotOk && scaleOk && modelOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：网格访问 ==========
    std::cout << "========== 测试4：网格访问 ==========" << std::endl;
    {
        Mesh mesh1, mesh2;
        RenderObject obj;

        obj.SetMesh(&mesh1);
        bool set1Ok = (obj.GetMesh() == &mesh1);

        obj.SetMesh(&mesh2);
        bool set2Ok = (obj.GetMesh() == &mesh2);

        std::cout << "设置网格1: " << (set1Ok ? "v" : "x") << std::endl;
        std::cout << "设置网格2: " << (set2Ok ? "v" : "x") << std::endl;
        std::cout << "结果: " << (set1Ok && set2Ok ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：颜色访问 ==========
    std::cout << "========== 测试5：颜色访问 ==========" << std::endl;
    {
        RenderObject obj;

        obj.SetColor(Color::Red());
        bool set1Ok = (obj.GetColor().getARGB() == Color::Red().getARGB());

        obj.SetColor(Color::Blue());
        bool set2Ok = (obj.GetColor().getARGB() == Color::Blue().getARGB());

        std::cout << "设置红色: " << (set1Ok ? "v" : "x") << std::endl;
        std::cout << "设置蓝色: " << (set2Ok ? "v" : "x") << std::endl;
        std::cout << "结果: " << (set1Ok && set2Ok ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：可见性 ==========
    std::cout << "========== 测试6：可见性 ==========" << std::endl;
    {
        RenderObject obj;

        bool defaultVisible = (obj.IsVisible() == true);

        obj.SetVisible(false);
        bool setFalseOk = (obj.IsVisible() == false);

        obj.SetVisible(true);
        bool setTrueOk = (obj.IsVisible() == true);

        std::cout << "默认为真: " << (defaultVisible ? "v" : "x") << std::endl;
        std::cout << "设置为假: " << (setFalseOk ? "v" : "x") << std::endl;
        std::cout << "设置为真: " << (setTrueOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (defaultVisible && setFalseOk && setTrueOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试7：多态 ==========
    std::cout << "========== 测试7：多态 ==========" << std::endl;
    {
        Object* obj = new TestRenderObject();

        obj->Update();
        obj->Update();

        TestRenderObject* derived = dynamic_cast<TestRenderObject*>(obj);
        bool polyOk = (derived != nullptr && derived->updateCount == 2);

        std::cout << "Update 调用次数: " << (derived ? derived->updateCount : -1) << " (期望 2)" << std::endl;
        std::cout << "结果: " << (polyOk ? "v 通过" : "x 失败") << std::endl << std::endl;

        delete obj;
    }

    // ========== 测试8：虚析构 ==========
    std::cout << "========== 测试8：虚析构 ==========" << std::endl;
    {
        TestRenderObject* derived = new TestRenderObject();
        Object* obj = derived;

        delete obj;

        std::cout << "派生类析构函数被调用: " << (derived->destroyed ? "v" : "x") << std::endl;
        std::cout << "结果: " << (derived->destroyed ? "v 通过" : "x 失败（内存泄漏）") << std::endl << std::endl;
    }

    return 0;
}