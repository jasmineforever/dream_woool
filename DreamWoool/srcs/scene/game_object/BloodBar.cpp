#include "pch.h"
#include "BloodBar.h"
#include <cmath>
#include <sstream>
namespace DW
{
	BloodBar::BloodBar(int width, int height,
		int min_value, int max_value, int now_value,
		bool enable_percentage,
		DWColor fill_color,
		DWColor border_color,
		DWColor text_color):
		width_(width),
		height_(height),
		min_value_(min_value),
		max_value_(max_value),
		now_value_(now_value),
		enable_percentage_(enable_percentage),
		fill_color_(fill_color),
		border_color_(border_color),
		text_color_(text_color)

	{

	}

	BloodBar::~BloodBar()
	{
	}
	void BloodBar::EnablePercentage(bool enable)
	{
		enable_percentage_ = enable;
	}
	std::shared_ptr<BloodBar> BloodBar::Create(int width, int height,
		int min_value, int max_value, int now_value,
		bool enable_percentage,
		DWColor fill_color,
		DWColor border_color,
		DWColor text_color)
	{
		auto bar = std::make_shared<BloodBar>(width, height, min_value, max_value, now_value,
			enable_percentage, fill_color, border_color, text_color);
		if (bar && bar->Init())
		{
			return bar;
		}
		return nullptr;
	}
	void BloodBar::SetValue(int value)
	{
		now_value_ = value;
		DrawBar();
	}
	void BloodBar::DrawBar()
	{
		int percent = 0;
		int drawWidth = 0;
		std::stringstream format_stream;
		if (now_value_ <= min_value_)
		{
			drawWidth = 0;
			percent = 0;
		}
		else if (now_value_ >= max_value_)
		{
			drawWidth = width_;
			percent = 100;
		}
		else
		{
			float percentf = ((float)now_value_ - (float)min_value_) / ((float)max_value_ - (float)min_value_);
			percent = std::floor(100.0 * percentf);
			drawWidth = percentf * width_;
		}
		Clear();
		DrawQuad(Vector2(-0.5, 0.), Size(width_ , height_ ), border_color_.GetFloatColor());
		DrawQuadSolid(Vector2(0.5, 0.5), Size(drawWidth - 1., height_ - 1.), fill_color_.GetFloatColor());
		
		if (enable_percentage_)
		{
			format_stream << percent << "/" << "100";
		}
		else
		{
			format_stream << now_value_<< "/" << max_value_;
		}
		
		label_->SetText(format_stream.str());
		label_->SetPosition((width_ - label_->GetContentSize().width) / 2, 4);

	}
	bool BloodBar::Init()
	{
		if (width_ < 2 || height_ < 2 || min_value_ < 0 || max_value_ < min_value_)
		{
			return false;
		}

		label_ = Label::Create("0", 11);
		label_->SetFontColor(text_color_);
		label_->SetBorder(true);
		AddChild(label_);
			
		DrawBar();
		return true;
	}
}
