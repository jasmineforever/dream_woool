#pragma once
#include "TTFInfo.h"
namespace DW
{

	class Font
	{
		std::string name;
	};
	class FontManager
	{
	public:
		static FontManager& GetInstance();

		~FontManager() = default;

		void AddTTFFont(const std::string& font_file);

		const std::string& GetFontFamilyName(const std::string& font_file) const;
	private:
		FontManager() = default;
		FontManager(const FontManager&) = delete;
		FontManager& operator =(const FontManager&) = delete;
	private:
		std::unordered_map<std::string, TTFInfo> fonts_;
	};
}