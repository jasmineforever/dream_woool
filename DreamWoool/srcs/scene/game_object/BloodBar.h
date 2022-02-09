#pragma once
#include "DrawNode.h"
#include "Color.h"
#include "Label.h"
namespace DW
{
	class BloodBar : public DrawNode
	{
	public:
		BloodBar(int width, int height,
			int min_value, int max_value, int now_value,
			bool enable_percentage,
			DWColor fill_color,
			DWColor border_color,
			DWColor text_color);
		~BloodBar();
		static std::shared_ptr<BloodBar> Create(int width, int height, 
			int min_value, int max_value, int now_value,
			bool enable_percentage = false, 
			DWColor fill_color = DWColor(static_cast<uint8_t>(207), 14, 7),
			DWColor border_color = DWColor::Black,
			DWColor text_color = DWColor::White);
		void EnablePercentage(bool enable);
		void SetValue(int value);
	private:
		BloodBar(const BloodBar&) = delete;
		BloodBar& operator =(const BloodBar&) = delete;

		bool Init();

		void DrawBar();
	private:
		std::shared_ptr<Label>	label_;
		int						width_;
		int						height_;
		int						min_value_;
		int						max_value_;
		int						now_value_;
		bool					enable_percentage_;
		DWColor					fill_color_;
		DWColor					border_color_;
		DWColor					text_color_;
	};
}


