#include <pch.h>
#include "Label.h"
#include "TextureCache.h"
#include "Director.h"
namespace DW
{
	Microsoft::WRL::ComPtr<IDWriteFontCollection>	Label::s_font_collection_;
	bool Label::s_init_state = false;

	Label::Label(const std::string& text)
	{
		auto& render = Director::GetInstance().GetRender();
		auto wtext = StringUtil::CstrToWstr(text);
		HR(render.d2d_render_target->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Yellow, alpha_with_parent_),
			color_brush_.GetAddressOf()));

		HR(render.d2d_render_target->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black, alpha_with_parent_),
			border_brush_.GetAddressOf()));

		text_render_ = std::make_unique<TextRender>(render.d2d_factory,
			render.d2d_render_target,
			color_brush_.Get(),
			border_brush_.Get());
		Microsoft::WRL::ComPtr<IDWriteFontFamily> fontFamily;
		wchar_t name_buf[128];

		for (int i = 0; i < s_font_collection_->GetFontFamilyCount(); i++)
		{
			fontFamily.Reset();
			s_font_collection_->GetFontFamily(i, fontFamily.GetAddressOf());
			Microsoft::WRL::ComPtr <IDWriteLocalizedStrings> name;
			fontFamily->GetFamilyNames(name.GetAddressOf());
			name->GetString(0, name_buf, 128);
		}
		
		HR(render.dwrite_factory->CreateTextFormat(L"Tensentype QinYuanJ-W1", 
			s_font_collection_.Get(),
			//DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_WEIGHT_BOLD,
			DWRITE_FONT_STYLE_NORMAL, 
			DWRITE_FONT_STRETCH_NORMAL, 
			30.0f, L"", 
			text_format_.GetAddressOf()));

		//HR(render.dwrite_factory->CreateTextFormat(L"ËÎÌו",
		//	nullptr,
		//	DWRITE_FONT_WEIGHT_NORMAL,
		//	DWRITE_FONT_STYLE_NORMAL,
		//	DWRITE_FONT_STRETCH_NORMAL,
		//	30.0f, L"",
		//	text_format_.GetAddressOf()));

		HR(render.dwrite_factory->CreateTextLayout(
			wtext.c_str(),                       // Text to be displayed
			wtext.size(),                     // Length of the text
			text_format_.Get(),                 // DirectWrite Text Format object
			200,                         // Width of the Text Layout
			40,                        // Height of the Text Layout
			&text_layout_
		));
		//text_layout_->SetUnderline(TRUE, { 0, text.size() });
		DWRITE_TEXT_METRICS info;
		text_layout_->GetMetrics(&info);
		SetContentSize(info.width, info.height);
	}

	Label::~Label()
	{

	}

	std::shared_ptr<Label> Label::Create(const std::string& text, const std::string& ttf_font_file)
	{
		if (!s_init_state)
		{
			InitFontAndRender();
		}
		return std::make_shared<Label>(text);
	}
	void Label::InitFontAndRender()
	{
		if (!s_init_state)
		{
			std::wstring fontFilename = L"resources/fonts/lantinghei.ttf";
			if (AddFontResource(fontFilename.c_str()) == 0)
			{
				OutputDebugString(L"Error adding font resource!\n");
			}
			
			if (FAILED(Director::GetInstance().GetRender().dwrite_factory->GetSystemFontCollection(s_font_collection_.GetAddressOf(), false)))
			{
				OutputDebugString(L"Failed to retrieve system font collection.\n");
			}
			s_init_state = true;
		}
	}
	void Label::Draw()
	{
		auto& render = Director::GetInstance().GetRender();
		auto transform = SimpleMath::XMMATRIXConvertToMatrix3x2F(transform_matrix_with_parent_);
		render.d2d_render_target->SetTransform(transform);
		render.d2d_render_target->BeginDraw();
		text_layout_->Draw(nullptr, text_render_.get(),0.f, 0.f);
		render.d2d_render_target->EndDraw();
	}
}
