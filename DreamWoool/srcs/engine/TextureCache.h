#pragma once
#include "Texture.h"

namespace DW
{
	//C++11 thread safe

	class TextureCache
	{
	public: 
		static TextureCache& GetInstance();

		std::shared_ptr<Texture> LoadFromFile(const std::string& file_path, bool need_cache = true);

		void AsyncLoadFromFile(const std::string& file_path, std::function<void (std::shared_ptr<Texture>)> loaded_callback, bool need_cache = true);
		
		std::shared_ptr<Texture> GetTexture(const std::string& key);

		void ClearNotUsedTempCache();
	private:
		TextureCache() = default;

		~TextureCache() = default;

		TextureCache(const TextureCache&) = delete;

		TextureCache& operator=(const TextureCache&) = delete;

		std::shared_ptr<Texture> GetTexture(const std::string& key, bool need_cache);
	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>>	managed_cache_;
		std::unordered_map<std::string, std::weak_ptr<Texture>>		temp_cache_;
	};
}


