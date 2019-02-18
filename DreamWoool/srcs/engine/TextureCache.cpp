#include <pch.h>
#include "TextureCache.h"
#include "WICTextureLoader.h"
#include "Director.h"
namespace DW
{
	TextureCache& TextureCache::GetInstance()
	{
		static TextureCache instance;
		return instance;
	}
	std::shared_ptr<Texture> TextureCache::GetTexture(const std::string& key)
	{
		auto it = cache_.find(key);
		if (it != cache_.end())
		{
			return it->second;
		}
		return nullptr;
	}
	std::shared_ptr<Texture> TextureCache::LoadFromFile(const std::string& file_path)
	{
		if (file_path.empty())
		{
			return nullptr;
		}
		auto texture = GetTexture(file_path);
		if (texture)
		{
			return texture;
		}
		auto d3d_device = Director::GetInstance().GetRender().d3d_device;
		if (!d3d_device)
		{
			return nullptr;
		}
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> t_src;
		if FAILED(CreateWICTextureFromFile(d3d_device, 
			StringUtil::CstrToWstr(file_path.c_str()).get(),
			resource.GetAddressOf(),
			t_src.GetAddressOf()))
		{
			return nullptr;
		}
		Microsoft::WRL::ComPtr<ID3D11Texture2D> t2d;
		DX::ThrowIfFailed(resource.As(&t2d));
		CD3D11_TEXTURE2D_DESC desc;
		t2d->GetDesc(&desc);
		texture = std::make_shared<Texture>();
		texture->width_ = desc.Width;
		texture->view_ = t_src;
		texture->height_ = desc.Height;
		cache_.insert(std::pair<std::string, std::shared_ptr<Texture>>(file_path, texture));
		return texture;
	}
}
