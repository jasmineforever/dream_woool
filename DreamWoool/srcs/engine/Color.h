#pragma once

namespace DW
{
	class DWColor
	{
	public:
		DWColor();

		DWColor(float r, float g, float b, float a = 1.0);

		DWColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

		DWColor(uint32_t color);

		DWColor(const std::string& color);

		~DWColor() = default;

		const XMVECTORF32& GetFloatColor();

		uint32_t GetByteColor();

		float GetFloatR();

		void SetFloatR(float r);

		float GetFloatG();

		void SetFloatg(float g);

		float GetFloatB();

		void SetFloatB(float b);

		float GetFloatA();

		void SetFloatA(float a);

		uint8_t GetByteR();

		void SetByteR(uint8_t r);

		uint8_t GetByteG();

		void SetByteG(uint8_t g);

		uint8_t GetByteB();

		void SetByteB(uint8_t b);

		uint8_t GetByteA();

		void SetByteA(uint8_t a);
	private:
		uint32_t	rgb_byte_; 
		XMVECTORF32 rgba_float_;
	public:
		static DWColor AliceBlue;
		static DWColor AntiqueWhite;
		static DWColor Aqua;
		static DWColor Aquamarine;
		static DWColor Azure;
		static DWColor Beige;
		static DWColor Bisque;
		static DWColor Black;
		static DWColor BlanchedAlmond;
		static DWColor Blue;
		static DWColor BlueViolet;
		static DWColor Brown;
		static DWColor BurlyWood;
		static DWColor CadetBlue;
		static DWColor Chartreuse;
		static DWColor Chocolate;
		static DWColor Coral;
		static DWColor CornflowerBlue;
		static DWColor Cornsilk;
		static DWColor Crimson;
		static DWColor Cyan;
		static DWColor DarkBlue;
		static DWColor DarkCyan;
		static DWColor DarkGoldenrod;
		static DWColor DarkGray;
		static DWColor DarkGreen;
		static DWColor DarkKhaki;
		static DWColor DarkMagenta;
		static DWColor DarkOliveGreen;
		static DWColor DarkOrange;
		static DWColor DarkOrchid;
		static DWColor DarkRed;
		static DWColor DarkSalmon;
		static DWColor DarkSeaGreen;
		static DWColor DarkSlateBlue;
		static DWColor DarkSlateGray;
		static DWColor DarkTurquoise;
		static DWColor DarkViolet;
		static DWColor DeepPink;
		static DWColor DeepSkyBlue;
		static DWColor DimGray;
		static DWColor DodgerBlue;
		static DWColor Firebrick;
		static DWColor FloralWhite;
		static DWColor ForestGreen;
		static DWColor Fuchsia;
		static DWColor Gainsboro;
		static DWColor GhostWhite;
		static DWColor Gold;
		static DWColor Goldenrod;
		static DWColor Gray;
		static DWColor Green;
		static DWColor GreenYellow;
		static DWColor Honeydew;
		static DWColor HotPink;
		static DWColor IndianRed;
		static DWColor Indigo;
		static DWColor Ivory;
		static DWColor Khaki;
		static DWColor Lavender;
		static DWColor LavenderBlush;
		static DWColor LawnGreen;
		static DWColor LemonChiffon;
		static DWColor LightBlue;
		static DWColor LightCoral;
		static DWColor LightCyan;
		static DWColor LightGoldenrodYellow;
		static DWColor LightGreen;
		static DWColor LightGray;
		static DWColor LightPink;
		static DWColor LightSalmon;
		static DWColor LightSeaGreen;
		static DWColor LightSkyBlue;
		static DWColor LightSlateGray;
		static DWColor LightSteelBlue;
		static DWColor LightYellow;
		static DWColor Lime;
		static DWColor LimeGreen;
		static DWColor Linen;
		static DWColor Magenta;
		static DWColor Maroon;
		static DWColor MediumAquamarine;
		static DWColor MediumBlue;
		static DWColor MediumOrchid;
		static DWColor MediumPurple;
		static DWColor MediumSeaGreen;
		static DWColor MediumSlateBlue;
		static DWColor MediumSpringGreen;
		static DWColor MediumTurquoise;
		static DWColor MediumVioletRed;
		static DWColor MidnightBlue;
		static DWColor MintCream;
		static DWColor MistyRose;
		static DWColor Moccasin;
		static DWColor NavajoWhite;
		static DWColor Navy;
		static DWColor OldLace;
		static DWColor Olive;
		static DWColor OliveDrab;
		static DWColor Orange;
		static DWColor OrangeRed;
		static DWColor Orchid;
		static DWColor PaleGoldenrod;
		static DWColor PaleGreen;
		static DWColor PaleTurquoise;
		static DWColor PaleVioletRed;
		static DWColor PapayaWhip;
		static DWColor PeachPuff;
		static DWColor Peru;
		static DWColor Pink;
		static DWColor Plum;
		static DWColor PowderBlue;
		static DWColor Purple;
		static DWColor Red;
		static DWColor RosyBrown;
		static DWColor RoyalBlue;
		static DWColor SaddleBrown;
		static DWColor Salmon;
		static DWColor SandyBrown;
		static DWColor SeaGreen;
		static DWColor SeaShell;
		static DWColor Sienna;
		static DWColor Silver;
		static DWColor SkyBlue;
		static DWColor SlateBlue;
		static DWColor SlateGray;
		static DWColor Snow;
		static DWColor SpringGreen;
		static DWColor SteelBlue;
		static DWColor Tan;
		static DWColor Teal;
		static DWColor Thistle;
		static DWColor Tomato;
		static DWColor Turquoise;
		static DWColor Violet;
		static DWColor Wheat;
		static DWColor White;
		static DWColor WhiteSmoke;
		static DWColor Yellow;
		static DWColor YellowGreen;
		//ÖÐ¹úÉ«
		static DWColor CHuaQing;// 2376b7
		static DWColor CCuiLv; //0x20a162;
		static DWColor CDouSha; //481e1c
		static DWColor CHeDingHong; //d42517
	};
}