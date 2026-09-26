/**
 * @file Fern3DEngine/FernCore/Window.h
 *
 * @brief 窗口管理模块
 * @details
 * 该模块执行窗口的管理与操作。暂时没有尝试过多窗口绘制
 * 
 * 
 *
 *
 * @author 半人马座beta星
 * @copyright Copyright (c) 2026 Rimuru Central Institute of Computer Science. All rights reserved.
 * @date 2026/7/22
 */



#pragma once

namespace Fern {

    class Window {
    public:
        int width;             ///<窗口宽度
        int height;            ///<窗口高度
        std::string title;     ///<窗口标题
        bool isOpen;           ///<是否打开

        HWND windowHwnd;       ///<窗口句柄

        Matrix4x4 windowMatrix; ///<视窗变换矩阵

    public:
        /**
        * @brief 构造函数还有一个作用就是自动创建窗口
        * @param int width 窗口宽度
        * @param int height 窗口高度
        * @param const:std::string& title 窗口标题
        * @param int flag eazyx的窗口控制位
        */
        Window(int w, int h, const std::string& t, int flag = 0)
            : width(w), height(h), title(t), isOpen(true), 
            windowMatrix(MATRIX_INIT_WITH_UNIT_MATRIX)
        {
            //初始化图形窗口，同时显示控制台
            initgraph(width, height, flag);

            //改变窗口标题
            windowHwnd = GetHWnd(); // 获取窗口句柄
            if (windowHwnd) {
                // 将 std::string 转换为宽字符 (wstring)
                std::wstring wtitle(title.begin(), title.end());
                SetWindowTextW(windowHwnd, wtitle.c_str());
            }

            //视口变换矩阵初始化
            windowMatrix.values[0][0] = -(double)width / 2;
            windowMatrix.values[0][3] = (double)width / 2 +0.5;     ///<带有0.5的偏移可以使点被映射到像素中央

            windowMatrix.values[1][1] = -(double)height / 2;
            windowMatrix.values[1][3] = (double)height / 2 +0.5;


            BeginBatchDraw();
        }

        /**
        * @brief 析构函数还有一个作用就是自动析构窗口
        */
        ~Window() {

            EndBatchDraw();

            if (isOpen) {
                closegraph();
                isOpen = false;
            }
        }

        // 清除屏幕为黑色（默认背景色）
        void Clear() {
            cleardevice();
        }

        // 交换缓冲区，将绘图内容显示到屏幕
        void Present() {
            FlushBatchDraw();
        }

        /**
        * @brief 处理窗口消息（如关闭、调整大小等）
        */
        void PullEvents() {
            // 处理 Windows 消息，避免窗口无响应
            // 可以使用 peekmessage 或直接调用 Windows API 的 PeekMessage
            // 简单起见，这里我们检查是否有按键消息，但为了保持窗口响应，需要处理系统消息。
            // 更健壮的方法：使用 EasyX 的 getmessage 或 Windows API。
            // 这里提供一个简单的空实现，专注于绘图。
            // 实际应用中，可以在这里处理窗口关闭消息。
        }

        /**
         * @brief 检查窗口是否应该被关闭
         */
        bool ShouldClose() const {
            return !isOpen;
        }

       
        int Width() const { return width; }        ///>获取窗口宽度
        int Height() const { return height; }      ///>获取窗口高度

        Window(const Window&) = delete;             ///>禁止拷贝（窗口对象不应被复制）
        Window& operator=(const Window&) = delete;  ///>禁止赋值（窗口对象不应被赋值）

        // 提供一个方法让 EasyX 的绘图函数可以访问窗口句柄（如果需要）
        // 对于我们的软渲染器，我们不需要直接暴露 HWND

        int DrawPixel(int x, int y, Color c) {
            if (x < 0) { return -1; }
            if (x >= width) { return -1; }
            if (y < 0) { return -2; }
            if (y >= height) { return -2; }

            putpixel(x, y, c.ToEasyXColor());
            return 1;
        }
    };

    
} 