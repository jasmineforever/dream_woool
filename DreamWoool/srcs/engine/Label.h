#pragma once
#include "Node.h"
#include "TextRender.h"
#include "Color.h"
namespace DW
{
	class Label : public Node
	{
	public:
		Label(const std::string& text,
			int font_size,
			const DWColor& color,
			const std::string& font_family);

		~Label();

		void SetText(const std::string& text);

		std::string GetText(const std::string& text) const;

		void SetFontSize(int size);

		int GetFontSize();

		void SetFontColor(const DWColor& color);

		const DWColor& GetFontColor() const;

		void SetBold(bool bold);

		bool GetBold();

		void SetItalic(bool italic);

		bool GetItalic();

		void SetUnderline(bool underline);

		bool GetUnderline();

		void SetBorder(bool border);

		bool GetBorder();

		void SetBorderColor(const DWColor& color);

		const DWColor&  GetBorderColor() const;

		void Draw();

		static std::shared_ptr<Label> Create(const std::string& text = "",
			int font_size = 15,
			const DWColor& color = DWColor::White,
			const std::string& ttf_font_file = ""
			);
	private:
		void CreateTextLayout();
		void CreateTextFormat();
		void CreateFontBrush();
		void CreateBorderBrush();
		void CreateTextRender();
	private:
		static Microsoft::WRL::ComPtr<IDWriteFontCollection>	s_font_collection_;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>			font_brush_;				// 单色笔刷
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>			border_brush_;				// 单色笔刷
		Microsoft::WRL::ComPtr<IDWriteFont>						font_;						// 字体
		Microsoft::WRL::ComPtr<IDWriteTextFormat>				text_format_;				// 文本格式color_brush_;
		Microsoft::WRL::ComPtr<IDWriteTextLayout>				text_layout_;
		std::unique_ptr<TextRender>								text_render_;
		std::wstring											font_family_;
		std::wstring											text_;
		DWColor													font_color_;
		DWColor													border_color_;
		int														font_size_;
		bool													border_;
		bool													bold_;
		bool													underline_;
		bool													italic_;

	};
}

