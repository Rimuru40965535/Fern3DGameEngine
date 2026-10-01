/**
* @file 13-DepthBuff_test.cpp
* @brief 深度缓冲单元测试
* @details
* 验证 DepthBuff 类的构造、宽高访问、深度读写、重置、边界处理。
*
* @author （留白） （AIGC：DeepSeek）
* @date 2026/10/1
*
*/
#include "Fern3DGameEngine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace Fern;

// 辅助函数：判断两个 double 是否接近
bool NearlyEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

int main() {
    std::cout << std::setprecision(6) << std::fixed;

    // 创建测试窗口
    Window window(800, 600, "DepthBuff Test", !EX_SHOWCONSOLE);

    // ========== 测试1：构造后所有像素为最远值 ==========
    std::cout << "========== 测试1：构造后所有像素为最远值 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        bool allFar = true;
        for (int y = 0; y < depthBuff.Height(); ++y) {
            for (int x = 0; x < depthBuff.Width(); ++x) {
                if (!NearlyEqual(depthBuff.GetDepth(x, y), DEPTHBUFF_FARTHEST_DEPTH)) {
                    allFar = false;
                    break;
                }
            }
            if (!allFar) break;
        }

        std::cout << "所有像素为最远值: " << (allFar ? "v" : "x") << std::endl;
        std::cout << "结果: " << (allFar ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试2：宽高访问 ==========
    std::cout << "========== 测试2：宽高访问 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        bool widthOk = (depthBuff.Width() == 800);
        bool heightOk = (depthBuff.Height() == 600);

        std::cout << "宽度: " << depthBuff.Width() << " (期望 800)" << std::endl;
        std::cout << "高度: " << depthBuff.Height() << " (期望 600)" << std::endl;
        std::cout << "结果: " << (widthOk && heightOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试3：GetDepth 有效坐标 ==========
    std::cout << "========== 测试3：GetDepth 有效坐标 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        bool corner00Ok = NearlyEqual(depthBuff.GetDepth(0, 0), DEPTHBUFF_FARTHEST_DEPTH);
        bool cornerMaxOk = NearlyEqual(depthBuff.GetDepth(799, 599), DEPTHBUFF_FARTHEST_DEPTH);
        bool centerOk = NearlyEqual(depthBuff.GetDepth(400, 300), DEPTHBUFF_FARTHEST_DEPTH);

        std::cout << "左上角深度: " << depthBuff.GetDepth(0, 0) << std::endl;
        std::cout << "右下角深度: " << depthBuff.GetDepth(799, 599) << std::endl;
        std::cout << "中心深度: " << depthBuff.GetDepth(400, 300) << std::endl;
        std::cout << "结果: " << (corner00Ok && cornerMaxOk && centerOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试4：GetDepth 越界坐标 ==========
    std::cout << "========== 测试4：GetDepth 越界坐标 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        bool negXOk = NearlyEqual(depthBuff.GetDepth(-1, 0), DEPTHBUFF_FARTHEST_DEPTH);
        bool negYOk = NearlyEqual(depthBuff.GetDepth(0, -1), DEPTHBUFF_FARTHEST_DEPTH);
        bool overXOk = NearlyEqual(depthBuff.GetDepth(800, 0), DEPTHBUFF_FARTHEST_DEPTH);
        bool overYOk = NearlyEqual(depthBuff.GetDepth(0, 600), DEPTHBUFF_FARTHEST_DEPTH);

        std::cout << "负 x 越界: " << (negXOk ? "v" : "x") << std::endl;
        std::cout << "负 y 越界: " << (negYOk ? "v" : "x") << std::endl;
        std::cout << "超 x 越界: " << (overXOk ? "v" : "x") << std::endl;
        std::cout << "超 y 越界: " << (overYOk ? "v" : "x") << std::endl;
        std::cout << "结果: " << (negXOk && negYOk && overXOk && overYOk ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试5：SetDepth 有效坐标 ==========
    std::cout << "========== 测试5：SetDepth 有效坐标 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        depthBuff.SetDepth(100, 200, 0.5);
        bool setOk = NearlyEqual(depthBuff.GetDepth(100, 200), 0.5);

        depthBuff.SetDepth(400, 300, 0.25);
        bool set2Ok = NearlyEqual(depthBuff.GetDepth(400, 300), 0.25);

        std::cout << "设置 (100, 200) = 0.5: " << (setOk ? "v" : "x") << std::endl;
        std::cout << "设置 (400, 300) = 0.25: " << (set2Ok ? "v" : "x") << std::endl;
        std::cout << "结果: " << (setOk && set2Ok ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试6：SetDepth 越界坐标 ==========
    std::cout << "========== 测试6：SetDepth 越界坐标 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        // 先设置一个有效值
        depthBuff.SetDepth(0, 0, 0.5);

        // 尝试越界设置，不应该影响有效值
        depthBuff.SetDepth(-1, 0, 0.1);
        depthBuff.SetDepth(0, -1, 0.1);
        depthBuff.SetDepth(800, 0, 0.1);
        depthBuff.SetDepth(0, 600, 0.1);

        bool unchanged = NearlyEqual(depthBuff.GetDepth(0, 0), 0.5);

        std::cout << "越界设置后 (0, 0) 仍为 0.5: " << (unchanged ? "v" : "x") << std::endl;
        std::cout << "结果: " << (unchanged ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试7：Reset 重置 ==========
    std::cout << "========== 测试7：Reset 重置 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        // 修改一些像素
        depthBuff.SetDepth(100, 200, 0.5);
        depthBuff.SetDepth(400, 300, 0.25);
        depthBuff.SetDepth(0, 0, 0.1);

        // 重置
        depthBuff.Reset();

        // 验证所有像素都回到最远值
        bool allFar = true;
        for (int y = 0; y < depthBuff.Height(); ++y) {
            for (int x = 0; x < depthBuff.Width(); ++x) {
                if (!NearlyEqual(depthBuff.GetDepth(x, y), DEPTHBUFF_FARTHEST_DEPTH)) {
                    allFar = false;
                    break;
                }
            }
            if (!allFar) break;
        }

        std::cout << "重置后所有像素为最远值: " << (allFar ? "v" : "x") << std::endl;
        std::cout << "结果: " << (allFar ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    // ========== 测试8：边界坐标 ==========
    std::cout << "========== 测试8：边界坐标 ==========" << std::endl;
    {
        Render::DepthBuff depthBuff(window);

        // 边界坐标应该有效
        depthBuff.SetDepth(0, 0, 0.1);
        depthBuff.SetDepth(799, 0, 0.2);
        depthBuff.SetDepth(0, 599, 0.3);
        depthBuff.SetDepth(799, 599, 0.4);

        bool corner00Ok = NearlyEqual(depthBuff.GetDepth(0, 0), 0.1);
        bool corner10Ok = NearlyEqual(depthBuff.GetDepth(799, 0), 0.2);
        bool corner01Ok = NearlyEqual(depthBuff.GetDepth(0, 599), 0.3);
        bool corner11Ok = NearlyEqual(depthBuff.GetDepth(799, 599), 0.4);

        std::cout << "左上角 (0, 0): " << (corner00Ok ? "v" : "x") << std::endl;
        std::cout << "右上角 (799, 0): " << (corner10Ok ? "v" : "x") << std::endl;
        std::cout << "左下角 (0, 599): " << (corner01Ok ? "v" : "x") << std::endl;
        std::cout << "右下角 (799, 599): " << (corner11Ok ? "v" : "x") << std::endl;
        std::cout << "结果: " << (corner00Ok && corner10Ok && corner01Ok && corner11Ok ? "v 通过" : "x 失败") << std::endl << std::endl;
    }

    return 0;
}