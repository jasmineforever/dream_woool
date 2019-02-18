#pragma once
#include "Node.h"
#include "TextRender.h"
namespace DW
{
	class Label : public Node
	{
	public:
		Label(const std::string& text);

		~Label();

		void SetText(const std::string& text);

		void Draw();

		static std::shared_ptr<Label> Create(const std::string& text = "");
	private:
		static void InitFontAndRender();
	private:
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>			color_brush_;				// 单色笔刷
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>			border_brush_;				// 单色笔刷
		Microsoft::WRL::ComPtr<IDWriteFont>						font_;								// 字体
		Microsoft::WRL::ComPtr<IDWriteTextFormat>				text_format_;					// 文本格式color_brush_;
		std::unique_ptr<TextRender>								text_render_;
		Microsoft::WRL::ComPtr<IDWriteTextLayout>				text_layout_;
		static Microsoft::WRL::ComPtr<IDWriteFontCollection>	s_font_collection_;
		static bool s_init_state;
	};
}

