/**
* @file 06-Object-test.cpp
* @brief 物体基类单元测试
* @details
* 
* @author 半人马座beta星 （AIGC：DeepSeek）
* @date 2026-9-28
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

// 验证向量是否接近相等
bool VerifyEqual(const Vector3D& a, const Vector3D& b, double eps = 1e-9) {
    return std::fabs(a.x - b.x) < eps
        && std::fabs(a.y - b.y) < eps
        && std::fabs(a.z - b.z) < eps;
}

// 测试用的派生类
class TestRenderObject : public Fern::Scene::Object {
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
        Fern::Scene::Object obj;

        std::cout << "位置: (" << obj.GetPosition().x << ", " << obj.GetPosition().y << ", " << obj.GetPosition().z << ")" << std::endl;
        std::cout << "旋转: (" << obj.GetRotation().x << ", " << obj.GetRotation().y << ", " << obj.GetRotation().z << ")" << std::endl;
        std::cout << "缩放: (" << obj.GetScale().x << ", " << obj.GetScale().y << ", " << obj.GetScale().z << ")" << std::endl;

        bool posOk = VerifyEqual(obj.GetPosition(), Vector3D(0, 0, 0));
        bool rotOk = VerifyEqual(obj.GetRotation(), Vector3D(0, 0, 0));
        bool scaleOk = VerifyEqual(obj.GetScale(), Vector3D(1, 1, 1));
        bool modelOk = VerifyEqual(obj.GetModelMatrix(), Matrix4x4::Identity());

        std::cout << "结果: " << (posOk && rotOk && scaleOk && modelOk ? "通过" : "失败") << std::endl << std::endl;
    }

    // ========== 测试2：设置位置 ==========
    std::cout << "========== 测试2：设置位置 ==========" << std::endl;
    {
        Fern::Scene::Object obj;
        obj.SetPosition(Vector3D(1, 2, 3));

        Matrix4x4 expected = Matrix4x4::Translation(1, 2, 3);
        PrintMatrix(obj.GetModelMatrix(), "模型矩阵");
        std::cout << "结果: " << (VerifyEqual(obj.GetModelMatrix(), expected) ? "通过" : "失败") << std::endl << std::endl;
    }

    // ========== 测试3：设置旋转 ==========
    std::cout << "========== 测试3：设置旋转 ==========" << std::endl;
    {
        Fern::Scene::Object obj;
        obj.SetRotation(Vector3D(0, 0.5, 0));

        Matrix4x4 expected = Matrix4x4::Rotation(Vector3D(0, 0.5, 0));
        std::cout << "结果: " << (VerifyEqual(obj.GetModelMatrix(), expected) ? "通过" : "失败") << std::endl << std::endl;
    }

    // ========== 测试4：设置缩放 ==========
    std::cout << "========== 测试4：设置缩放 ==========" << std::endl;
    {
        Fern::Scene::Object obj;
        obj.SetScale(Vector3D(2, 3, 4));

        Matrix4x4 expected = Matrix4x4::Scale(2, 3, 4);
        std::cout << "结果: " << (VerifyEqual(obj.GetModelMatrix(), expected) ? "通过" : "失败") << std::endl << std::endl;
    }

    // ========== 测试5：组合变换 ==========
    std::cout << "========== 测试5：组合变换 ==========" << std::endl;
    {
        Fern::Scene::Object obj;
        obj.SetPosition(Vector3D(1, 2, 3));
        obj.SetRotation(Vector3D(0, 0.5, 0));
        obj.SetScale(Vector3D(2, 2, 2));

        // 期望顺序：T × R × S
        Matrix4x4 expected = Matrix4x4::Translation(1, 2, 3)
            * Matrix4x4::Rotation(Vector3D(0, 0.5, 0))
            * Matrix4x4::Scale(2, 2, 2);

        PrintMatrix(obj.GetModelMatrix(), "组合变换模型矩阵");
        std::cout << "结果: " << (VerifyEqual(obj.GetModelMatrix(), expected) ? "通过" : "失败") << std::endl << std::endl;
    }

    // ========== 测试6：脏标记 ==========
    std::cout << "========== 测试6：脏标记 ==========" << std::endl;
    {
        Fern::Scene::Object obj;
        obj.SetPosition(Vector3D(1, 2, 3));

        // 第一次获取模型矩阵，会触发计算
        Matrix4x4 m1 = obj.GetModelMatrix();

        // 第二次获取，应该返回同一个矩阵（没有重新计算）
        Matrix4x4 m2 = obj.GetModelMatrix();

        bool same = VerifyEqual(m1, m2);

        // 修改位置，再次获取
        obj.SetPosition(Vector3D(4, 5, 6));
        Matrix4x4 m3 = obj.GetModelMatrix();

        bool changed = !VerifyEqual(m1, m3);

        std::cout << "未修改时矩阵不变: " << (same ? "v" : "x") << std::endl;
        std::cout << "修改后矩阵改变: " << (changed ? "v" : "x") << std::endl;
        std::cout << "结果: " << (same && changed ? "通过" : "失败") << std::endl << std::endl;
    }

    // ========== 测试7：多态 ==========
    std::cout << "========== 测试7：多态 ==========" << std::endl;
    {
        Fern::Scene::Object* obj = new TestRenderObject();

        obj->Update();
        obj->Update();
        obj->Update();

        TestRenderObject* derived = dynamic_cast<TestRenderObject*>(obj);
        bool polyOk = (derived != nullptr && derived->updateCount == 3);

        std::cout << "Update 调用次数: " << (derived ? derived->updateCount : -1) << " (期望 3)" << std::endl;
        std::cout << "结果: " << (polyOk ? "通过" : "失败") << std::endl << std::endl;

        delete obj;
    }

    // ========== 测试8：虚析构 ==========
    std::cout << "========== 测试8：虚析构 ==========" << std::endl;
    {
        TestRenderObject* derived = new TestRenderObject();
        Fern::Scene::Object* obj = derived;

        delete obj;  // 通过基类指针删除

        std::cout << "派生类析构函数被调用: " << (derived->destroyed ? "" : "") << std::endl;
        std::cout << "结果: " << (derived->destroyed ? "通过" : "失败（内存泄漏）") << std::endl << std::endl;
    }


    return 0;
}