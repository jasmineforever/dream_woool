#pragma once
#include "Texture.h"
#include "SimpleMath.h"
namespace DW
{
	class SpriteFrame
	{
	public:
		SpriteFrame(std::shared_ptr<Texture> texture,
					SimpleMath::Size size,
					SimpleMath::Vector2 origin,
					bool rotated);
		~SpriteFrame();
		static std::shared_ptr<SpriteFrame> Create(std::shared_ptr<Texture> texture,
			SimpleMath::Size size,
			SimpleMath::Vector2 origin,
			bool rotated);
		SimpleMath::Size GetSize();
		std::shared_ptr<Texture> GetTexture();
	private:
		std::weak_ptr<Texture>	texture_;
		SimpleMath::Size		size_;
		SimpleMath::Vector2		origin_;
		SimpleMath::Vector2		offset_;
		bool					rotated_;
	};
}


