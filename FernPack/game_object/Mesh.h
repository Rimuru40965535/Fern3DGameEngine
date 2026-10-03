/**
* @file ./FernPack/game_object/Mesh.h
* @brief 网格定义
* @details
* 定义纯几何数据的容器，包含顶点、边、面。
* 面中预存法向量，便于背面剔除和光照计算。
*
* @author 半人马座beta星（AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#pragma once

namespace Fern::Scene {

    /**
    * @brief 棱结构体
    * @details
    * 存储一条边的两个端点索引和颜色。
    */
    struct Edge {
        union {
            struct {
                int verticeStart;       ///< 起始点索引
                int verticeEnd;         ///< 终止点索引
            };
            int vertices[2];            ///< 数组形式的属性
        };
        Color color;                    ///< 边的颜色

        Edge(int start, int end, Color c)
            : verticeStart(start), verticeEnd(end), color(c) {
        }
    };

    /**
    * @brief 面结构体
    * @details
    * 存储一个三角形的三个顶点索引、法向量和颜色。
    * 法向量在创建时计算并预存，便于背面剔除和光照计算。
    */
    struct Surface {
        union {
            struct {
                int verticeA;           ///< 顶点A索引
                int verticeB;           ///< 顶点B索引
                int verticeC;           ///< 顶点C索引
            };
            int vertices[3];            ///< 数组形式的属性
        };
        Vector3D normal;                ///< 法向量
        Color color;                    ///< 面的颜色

        Surface(int A, int B, int C, Color c)
            : verticeA(A), verticeB(B), verticeC(C), color(c) {
        }

        Surface(int A, int B, int C, Vector3D n, Color c)
            : verticeA(A), verticeB(B), verticeC(C), normal(n), color(c) {
        }
    };

    /**
    * @brief 网格类
    * @details
    * 纯几何数据的容器，包含顶点、边、面。
    * 同一个网格可以被多个物体实例共享，节省内存。
    */
    class Mesh {
    public:
        std::vector<Vector3D> vertices;   ///< 顶点列表
        std::vector<Edge> edges;          ///< 边列表
        std::vector<Surface> surfaces;    ///< 面列表

        /**
        * @brief 默认构造函数
        */
        Mesh() = default;

        /**
        * @brief 计算所有面的法向量
        * @details
        * 根据每个面的三个顶点，用叉积计算法向量。
        * 法向量方向遵循逆时针绕序约定。
        */
        void CalculateNormals() {
            for (auto& surface : surfaces) {
                Vector3D v0 = vertices[surface.verticeA];
                Vector3D v1 = vertices[surface.verticeB];
                Vector3D v2 = vertices[surface.verticeC];

                // 叉积计算法向量
                surface.normal = (v1 - v0) % (v2 - v0);
                surface.normal.makeLengthUnit();
            }
        }
    };

    /// <summary>
    /// 创建立方体网格。示例函数。
    /// </summary>
    /// <returns>一个标准的立方体网格</returns>
    Mesh CreateCubeMesh() {
        Mesh mesh;

        // 8 个顶点
        mesh.vertices = {
            {-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1},
            {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1}
        };

        mesh.edges = {
            {0, 1, Color::White()},
            {0, 3, Color::White()},
            {0, 4, Color::White()},
            {1, 2, Color::White()},
            {1, 5, Color::White()},
            {2, 3, Color::White()},
            {2, 6, Color::White()},
            {3, 7, Color::White()},
            {4, 5, Color::White()},
            {4, 7, Color::White()},
            {5, 6, Color::White()},
            {6, 7, Color::White()}
        };

        // 12 个三角形
        mesh.surfaces = {
            // 前面 (Z = -1)
            {0, 2, 1, Color::Red()}, {0, 3, 2, Color::Red()},
            // 后面 (Z = +1)
            {4, 5, 6, Color::Green()}, {4, 6, 7, Color::Green()},
            // 左面 (X = -1)
            {0, 7, 3, Color::Magenta()}, {0, 4, 7, Color::Magenta()},
            // 右面 (X = +1)
            {1, 6, 5, Color::Cyan()}, {1, 2, 6, Color::Cyan()},
            // 底面 (Y = -1)
            {0, 5, 4, Color::Blue()}, {0, 1, 5, Color::Blue()},
            // 顶面 (Y = +1)
            {2, 7, 6, Color::Yellow()}, {2, 3, 7, Color::Yellow()}
        };

        // 计算法向量
        mesh.CalculateNormals();

        return mesh;
    }
}