#include "pch.h"
#include "FontManager.h"
namespace DW
{
	FontManager& FontManager::GetInstance()
	{
		static FontManager manager;
		return manager;
	}
	const std::string& FontManager::GetFontFamilyName(const std::string& font_file) const
	{
		if (!font_file.empty())
		{
			auto it = fonts_.find(font_file);
			if (it == fonts_.end())
			{
				GetInstance().AddTTFFont(font_file);
			}
			it = fonts_.find(font_file);
			if (it != fonts_.end())
			{
				return it->second.GetZhFontInfo(TTFInfo::PropertyIndex::family_name);
			}
		}
		return TTFInfo::s_empty_string;
	}
	void FontManager::AddTTFFont(const std::string& font_file)
	{
		if (!font_file.empty())
		{
			auto it = fonts_.find(font_file);

			if (it == fonts_.end())
			{
				TTFInfo info;
				if (!TTFInfo::ParseTTFInfo(font_file, info))
				{
					LOG_WARNING("load font info failed:%s", font_file.c_str());
					return;
				}
				if (AddFontResource(StringUtil::CstrToWstr(font_file).c_str()) == 0)
				{
					LOG_WARNING("AddFontResource failed:%s", font_file.c_str());
				}
				fonts_.insert(std::pair<std::string, TTFInfo>(font_file, info));
			}
		}
		
	}
}