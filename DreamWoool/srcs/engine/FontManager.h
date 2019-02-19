#pragma once

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

		void AddTTFFont(const std::string& font);
	private:
		FontManager() = delete;
		FontManager(const FontManager&) = delete;
		FontManager& operator =(const FontManager&) = delete;
	};
}