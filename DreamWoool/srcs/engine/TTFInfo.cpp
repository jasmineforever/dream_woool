#include "pch.h"
#include "TTFInfo.h"
namespace DW
{
	const std::string TTFInfo::s_empty_string;
	const std::string& TTFInfo::GetZhFontInfo(PropertyIndex index) const
	{
		return FindInMap(zh_cn_map_, index);
	}

	const std::string& TTFInfo::GetZhTwFontInfo(PropertyIndex index) const
	{
		return FindInMap(zh_tw_map_, index);
	}

	const std::string& TTFInfo::GetEnFontInfo(PropertyIndex index) const
	{
		return FindInMap(en_us_map_, index);
	}

	const std::string& TTFInfo::FindInMap(const std::unordered_map<PropertyIndex, std::string>& map, PropertyIndex index) const
	{
		auto it = map.find(index);
		if (it != map.end())
		{
			return it->second;
		}
		return s_empty_string;
	}

	void TTFInfo::Clear()
	{
		zh_cn_map_.clear();
		zh_tw_map_.clear();
		en_us_map_.clear();
	}
	//==========================================================================================
	struct TT_OFFSET_TABLE 
	{
		USHORT    uMajorVersion;
		USHORT    uMinorVersion;
		USHORT    uNumOfTables;
		USHORT    uSearchRange;
		USHORT    uEntrySelector;
		USHORT    uRangeShift;
	};

	struct TT_TABLE_DIRECTORY 
	{
		char    szTag[4];            //table name
		ULONG    uCheckSum;            //Check sum
		ULONG    uOffset;            //Offset from beginning of file
		ULONG    uLength;            //length of the table in bytes
	};

	struct TT_NAME_TABLE_HEADER 
	{
		USHORT    uFSelector;            //format selector. Always 0
		USHORT    uNRCount;            //Name Records count
		USHORT    uStorageOffset;        //Offset for strings storage, from start of the table
	};

	struct TT_NAME_RECORD 
	{
		USHORT    uPlatformID;
		USHORT    uEncodingID;
		USHORT    uLanguageID;
		USHORT    uNameID;
		USHORT    uStringLength;
		USHORT    uStringOffset;    //from start of storage area
	};
#define MAKEWORD_LOCAL(a, b) (((a & 0xff) << 8) | (b >> 8))
#define MAKELONG_LOCAL(a, b) (((a & 0xff) << 24) | ((a & 0xff00) << 8) | ((b & 0xff0000) >> 8) | (b >> 24))

#define SWAPWORD_LOCAL(x)        MAKEWORD_LOCAL(x,x)
#define SWAPLONG_LOCAL(x)        MAKELONG_LOCAL(x,x)

	void TTFInfo::BigEndianToLittleEndian(wchar_t* in_buf, unsigned int size)
	{
		for (int i = 0; i < size; i++)
		{
			in_buf[i] = SWAPWORD_LOCAL(in_buf[i]);
		}
	}
	bool TTFInfo::ParseTTFInfo(const std::string& ttf_file_path, TTFInfo& out)
	{
		FILE *fp = nullptr;
		int font_name_len = 0;
		fopen_s(&fp, ttf_file_path.c_str(), "rb");
		if (NULL == fp)
		{
			return false;
		}
		out.Clear();
		TT_OFFSET_TABLE ttOffsetTable;
		fread(&ttOffsetTable, sizeof(TT_OFFSET_TABLE), 1, fp);
		ttOffsetTable.uNumOfTables = SWAPWORD_LOCAL(ttOffsetTable.uNumOfTables);
		ttOffsetTable.uMajorVersion = SWAPWORD_LOCAL(ttOffsetTable.uMajorVersion);
		ttOffsetTable.uMinorVersion = SWAPWORD_LOCAL(ttOffsetTable.uMinorVersion);

		//check is this is a true type font and the version is 1.0
		if (ttOffsetTable.uMajorVersion != 1 || ttOffsetTable.uMinorVersion != 0)
		{
			fclose(fp);
			return false;
		}

		TT_TABLE_DIRECTORY tblDir;
		bool bFound = false;
		char csTemp[256] = { 0 };

		for (int i = 0; i < ttOffsetTable.uNumOfTables; i++)
		{
			fread(&tblDir, sizeof(TT_TABLE_DIRECTORY), 1, fp);
			strncpy_s(csTemp, tblDir.szTag, 4);
			if (0 == strcmp(csTemp, "name"))
			{
				bFound = true;
				tblDir.uLength = SWAPLONG_LOCAL(tblDir.uLength);
				tblDir.uOffset = SWAPLONG_LOCAL(tblDir.uOffset);
				break;
			}
		}

		if (!bFound)
		{
			fclose(fp);
			return false;
		}
		
		fseek(fp, tblDir.uOffset, SEEK_SET);

		TT_NAME_TABLE_HEADER ttNTHeader;
		fread(&ttNTHeader, sizeof(TT_NAME_TABLE_HEADER), 1, fp);
		ttNTHeader.uNRCount = SWAPWORD_LOCAL(ttNTHeader.uNRCount);
		ttNTHeader.uStorageOffset = SWAPWORD_LOCAL(ttNTHeader.uStorageOffset);
		TT_NAME_RECORD ttRecord;

		for (int i = 0; i < ttNTHeader.uNRCount; i++)
		{
			fread(&ttRecord, sizeof(TT_NAME_RECORD), 1, fp);

			ttRecord.uNameID = SWAPWORD_LOCAL(ttRecord.uNameID);
			ttRecord.uLanguageID = SWAPWORD_LOCAL(ttRecord.uLanguageID);
			ttRecord.uEncodingID = SWAPWORD_LOCAL(ttRecord.uEncodingID);
			ttRecord.uStringLength = SWAPWORD_LOCAL(ttRecord.uStringLength);
			ttRecord.uStringOffset = SWAPWORD_LOCAL(ttRecord.uStringOffset);
			ttRecord.uPlatformID = SWAPWORD_LOCAL(ttRecord.uPlatformID);
			long nPos = ftell(fp);
			fseek(fp, tblDir.uOffset + ttRecord.uStringOffset + ttNTHeader.uStorageOffset, SEEK_SET);
			int font_name_buf_size = ttRecord.uStringLength + 2;
			char* font_name_buf = (char*)malloc(font_name_buf_size);
			memset(font_name_buf, 0, font_name_buf_size);
			fread(font_name_buf, 1, ttRecord.uStringLength, fp);
			fseek(fp, nPos, SEEK_SET);

			if (2052 == ttRecord.uLanguageID)
			{
				wchar_t *p = (wchar_t*)font_name_buf;
				BigEndianToLittleEndian(p, ttRecord.uStringLength / 2);
				out.zh_cn_map_.insert(std::pair<PropertyIndex, std::string>(static_cast<PropertyIndex>(ttRecord.uNameID), StringUtil::WstrToCstr(p).get()));
				
			}
			else if (1028 == ttRecord.uLanguageID)
			{
				wchar_t *p = (wchar_t*)font_name_buf;
				BigEndianToLittleEndian(p, ttRecord.uStringLength / 2);
				out.zh_tw_map_.insert(std::pair<PropertyIndex, std::string>(static_cast<PropertyIndex>(ttRecord.uNameID), StringUtil::WstrToCstr(p).get()));
			}
			else if (0 == ttRecord.uLanguageID)
			{
				out.en_us_map_.insert(std::pair<PropertyIndex, std::string>(static_cast<PropertyIndex>(ttRecord.uNameID), font_name_buf));
			}
			free(font_name_buf);
		}
		return true;
	}
}