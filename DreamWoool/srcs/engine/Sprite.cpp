#include <pch.h>
#include "Sprite.h"
#include "TextureCache.h"
#include "Director.h"
#include "SimpleMath.h"
namespace DW
{
	const uint16_t Sprite::indices_[6] = { 0, 1, 2, 2, 1, 3 };
	Sprite::Sprite():
		Node("")
	{
		SetAnchorPoint(Vector2(0.5, 0.5));
		SetNodeType(NodeType::SPRITE);
	}
	Sprite::~Sprite()
	{
	}
	std::shared_ptr<Sprite> Sprite::Create(const std::string& png_file_path)
	{
		return Sprite::Create(TextureCache::GetInstance().LoadFromFile(png_file_path, false));
	}
	std::shared_ptr<Sprite> Sprite::Create()
	{
		auto sprite = std::make_shared<Sprite>();
		sprite->InitGraphics();
		return sprite;
	}
	std::shared_ptr<Sprite> Sprite::Create(std::shared_ptr<Texture> texture)
	{
		if (texture)
		{
			auto sprite = std::make_shared<Sprite>();
			sprite->InitGraphics();
			auto sf = SpriteFrame::Create(texture, 
				SimpleMath::Rectangle(0, 0, texture->Width(), texture->Height()),
				SimpleMath::Size(texture->Width(), texture->Height()),
				SimpleMath::Vector2(0, 0), false);
			sprite->SetSpriteFrame(sf);
			return sprite;
		}
		return nullptr;
	}
	void Sprite::SetSpriteFrame(std::shared_ptr<SpriteFrame> sf)
	{
		if (sprite_frame_)
		{
			sprite_frame_.reset();
		}
		sprite_frame_ = sf;
		SetContentSize(sprite_frame_->GetOriginSize().width, sprite_frame_->GetOriginSize().height);
		InitVertices();
	}
	void Sprite::InitGraphics()
	{
		auto& render = Director::GetInstance().GetRender();
		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(VertexPositionColorTexture) * ARRAYSIZE(vertices_);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA vv = { vertices_, 0, 0 };
		render.d3d_device->CreateBuffer(&bd, &vv, vertex_buffer_.GetAddressOf());

		
		bd.ByteWidth = sizeof(uint16_t) * ARRAYSIZE(indices_);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA srd = { indices_, 0, 0 };
		render.d3d_device->CreateBuffer(&bd, &srd, index_buffer_.GetAddressOf());

		auto shader = render.GetSpriteDefaultVertexShaderCode();
		render.d3d_device->CreateInputLayout(VertexPositionColorTexture::InputElements,
			VertexPositionColorTexture::InputElementCount,
			shader.code,
			shader.size,
			inputlayout_.GetAddressOf());
	}

	void Sprite::SetColor(const DWColor& color)
	{
		color_ = color;
	}
	void Sprite::InitVertices()
	{
		for (int i = 0; i < ARRAYSIZE(vertices_); ++i)
		{
			//vertices_[i].color.x = color_.GetFloatColor().f[0];
			//vertices_[i].color.y = color_.GetFloatColor().f[1];
			//vertices_[i].color.z = color_.GetFloatColor().f[2];
			//vertices_[i].color.w = color_.GetFloatColor().f[3];

			memcpy(&vertices_[i].color, color_.GetFloatColor().f, 4 * sizeof(float));
		}
		auto src_rect = sprite_frame_->GetSourceRect();
		
		if (sprite_frame_->Rotated())
		{
			auto texture_size = Size(sprite_frame_->GetTexture()->Width(), sprite_frame_->GetTexture()->Height());
			vertices_[2].textureCoordinate = { static_cast<float>(src_rect.x) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y) / static_cast<float>(texture_size.height) };
			vertices_[0].textureCoordinate = { static_cast<float>(src_rect.x + src_rect.width) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y) / static_cast<float>(texture_size.height) };
			vertices_[3].textureCoordinate = { static_cast<float>(src_rect.x) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y + src_rect.height) / static_cast<float>(texture_size.height) };
			vertices_[1].textureCoordinate = { static_cast<float>(src_rect.x + src_rect.width) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y + src_rect.height) / static_cast<float>(texture_size.height) };
		}
		else
		{
			auto texture_size = Size(sprite_frame_->GetTexture()->Width(), sprite_frame_->GetTexture()->Height());
			vertices_[0].textureCoordinate = { static_cast<float>(src_rect.x) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y) / static_cast<float>(texture_size.height) };
			vertices_[1].textureCoordinate = { static_cast<float>(src_rect.x + src_rect.width) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y) / static_cast<float>(texture_size.height) };
			vertices_[2].textureCoordinate = { static_cast<float>(src_rect.x) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y + src_rect.height) / static_cast<float>(texture_size.height) };
			vertices_[3].textureCoordinate = { static_cast<float>(src_rect.x + src_rect.width) / static_cast<float>(texture_size.width),
				static_cast<float>(src_rect.y + src_rect.height) / static_cast<float>(texture_size.height) };
		}
		
		//auto& origin_size = sprite_frame_->GetOriginSize();
		auto offset = sprite_frame_->GetOffset();
		if (sprite_frame_->Rotated())
		{
			vertices_[0].position = { offset.x, offset.y, 0.0 };
			vertices_[1].position = { offset.x + src_rect.height, offset.y, 0.0 };
			vertices_[2].position = { offset.x, offset.y + src_rect.width, 0.0 };
			vertices_[3].position = { offset.x + src_rect.height, offset.y + src_rect.width, 0.0 };
		}
		else
		{
			vertices_[0].position = { offset.x, offset.y, 0.0 };
			vertices_[1].position = { offset.x + src_rect.width, offset.y, 0.0 };
			vertices_[2].position = { offset.x, offset.y + src_rect.height, 0.0 };
			vertices_[3].position = { offset.x + src_rect.width, offset.y + src_rect.height, 0.0 };
		}
	}
	void Sprite::Draw()
	{
		if (!sprite_frame_)
		{
			return;
		}
		auto& render = Director::GetInstance().GetRender();
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		render.d3d_context->IASetVertexBuffers(0, 1, vertex_buffer_.GetAddressOf(), &stride, &offset);
		render.d3d_context->IASetIndexBuffer(index_buffer_.Get(), DXGI_FORMAT_R16_UINT, 0);
		render.d3d_context->IASetInputLayout(inputlayout_.Get());
		
		D3D11_MAPPED_SUBRESOURCE ms;
		render.d3d_context->Map(vertex_buffer_.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices_, sizeof(vertices_));
		render.d3d_context->Unmap(vertex_buffer_.Get(), NULL);

		render.d3d_context->Map(index_buffer_.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms); 
		memcpy(ms.pData, indices_, sizeof(indices_));
		render.d3d_context->Unmap(index_buffer_.Get(), NULL);
	
		render.d3d_context->PSSetShaderResources(0, 1, sprite_frame_->GetTexture()->View().GetAddressOf());
		// set the primitive topology
		render.d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		render.d3d_context->DrawIndexed(6, 0, 0);
	}

	void Sprite::RunSpriteFrameAnimation(std::shared_ptr<SpriteFrameAnimation> animation)
	{
		if (sprite_animation_)
		{
			sprite_animation_.reset();
		}
		sprite_animation_ = animation;
		sprite_animation_->SetSprite(this);
	}

	void Sprite::Update(float dt)
	{
		if (sprite_animation_)
		{
			if (!sprite_animation_->Update(dt))
			{
				sprite_animation_.reset();
			}
		}
	}
}
