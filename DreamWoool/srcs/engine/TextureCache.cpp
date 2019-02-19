#include "pch.h"
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
	void TextureCache::ClearNotUsedTempCache()
	{

	}
	std::shared_ptr<Texture> TextureCache::GetTexture(const std::string& key, bool need_cache)
	{
		//4种情况
		//1.请求的是需要缓存的贴图，贴图在管理缓存中：返回管理缓存中的数据
		//2.请求的是需要缓存的贴图，贴图在在临时缓存中：将临时缓存中数据放入管理缓存后返回
		//3.请求的是不需要缓存的贴图，贴图在管理缓存中：返回管理缓存中的数据
		//4.请求的是不需要缓存的贴图，贴图在临时缓存中：返回临时缓存中的数据
		if (!key.empty())
		{
			auto m_it = managed_cache_.find(key);
			if (m_it != managed_cache_.end())
			{
				return m_it->second;
			}
			auto t_it = temp_cache_.find(key);
			if (t_it != temp_cache_.end())
			{
				auto shared_p = t_it->second.lock();
				if (shared_p)
				{
					if (need_cache)
					{
						managed_cache_.insert(std::pair<std::string, std::shared_ptr<Texture>>(key, shared_p));
					}
				}
				if (need_cache || !shared_p)
				{
					temp_cache_.erase(t_it);
				}
				return shared_p;
			}
		}
		return nullptr;
	}
	std::shared_ptr<Texture> TextureCache::GetTexture(const std::string& key)
	{
		auto m_it = managed_cache_.find(key);
		if (m_it != managed_cache_.end())
		{
			return m_it->second;
		}
		return nullptr;
	}
	std::shared_ptr<Texture> TextureCache::LoadFromFile(const std::string& file_path, bool need_cache)
	{
		if (file_path.empty())
		{
			return nullptr;
		}
		auto texture = GetTexture(file_path, need_cache);
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
			LOG_FATAL("load texture failed:%s", file_path.c_str());
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
		if (need_cache)
		{
			temp_cache_.insert(std::pair<std::string, std::weak_ptr<Texture>>(file_path, texture));
		}
		else
		{
			managed_cache_.insert(std::pair<std::string, std::shared_ptr<Texture>>(file_path, texture));
		}
		return texture;
	}
}
