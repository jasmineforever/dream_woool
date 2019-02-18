#pragma once
#include "Texture.h"

namespace DW
{
	//C++11 thread safe

	class TextureCache
	{
	public: 
		static TextureCache& GetInstance();

		std::shared_ptr<Texture> LoadFromFile(const std::string& file_path);

		void AsyncLoadFromFile(const std::string& file_path, std::function<void (std::shared_ptr<Texture>)> loaded_callback);
		
		std::shared_ptr<Texture> GetTexture(const std::string& key);

	private:
		TextureCache() = default;
		~TextureCache() = default;
		TextureCache(const TextureCache&) = delete;
		TextureCache& operator=(const TextureCache&) = delete;

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>>	cache_;
	};
}


