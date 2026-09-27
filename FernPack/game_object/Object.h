/**
* @file ./FernPack/game_object/Object.h
* @brief 物体基类
* @details 
* 在这个文件里我想把物体基类单独放好。
* 其他文件放的宏定义放到后面就可以直接继承这一个类。
*/

#pragma once

namespace Fern::Scene {
    
    class Object {
    protected:
        Vector3D position;      ///< 位置
        Vector3D rotation;      ///< 旋转（欧拉角，弧度制）
        Vector3D scale;         ///< 缩放
        
        Matrix4x4 modelMatrix;  ///< 模型矩阵（模型空间 → 世界空间）
        bool dirty;             ///< 脏标记，是否需要重新计算模型矩阵

    public:
        /**
         * @brief 默认构造函数
         * @details 初始化为原点、无旋转、单位缩放。
         */
        Object()
            : position(0, 0, 0)
            , rotation(0, 0, 0)
            , scale(1, 1, 1)
            , modelMatrix(Matrix4x4::Identity())
            , dirty(false)
        {
        }

        /**
         * @brief 虚析构函数
         */
        virtual ~Object() = default;

        // ========== 变换属性访问 ==========

        void SetPosition(const Vector3D& pos) {
            position = pos;
            dirty = true;
        }

        void SetRotation(const Vector3D& rot) {
            rotation = rot;
            dirty = true;
        }

        void SetScale(const Vector3D& s) {
            scale = s;
            dirty = true;
        }

        const Vector3D& GetPosition() const { return position; }
        const Vector3D& GetRotation() const { return rotation; }
        const Vector3D& GetScale() const { return scale; }

        // ========== 模型矩阵 ==========

        /**
         * @brief 获取模型矩阵
         * @details 如果脏标记为 true，则重新计算模型矩阵。
         * @return const Matrix4x4& 模型矩阵
         */
        const Matrix4x4& GetModelMatrix() {
            if (dirty) {
                modelMatrix = Matrix4x4::Translation(position)
                    * Matrix4x4::Rotation(rotation)
                    * Matrix4x4::Scale(scale);
                dirty = false;
            }
            return modelMatrix;
        }

        // ========== 更新接口 ==========

        /**
         * @brief 每帧更新
         * @details 子类可以重写此方法，实现自己的更新逻辑。
         */
        virtual void Update() {}
    };
}