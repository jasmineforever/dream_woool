#pragma once
#include "SpriteFrame.h"

namespace DW
{
	class SpriteFrameAnimation
	{
	public:
		SpriteFrameAnimation(const std::vector<std::shared_ptr<SpriteFrame>>& frames, float frame_time, bool is_loop);

		~SpriteFrameAnimation();

		static std::shared_ptr<SpriteFrameAnimation> Create(const std::vector<std::shared_ptr<SpriteFrame>>& frames, float frame_time, bool is_loop);

		void SetSprite(void* sp);

		bool Update(float dt);
	private:
		SpriteFrameAnimation(const SpriteFrameAnimation&) = delete;

		SpriteFrameAnimation& operator = (const SpriteFrameAnimation&) = delete;
	private:
		bool										is_loop_;
		float										frame_time_;
		std::vector<std::shared_ptr<SpriteFrame>>	sprite_frames_;
		void*										sp_;
		int											index_;
		float										last_frame_time_;
		float										timeline_;
		bool										is_end_;
	};
}