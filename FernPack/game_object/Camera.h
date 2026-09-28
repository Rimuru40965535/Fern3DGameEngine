/**
* @file FernPack/game_object/Camera.h
* @brief 相机
* @details
* 继承自 Object，额外持有投影参数。
* 观察矩阵 = 相机模型矩阵的逆。
* 投影矩阵由视场角、宽高比、近平面、远平面决定。
* 相机空间：+X 左，+Y 上，+Z 前（右手系）。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/9/28
*
*/
#pragma once


namespace Fern::Scene {

    /**
    * @brief 相机
    * @details
    * 继承自 Object，额外持有投影参数。
    * 观察矩阵 = 相机模型矩阵的逆。
    */
    class Camera : public Object {
    protected:
        double fov;             ///< 视场角（弧度）
        double nearPlane;       ///< 近平面
        double farPlane;        ///< 远平面

    public:
        /**
        * @brief 默认构造函数
        * @details 初始化为标准相机：fov = 60度，aspect = 4/3，near = 0.1，far = 100
        */
        Camera()
            : Object(),
            fov( Fern::Math::PI / 3.0),
            nearPlane(0.1),
            farPlane(100.0)
        {
        }

        /**
        * @brief 带参构造函数
        * @param double f 视场角（弧度）
        * @param double a 宽高比
        * @param double n 近平面
        * @param double f2 远平面
        */
        Camera(double f, double n, double f2)
            : Object(),
            fov(f),
            nearPlane(n),
            farPlane(f2)
        {
        }

        /**
        * @brief 虚析构函数
        */
        virtual ~Camera() = default;

        // ========== 投影参数访问 ==========

        void SetFov(double f) { fov = f; }
        void SetNearPlane(double n) { nearPlane = n; }
        void SetFarPlane(double f) { farPlane = f; }

        double GetFov() const { return fov; }
        double GetNearPlane() const { return nearPlane; }
        double GetFarPlane() const { return farPlane; }

        // ========== 矩阵获取 ==========

        /**
        * @brief 获取投影矩阵
        * @details 使用预先设置的宽高比生成投影矩阵
        * @return Matrix4x4 投影矩阵
        */
        Matrix4x4 GetProjMatrix(const Fern::Window& w) const {
            return Fern::GeneratePerspectiveMatrix(
                w.Width(),
                w.Height(),
                fov,
                nearPlane,
                farPlane
            );
        }
    };

}