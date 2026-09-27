/**
* @file 07-Mesh_test.cpp
* @brief 网格单元测试
* @details
* 验证 Mesh 类的顶点、边、面列表，以及法向量的计算。
*
* @author 半人马座beta星 （AIGC：DeepSeek）
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

// 辅助函数：判断向量是否为单位向量
bool IsUnitVector(const Vector3D& v, double eps = 1e-9) {
    return NearlyEqual(v.getLength(), 1.0, eps);
}

// 辅助函数：判断向量是否垂直于另一个向量
bool IsPerpendicular(const Vector3D& a, const Vector3D& b, double eps = 1e-9) {
    return NearlyEqual(a * b, 0.0, eps);
}

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    Vector3D a(1, 0, 0);
    Vector3D b(0, 1, 0);

    Vector3D c1 = a % b;  // 应该是 (0, 0, 1)
    Vector3D c2 = b % a;  // 应该是 (0, 0, -1)

    std::cout << "a % b = (" << c1.x << ", " << c1.y << ", " << c1.z << ")" << std::endl;
    std::cout << "b % a = (" << c2.x << ", " << c2.y << ", " << c2.z << ")" << std::endl;

    // ========== 测试1：默认构造 ==========
    std::cout << "========== 测试1：默认构造 ==========" << std::endl;
    {
        Scene::Mesh mesh;
        bool verticesEmpty = mesh.vertices.empty();
        bool edgesEmpty = mesh.edges.empty();
        bool surfacesEmpty = mesh.surfaces.empty();

        std::cout << "顶点列表为空: " << (verticesEmpty ? "v" : "x") << std::endl;
        std::cout << "边列表为空: " << (edgesEmpty ? "v" : "x") << std::endl;
        std::cout << "面列表为空: " << (surfacesEmpty ? "v" : "x") << std::endl;
        std::cout << "结果: " << (verticesEmpty && edgesEmpty && surfacesEmpty ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试2：创建立方体 ==========
    std::cout << "========== 测试2：创建立方体 ==========" << std::endl;
    {
        Scene::Mesh mesh = Scene::CreateCubeMesh();

        bool vertexCountOk = (mesh.vertices.size() == 8);
        bool surfaceCountOk = (mesh.surfaces.size() == 12);

        std::cout << "顶点数量: " << mesh.vertices.size() << " (期望 8)" << std::endl;
        std::cout << "面数量: " << mesh.surfaces.size() << " (期望 12)" << std::endl;
        std::cout << "结果: " << (vertexCountOk && surfaceCountOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试3：法向量计算 ==========
    std::cout << "========== 测试3：法向量计算 ==========" << std::endl;
    {
        Scene::Mesh mesh = Scene::CreateCubeMesh();
        mesh.CalculateNormals();

        bool allUnit = true;
        for (const auto& surface : mesh.surfaces) {
            if (!IsUnitVector(surface.normal)) {
                allUnit = false;
                break;
            }
        }

        std::cout << "所有法向量为单位向量: " << (allUnit ? "v" : "x") << std::endl;
        std::cout << "结果: " << (allUnit ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：法向量垂直于面 ==========
    std::cout << "========== 测试4：法向量垂直于面 ==========" << std::endl;
    {
        Scene::Mesh mesh = Scene::CreateCubeMesh();
        mesh.CalculateNormals();

        bool allPerpendicular = true;
        for (const auto& surface : mesh.surfaces) {
            Vector3D v0 = mesh.vertices[surface.verticeA];
            Vector3D v1 = mesh.vertices[surface.verticeB];
            Vector3D v2 = mesh.vertices[surface.verticeC];

            // 法向量应该垂直于面的两条边
            Vector3D edge1 = v1 - v0;
            Vector3D edge2 = v2 - v0;

            if (!IsPerpendicular(surface.normal, edge1) || !IsPerpendicular(surface.normal, edge2)) {
                allPerpendicular = false;
                break;
            }
        }

        std::cout << "所有法向量垂直于面: " << (allPerpendicular ? "v" : "x") << std::endl;
        std::cout << "结果: " << (allPerpendicular ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：法向量指向外部 ==========
    std::cout << "========== 测试5：法向量指向外部 ==========" << std::endl;
    {
        Scene::Mesh mesh = Scene::CreateCubeMesh();
        mesh.CalculateNormals();

        // 对于中心在原点的立方体，法向量应该指向远离原点的方向
        int Outward = 12;
        for (const auto& surface : mesh.surfaces) {
            // 计算面中心
            Vector3D v0 = mesh.vertices[surface.verticeA];
            Vector3D v1 = mesh.vertices[surface.verticeB];
            Vector3D v2 = mesh.vertices[surface.verticeC];
            Vector3D center = (v0 + v1 + v2) * (1.0 / 3.0);

            // 法向量与面中心的点积应该为正（指向外部）
            if (surface.normal * center <= 0) {
                Outward --;
                std::cout << "法向量没有指向外部： "<< surface.verticeA << ", " << surface.verticeB << ", " << surface.verticeC << std::endl;
            }
            else {
                std::cout << "法向量指向外部" << surface.verticeA << ", " << surface.verticeB << ", " << surface.verticeC << std::endl;
            }

        }

        std::cout << "法向量指向外部数量: " << (Outward) << std::endl;
        std::cout << "结果: " << (Outward >= 12 ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：边结构 ==========
    std::cout << "========== 测试6：边结构 ==========" << std::endl;
    {
        Scene::Mesh mesh;
        mesh.vertices = {
            {0, 0, 0}, {1, 0, 0}, {0, 1, 0}
        };
        mesh.edges = {
            {0, 1, Color::Red()},
            {1, 2, Color::Green()},
            {2, 0, Color::Blue()}
        };

        bool edgeCountOk = (mesh.edges.size() == 3);
        bool edge0Ok = (mesh.edges[0].verticeStart == 0 && mesh.edges[0].verticeEnd == 1);
        bool edge1Ok = (mesh.edges[1].verticeStart == 1 && mesh.edges[1].verticeEnd == 2);
        bool edge2Ok = (mesh.edges[2].verticeStart == 2 && mesh.edges[2].verticeEnd == 0);

        // 测试数组访问
        bool arrayAccessOk = (mesh.edges[0].vertices[0] == 0 && mesh.edges[0].vertices[1] == 1);

        std::cout << "边数量: " << mesh.edges.size() << " (期望 3)" << std::endl;
        std::cout << "边0索引正确: " << (edge0Ok ? "v" : "x") << std::endl;
        std::cout << "边1索引正确: " << (edge1Ok ? "v" : "x") << std::endl;
        std::cout << "边2索引正确: " << (edge2Ok ? "v" : "x") << std::endl;
        std::cout << "数组访问正确: " << (arrayAccessOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (edgeCountOk && edge0Ok && edge1Ok && edge2Ok && arrayAccessOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    return 0;
}