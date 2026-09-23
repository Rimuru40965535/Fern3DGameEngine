#pragma once



namespace Fern {
	//颜色定义
	struct Color {
	public:
		union {
			struct {
				uint8_t b;	//blue
				uint8_t g;	//green
				uint8_t r;	//red
				uint8_t a;	//alpha
			};
			uint32_t argb;	//全色号
		};
	public:
		Color() :argb(0xFFFFFFFF){}								//默认构造，白色
		Color(uint8_t red, uint8_t green, uint8_t blue)			//从RGB构造
			: r(red), g(green), b(blue), a(255) {}
		Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)//从RGBA构造
			: r(red), g(green), b(blue), a(alpha) {}
		explicit Color(uint32_t rgbaValue) : argb(rgbaValue) {}	//从色号构造

		// --- 转换为 EasyX COLORREF ---
		// EasyX 使用 COLORREF，实际为 0x00BBGGRR 格式 (顺序不同)
		// 这个方法将 RGBA 转换为 EasyX 能识别的格式。
		COLORREF ToEasyXColor() const {
			// 使用 EasyX 的 RGB 宏，它接受 (红, 绿, 蓝) 并生成 0x00BBGGRR
			return RGB(r, g, b);
		}

		uint32_t getARGB()const {
			return argb;
		}

		// --- 预定义常用颜色（静态工厂方法）---

		inline static Color Red()			{ return Color(	255,	0,		0				); }
		inline static Color Green()			{ return Color(	0,		255,	0				); }
		inline static Color Blue()			{ return Color(	0,		0,		255				); }
		inline static Color White()			{ return Color(	255,	255,	255				); }
		inline static Color Black()			{ return Color(	0,		0,		0				); }
		inline static Color Yellow()		{ return Color(	255,	255,	0				); }
		inline static Color Cyan()			{ return Color(	0,		255,	255				); }
		inline static Color Orange()		{ return Color(	255,	127,	0				); }
		inline static Color Magenta()		{ return Color(	255,	0,		255				); }
		inline static Color RimuruBlue()	{ return Color(	0xcf,	0xef,	0xfc			); }
		inline static Color Transparent()	{ return Color(	0,		0,		0,		0		); }
	};
}