#include <pch.h>
#include "SpriteFrame.h"

namespace DW
{
	SpriteFrame::SpriteFrame(std::shared_ptr<Texture> texture,
		const SimpleMath::Rectangle& source_rect,
		const SimpleMath::Size& origin_size,
		const SimpleMath::Vector2& offset,
		bool rotated):
			texture_(texture),
			src_rect_(source_rect),
			origin_size_(origin_size),
			offset_(offset),
			rotated_(rotated)
	{
	}
	std::shared_ptr<Texture> SpriteFrame::GetTexture()
	{
		return texture_;
	}
	const SimpleMath::Rectangle SpriteFrame::GetSourceRect() const
	{
		return src_rect_;
	}

	const SimpleMath::Size SpriteFrame::GetOriginSize() const
	{
		return origin_size_;
	}

	const SimpleMath::Vector2 SpriteFrame::GetOffset() const
	{
		return offset_;
	}

	bool SpriteFrame::Rotated()
	{
		return rotated_;
	}

	std::shared_ptr<SpriteFrame> SpriteFrame::Create(std::shared_ptr<Texture> texture,
		const SimpleMath::Rectangle& source_rect,
		const SimpleMath::Size& origin_size,
		const SimpleMath::Vector2& offset,
		bool rotated)
	{
		return std::make_shared<SpriteFrame>(texture, source_rect, origin_size, offset, rotated);
	}
	SpriteFrame::~SpriteFrame()
	{
	}
}
