#include "pch.h"
#include "SpriteFrameAnimation.h"
#include"Sprite.h"

namespace DW
{
	SpriteFrameAnimation::SpriteFrameAnimation(const std::vector<std::shared_ptr<SpriteFrame>>& frames,
		float frame_time, 
		bool is_loop):
		frame_time_(frame_time),
		sprite_frames_(frames),
		is_loop_(is_loop),
		sp_(nullptr),
		index_(0),
		is_end_(false),
		last_frame_time_(0.),
		timeline_(0.)
	{

	}
	SpriteFrameAnimation::~SpriteFrameAnimation()
	{

	}
	void SpriteFrameAnimation::SetSprite(void* sp)
	{
		
		sp_ = sp;
		auto p = static_cast<Sprite*>(sp);

		if (sprite_frames_.size() > 0)
		{
			index_ = 0;
			timeline_ = 0.;
			is_end_ = false;
			last_frame_time_ = 0;
			p->SetSpriteFrame(sprite_frames_[0]);
		}
	}

	bool SpriteFrameAnimation::Update(float dt)
	{
		if (is_end_)
		{
			return false;
		}
		timeline_ += dt;

		if (timeline_ - last_frame_time_ > frame_time_)
		{
			last_frame_time_ = timeline_;
			index_ += 1;
			if (index_ == sprite_frames_.size())
			{
				if (is_loop_)
				{
					index_ = 0;
				}
				else
				{
					is_end_ = true;
					return false;
				}
			}
			static_cast<Sprite*>(sp_)->SetSpriteFrame(sprite_frames_[index_]);
		}
		return true;
	}
	std::shared_ptr<SpriteFrameAnimation> SpriteFrameAnimation::Create(const std::vector<std::shared_ptr<SpriteFrame>>& frames, float frame_time, bool is_loop)
	{
		return std::make_shared<SpriteFrameAnimation>(frames, frame_time, is_loop);
	}
}