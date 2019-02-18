#include <pch.h>
#include "Sprite.h"
#include "TextureCache.h"
#include "Director.h"

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
		return Sprite::Create(TextureCache::GetInstance().LoadFromFile(png_file_path));
	}

	std::shared_ptr<Sprite> Sprite::Create(std::shared_ptr<Texture> texture)
	{
		if (texture)
		{
			auto sprite = std::make_shared<Sprite>();
			sprite->InitGraphics();
			auto sf = SpriteFrame::Create(texture, Size(texture->width(), texture->height()), Vector2(0., 0.), false);
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
		SetContentSize(sprite_frame_->GetSize());
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
	void Sprite::InitVertices()
	{
		for (int i = 0; i < ARRAYSIZE(vertices_); ++i)
		{
			vertices_[i].color = { 1.0, 1.0, 1.0, 1.0 };
		}
		vertices_[0].textureCoordinate = { 0.0, 0.0 };
		vertices_[1].textureCoordinate = { 1.0, 0.0 };
		vertices_[2].textureCoordinate = { 0.0, 1.0 };
		vertices_[3].textureCoordinate = { 1.0, 1.0 };

		vertices_[0].position = { 0.0, 0.0, 0.0 };
		vertices_[1].position = { sprite_frame_->GetSize().width, 0.0, 0.0 };
		vertices_[2].position = { 0.0, sprite_frame_->GetSize().height, 0.0 };
		vertices_[3].position = { sprite_frame_->GetSize().width, sprite_frame_->GetSize().height, 0.0 };
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
	
		render.d3d_context->PSSetShaderResources(0, 1, sprite_frame_->GetTexture()->view().GetAddressOf());
		// set the primitive topology
		render.d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		render.d3d_context->DrawIndexed(6, 0, 0);
	}
}