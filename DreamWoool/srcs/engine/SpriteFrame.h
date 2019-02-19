#pragma once
#include "Texture.h"
#include "SimpleMath.h"
namespace DW
{
	class SpriteFrame
	{
	public:
		SpriteFrame(std::shared_ptr<Texture> texture,
					const SimpleMath::Rectangle& source_rect,
					const SimpleMath::Size& origin_size,
					const SimpleMath::Vector2& offset,
					bool rotated);

		~SpriteFrame();

		static std::shared_ptr<SpriteFrame> Create(std::shared_ptr<Texture> texture,
			const SimpleMath::Rectangle& source_rect,
			const SimpleMath::Size& origin_size,
			const SimpleMath::Vector2& offset,
			bool rotated);

		const SimpleMath::Rectangle GetSourceRect() const;

		const SimpleMath::Size GetOriginSize() const;

		const SimpleMath::Vector2 GetOffset() const;

		bool Rotated();

		std::shared_ptr<Texture> GetTexture();
	private:
		std::shared_ptr<Texture>	texture_;
		SimpleMath::Size			origin_size_;
		SimpleMath::Vector2			offset_;
		SimpleMath::Rectangle		src_rect_;
		bool						rotated_;
	};
}


