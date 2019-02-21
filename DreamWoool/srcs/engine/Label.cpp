#include <pch.h>
#include "Label.h"
#include "Director.h"
#include "FontManager.h"
namespace DW
{
	Microsoft::WRL::ComPtr<IDWriteFontCollection>	Label::s_font_collection_;

	Label::Label(const std::string& text,
		int font_size,
		const DWColor& color,
		const std::string& font_family):
		text_(StringUtil::CstrToWstr(text)),
		font_color_(color),
		border_color_(DWColor::Black),
		font_size_(font_size),
		border_(false),
		bold_(false),
		underline_(false),
		italic_(false),
		font_family_(StringUtil::CstrToWstr(font_family))
	{
		if (font_family_.empty())
		{
			font_family_ = L"Arial";
		}
		CreateFontBrush();

		if (border_)
		{
			CreateBorderBrush();
		}
		CreateTextRender();
		CreateTextFormat();
		CreateTextLayout();
	}

	Label::~Label()
	{

	}

	std::shared_ptr<Label> Label::Create(const std::string& text,
		int font_size,
		const DWColor& color,
		const std::string& ttf_font_file)
	{
		if (s_font_collection_.Get())
		{
			Director::GetInstance().GetRender().dwrite_factory->GetSystemFontCollection(s_font_collection_.GetAddressOf());
		}
		return std::make_shared<Label>(text, font_size, color, FontManager::GetInstance().GetFontFamilyName(ttf_font_file));
	}
	void Label::SetText(const std::string& text)
	{
		text_ = StringUtil::CstrToWstr(text);
		CreateTextLayout();
	}

	std::string Label::GetText(const std::string& text) const
	{
		return StringUtil::WstrToCstr(text_);
	}

	void Label::SetFontSize(int size)
	{
		if (font_size_ != size)
		{
			font_size_ = size;
			CreateTextFormat();
			CreateTextLayout();
		}
	}

	int Label::GetFontSize()
	{
		return font_size_;
	}

	void Label::SetFontColor(const DWColor& color)
	{
		font_color_ = color;
		font_brush_->SetColor(D2D1::ColorF(font_color_.GetByteColor(), alpha_with_parent_ * font_color_.GetFloatA()));
		CreateTextRender();
	}

	const DWColor& Label::GetFontColor() const
	{
		return font_color_;
	}

	void Label::SetBold(bool bold)
	{
		if (bold != bold_)
		{
			bold_ = bold;
			CreateTextFormat();
			CreateTextLayout();
		}
	}

	bool Label::GetBold()
	{
		return bold_;
	}

	void Label::SetItalic(bool italic)
	{
		if (italic != italic_)
		{
			italic_ = italic;
			CreateTextFormat();
			CreateTextLayout();
		}
	}

	bool Label::GetItalic()
	{
		return italic_;
	}

	void Label::SetUnderline(bool underline)
	{
		if (underline != underline_)
		{
			underline_ = underline;
			text_layout_->SetUnderline(underline_, { 0, text_.size() });
		}
	}

	bool Label::GetUnderline()
	{
		return underline_;
	}

	void Label::SetBorder(bool border)
	{
		if (border != border_)
		{
			border_ = border;
			if (border_)
			{
				CreateBorderBrush();
				CreateTextRender();
			}
			else
			{
				border_brush_.Reset();
				CreateTextRender();
			}
		}
	}

	bool Label::GetBorder()
	{
		return border_;
	}

	void Label::SetBorderColor(const DWColor& color)
	{
		border_color_ = color;
		if (border_)
		{
			CreateBorderBrush();
			CreateTextRender();
		}
	}

	const DWColor& Label::GetBorderColor() const
	{
		return border_color_;
	}

	void Label::CreateTextLayout()
	{
		auto screen_size = Director::GetInstance().GetScreenSize();
		text_layout_.Reset();
		HR(Director::GetInstance().GetRender().dwrite_factory->CreateTextLayout(
			text_.c_str(),                       // Text to be displayed
			text_.size(),                     // Length of the text
			text_format_.Get(),                 // DirectWrite Text Format object
			screen_size.width,                        // Width of the Text Layout
			screen_size.height,                       // Height of the Text Layout
			&text_layout_
		));
		if (underline_)
		{
			text_layout_->SetUnderline(true, {0, text_.size()});
		}
		DWRITE_TEXT_METRICS info;
		text_layout_->GetMetrics(&info);
		SetContentSize(info.width, info.height);
	}
	void Label::CreateTextFormat()
	{
		text_format_.Reset();
		DWRITE_FONT_STYLE style = italic_ ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL;
		DWRITE_FONT_WEIGHT weight = bold_ ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL;
		HR(Director::GetInstance().GetRender().dwrite_factory->CreateTextFormat(font_family_.c_str(),
			s_font_collection_.Get(),
			weight,
			style,
			DWRITE_FONT_STRETCH_NORMAL,
			static_cast<float>(font_size_), L"",
			text_format_.GetAddressOf()));
	}
	void Label::CreateFontBrush()
	{
		font_brush_.Reset();
		HR(Director::GetInstance().GetRender().d2d_render_target->CreateSolidColorBrush(
			D2D1::ColorF(font_color_.GetByteColor(), alpha_with_parent_ * font_color_.GetFloatA()),
			font_brush_.GetAddressOf()));
	}
	void Label::CreateBorderBrush()
	{
		border_brush_.Reset();
		HR(Director::GetInstance().GetRender().d2d_render_target->CreateSolidColorBrush(
			D2D1::ColorF(border_color_.GetByteColor(), alpha_with_parent_ * border_color_.GetFloatA()),
			border_brush_.GetAddressOf()));
	}
	void Label::CreateTextRender()
	{
		text_render_.reset();
		text_render_ = std::make_unique<TextRender>(Director::GetInstance().GetRender().d2d_factory,
			Director::GetInstance().GetRender().d2d_render_target,
			font_brush_.Get(),
			border_brush_.Get());
	}
	void Label::Draw()
	{
		auto& render = Director::GetInstance().GetRender();
		auto transform = SimpleMath::XMMATRIXConvertToMatrix3x2F(transform_matrix_with_parent_);
		render.d2d_render_target->SetTransform(transform);
		render.d2d_render_target->BeginDraw();
		text_layout_->Draw(nullptr, text_render_.get(), 0.f, 0.f);
		render.d2d_render_target->EndDraw();
	}
}
