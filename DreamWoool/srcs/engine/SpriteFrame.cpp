#include <pch.h>
#include "SpriteFrame.h"

namespace DW
{
	SpriteFrame::SpriteFrame(std::shared_ptr<Texture> texture,
		SimpleMath::Size size,
		SimpleMath::Vector2 origin,
		bool rotated):
		texture_(texture),
			size_(size),
			origin_(origin),
			rotated_(rotated)
	{
	}
	std::shared_ptr<Texture> SpriteFrame::GetTexture()
	{
		if (auto p = texture_.lock())
		{
			return p;
		}
		return nullptr;
	}
	SimpleMath::Size SpriteFrame::GetSize()
	{
		return size_;
	}
	std::shared_ptr<SpriteFrame> SpriteFrame::Create(std::shared_ptr<Texture> texture,
		SimpleMath::Size size,
		SimpleMath::Vector2 origin,
		bool rotated)
	{
		return std::make_shared<SpriteFrame>(texture, size, origin, rotated);
	}
	SpriteFrame::~SpriteFrame()
	{
	}
}
