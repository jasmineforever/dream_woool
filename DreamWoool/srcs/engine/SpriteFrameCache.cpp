#include "pch.h"
#include <fstream>
#include "SpriteFrameCache.h"
#include "TextureCache.h"
#include <regex>

namespace DW
{
	SpriteFrameCache& SpriteFrameCache::GetInstance()
	{
		static SpriteFrameCache cache;
		return cache;
	}

	void SpriteFrameCache::LoadFromFile(const std::string& sprite_sheet_file_name,
		const std::string& image_file_name)
	{
		
		//if(TextureCache::GetInstance().LoadFromFile(image_file_name))
	}

	bool SpriteFrameCache::ParseSpriteSheetFile(const std::string& sprite_sheet_file_name)
	{
		std::ifstream inFile(sprite_sheet_file_name);
		if (!inFile)
		{
			LOG_FATAL("load sprite sheet file failed:%s", sprite_sheet_file_name.c_str());
			return false;
		}
		char line[1024];
		int line_num = 1;
		
		for (;;)
		{
			inFile >> line;
			if (!inFile)
				break;

			if (std::regex_match(line, std::regex("[\n\r \t]*#")))
			{
				// Comment
			}
			else
			{
				std::vector<std::string> elements;
				FrameData frame_data;
				GetStringSplitBySemicolon(line, elements);
				if (elements.size() != 10)
				{
					LOG_FATAL("parse sprite sheet file line failed:%s, line:%d", sprite_sheet_file_name.c_str(), line_num);
					return false;
				}
				if (GetFrameData(elements, frame_data))
				{

				}
			}

			inFile.ignore(1000, '\n');
			line_num += 1;
		}
		return true;
	}
	bool SpriteFrameCache::GetFrameData(const std::vector<std::string> in, FrameData& out)
	{
		bool ret = false;
		do
		{
			if (in.size() != 10)
				break;
			if (in[FrameDataIndex::key].empty())
				break;
			out.key = in[FrameDataIndex::key];

			if (in[FrameDataIndex::rotated].empty())
				break;
			auto i_rotated = atoi(in[FrameDataIndex::rotated].c_str());
			if (i_rotated == 1)
			{
				out.rotated = true;
			}
			else
			{
				out.rotated = false;
			}

			if (in[FrameDataIndex::x].empty())
				break;
			auto x = atoi(in[FrameDataIndex::x].c_str());
			if (x < 0)
				break;
			out.x = x;

			if (in[FrameDataIndex::y].empty())
				break;
			auto y = atoi(in[FrameDataIndex::y].c_str());
			if (y < 0)
				break;
			out.y = y;

			if (in[FrameDataIndex::width].empty())
				break;
			auto width = atoi(in[FrameDataIndex::width].c_str());
			if (width <= 0)
				break;
			out.width = width;

			if (in[FrameDataIndex::height].empty())
				break;
			auto height = atoi(in[FrameDataIndex::height].c_str());
			if (height <= 0)
				break;
			out.height = height;

			if (in[FrameDataIndex::origin_width].empty())
				break;
			auto origin_width = atoi(in[FrameDataIndex::origin_width].c_str());
			if (origin_width <= 0)
				break;
			out.origin_width = origin_width;

			if (in[FrameDataIndex::origin_height].empty())
				break;
			auto origin_height = atoi(in[FrameDataIndex::origin_height].c_str());
			if (origin_height <= 0)
				break;
			out.origin_height = origin_height;

			if (in[FrameDataIndex::pivot_x].empty())
				break;
			auto pivot_x = atof(in[FrameDataIndex::pivot_x].c_str());
			out.pivot_x = static_cast<float>(pivot_x);

			if (in[FrameDataIndex::pivot_y].empty())
				break;
			auto pivot_y = atof(in[FrameDataIndex::pivot_y].c_str());
			out.pivot_y = static_cast<float>(pivot_y);

			ret = true;
		} while (0);
		return ret;
	}
	void SpriteFrameCache::GetStringSplitBySemicolon(const std::string& in_str, std::vector<std::string>& out)
	{
		out.clear();
		if (in_str.size() < 2)
		{
			return;
		}
		int offset = 0;
		do
		{
			if (offset >= in_str.size())
			{
				break;
			}
			size_t index = in_str.find(';', offset);
			if (index == std::string::npos)
			{
				if(in_str.size() > offset)
					out.push_back(in_str.substr(offset, in_str.size() - offset));
				break;
			}
			else
			{
				if(index > offset)
					out.push_back(in_str.substr(offset, index - offset));
				offset = index + 1;
			}
		} 
		while (1);
	}
	SpriteFrameCache::~SpriteFrameCache()
	{

	}
}
