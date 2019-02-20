#pragma once

namespace DW
{
	class TTFInfo
	{
	public:
		TTFInfo() = default;
		~TTFInfo() = default;
		enum PropertyIndex
		{
			copyright = 0,
			family_name,
			subfamily_name,
			unique_id,
			full_name,
			version,
			postscript_name,
			trademark,
			manufacturer,
			designer,
			description,
			url_vendor,
			url_designer,
			license_description,
			license_info_url
		};
		const std::string& GetZhFontInfo(PropertyIndex index) const; //zh-cn
		const std::string& GetZhTwFontInfo(PropertyIndex index) const;//zh-tw
		const std::string& GetEnFontInfo(PropertyIndex index) const;//en-us
		static bool ParseTTFInfo(const std::string& ttf_file_path, TTFInfo& out);
	private:
		void Clear();
		static void BigEndianToLittleEndian(wchar_t* in_buf, unsigned int size);
		const std::string& FindInMap(const std::unordered_map<PropertyIndex, std::string>& map, PropertyIndex index) const;
	private:
		static const std::string s_empty_string;
		std::unordered_map<PropertyIndex, std::string> zh_cn_map_;
		std::unordered_map<PropertyIndex, std::string> zh_tw_map_;
		std::unordered_map<PropertyIndex, std::string> en_us_map_;
	};
}