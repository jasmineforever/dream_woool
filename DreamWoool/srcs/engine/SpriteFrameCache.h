#pragma once
#include "SpriteFrame.h"
namespace DW
{
	class SpriteFrameCache
	{
	public:
		static SpriteFrameCache& GetInstance();

		void AsyncLoadFromFile(const std::string& sprite_sheet_file_name,
			const std::string& image_file_name, std::function<void(bool)> loaded_callback);

		void LoadFromFile(const std::string& sprite_sheet_file_name,
			const std::string& image_file_name);

		~SpriteFrameCache();

		bool ParseSpriteSheetFile(const std::string& sprite_sheet_file_name);
	private:
		SpriteFrameCache() = default;
		SpriteFrameCache(const SpriteFrameCache&) = delete;
		SpriteFrameCache& operator=(const SpriteFrameCache&) = delete;
		
		struct FrameData
		{
			std::string key;
			bool rotated;
			int x;
			int y;
			int width;
			int height;
			int origin_width;
			int origin_height;
			float pivot_x;
			float pivot_y;
		};
		enum FrameDataIndex
		{
			key = 0,
			rotated,
			x,
			y,
			width,
			height,
			origin_width,
			origin_height,
			pivot_x,
			pivot_y
		};
		//bool ParseSpriteSheetFile(const std::string& sprite_sheet_file_name);

		bool GetFrameData(const std::vector<std::string> in, FrameData& out);

		void GetStringSplitBySemicolon(const std::string& in_str, std::vector<std::string>& out);
	private:
		std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> cache_;
	};
}


