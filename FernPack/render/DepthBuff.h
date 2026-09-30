/**
* @file FernPack/render/DepthBuff.h
* @brief 深度缓冲
* @details
* 管理一个二维 double 数组，记录每个像素位置的最小深度值。
* 每帧开始时需要重置为最远值。
* 提供遮挡测试的判据：当前深度是否小于缓冲中的深度。
*
* @author 半人马座beta星（AIGC：DeepSeek）
* @date 2026/10/1
*
*/
#pragma once

#define DEPTHBUFF_FARTHEST_DEPTH 1.5

namespace Fern::Render {

    /**
    * @brief 深度缓冲
    * @details
    * 管理一个二维 double 数组，记录每个像素位置的最小深度值。
    * 每帧开始时需要重置为最远值。
    * 提供遮挡测试的判据：当前深度是否小于缓冲中的深度。
    */
    class DepthBuff {
    protected:
        double** buffer;    ///< 二维深度数组 buffer[y][x]
        int width;          ///< 宽度
        int height;         ///< 高度

    public:
        /**
        * @brief 构造函数
        * @param Window& w 窗口引用，用于读取宽高
        * @param double far 最远值，默认为 1.0
        */
        DepthBuff(const Window& w)
            : width(w.Width()), height(w.Height())
        {
            buffer = new double* [height];
            for (int row = 0; row < height; ++row) {
                buffer[row] = new double[width];
                for (int col = 0; col < width; ++col) {
                    buffer[row][col] = DEPTHBUFF_FARTHEST_DEPTH;
                }
            }
        }

        /**
        * @brief 析构函数
        * @details 释放二维数组
        */
        ~DepthBuff() {
            if (buffer) {
                for (int row = 0; row < height; ++row) {
                    delete[] buffer[row];
                }
                delete[] buffer;
                buffer = nullptr;
            }
        }

        // 禁止拷贝
        DepthBuff(const DepthBuff&) = delete;
        DepthBuff& operator=(const DepthBuff&) = delete;

        /**
        * @brief 重置深度缓冲
        * @details 将所有像素的深度值重置为最远值。每帧开始时调用。
        */
        void Reset() {
            for (int row = 0; row < height; ++row) {
                for (int col = 0; col < width; ++col) {
                    buffer[row][col] = DEPTHBUFF_FARTHEST_DEPTH;
                }
            }
        }

        /**
        * @brief 获取指定位置的深度值
        */
        double GetDepth(int x, int y) const {
            if (x < 0 || x >= width || y < 0 || y >= height) {
                return DEPTHBUFF_FARTHEST_DEPTH;
            }
            return buffer[y][x];
        }

        void SetDepth(int x, int y, double depth) {
            if (x < 0) return;
            if (x >= width) return;
            if (y < 0) return;
            if (y >= height) return;

            buffer[y][x] = depth;
        }

        int Width() const { return width; }
        int Height() const { return height; }
    };

} 