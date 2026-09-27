/**
* @file FernPack/game_object/RenderObject.h
* @brief 可渲染物体
* @details
* 继承自 Object，额外持有一个 Mesh 指针和颜色。
* 同一个 Mesh 可以被多个 RenderObject 共享。
*
* @author 半人马座beta星（AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#pragma once

namespace Fern::Scene {

    /**
    * @brief 可渲染物体
    * @details
    * 继承自 Object，额外持有一个 Mesh 指针和颜色。
    * 同一个 Mesh 可以被多个 RenderObject 共享。
    */
    class RenderObject : public Object {
    protected:
        Mesh* mesh;         ///< 指向网格数据（可共享）
        Color color;        ///< 物体的颜色
        bool visible;       ///< 是否可见

    public:
        /**
        * @brief 默认构造函数
        */
        RenderObject()
            : Object(),
            mesh(nullptr),
            color(Color::White()),
            visible(true)
        {
        }

        /**
        * @brief 带参构造函数
        * @param Mesh* m 指向网格的指针
        * @param Color c 物体的颜色
        */
        RenderObject(Mesh* m, Color c)
            : Object(),
            mesh(m),
            color(c),
            visible(true)
        {
        }

        /**
        * @brief 虚析构函数
        */
        virtual ~RenderObject() override = default;

        // ========== 网格访问 ==========

        void SetMesh(Mesh* m) { mesh = m; }
        Mesh* GetMesh() const { return mesh; }

        // ========== 颜色访问 ==========

        void SetColor(Color c) { color = c; }
        Color GetColor() const { return color; }

        // ========== 可见性 ==========

        void SetVisible(bool v) { visible = v; }
        bool IsVisible() const { return visible; }
    };

}