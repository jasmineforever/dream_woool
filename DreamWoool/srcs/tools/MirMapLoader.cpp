#include "pch.h"
#include "MirMapLoader.h"

namespace DW
{
	MirMapLoader& MirMapLoader::GetInstance()
	{
		static MirMapLoader loader;
		return loader;
	}
	MirMapLoader::MirMapLoader():
		image_info_(new ImageInfoWrapper[MIR_OBJECT_FILE_MAX_NUM])
	{
		memset(image_info_, 0, MIR_OBJECT_FILE_MAX_NUM * sizeof(ImageInfoWrapper));
	}
	MirMapLoader::~MirMapLoader()
	{
		for (int i = 0; i < MIR_OBJECT_FILE_MAX_NUM; i++)
		{
			if (image_info_[i].data)
			{
				delete [] image_info_[i].data;
			}
		}
		delete [] image_info_;
	}
	int MirMapLoader::GetFileIndex(const std::string& wil_file_name)
	{
		int ret = -1;
		auto point_index = wil_file_name.rfind('.');
		if (point_index != std::string::npos)
		{
			auto split_index = wil_file_name.rfind('/');
			if (split_index == std::string::npos)
			{
				split_index = wil_file_name.rfind('\\');
			}
			if (split_index != std::string::npos && split_index < point_index)
			{
				int begin_index = split_index + 1;
				for (int i = point_index - 1; i > split_index; i--)
				{
					if (wil_file_name[i] < '0' || wil_file_name[i] > '9')
					{
						begin_index = i + 1;
						break;
					}
				}

				auto str = wil_file_name.substr(begin_index, point_index - begin_index);

				ret = atoi(str.c_str()) - 1;
			}
		}
		return ret;
	}
	bool MirMapLoader::LoadWixInfo(const std::string& wix_file, std::vector<int>& out)
	{
		bool ret = false;
		FILE* fp = NULL;
		out.clear();
		fopen_s(&fp, wix_file.c_str(), "rb");
		if (fp)
		{
			int count = 0;
			fseek(fp, 44, SEEK_SET);
			fread(&count, 4, 1, fp);
			fseek(fp, 0L, SEEK_END);
			int size = ftell(fp) - 48;
			fseek(fp, 48L, SEEK_SET);
			if (size == 4 * count)
			{
				int v;
				for (int i = 0; i < count; i++)
				{
					fread(&v, 4, 1, fp);
					out.push_back(v);
				}
				ret = true;
			}
			fclose(fp);
		}
		return ret;
	}
	bool MirMapLoader::LoadImageInfo(const std::string& wil_file, const std::string& wix_file)
	{
		bool ret = false;
		if (!wil_file.empty() && !wix_file.empty())
		{
			int index = GetFileIndex(wil_file);

			if (index >=0 && index < MIR_OBJECT_FILE_MAX_NUM && !image_info_[index].data)
			{
				std::vector<int> wil_positions;
				if (LoadWixInfo(wix_file, wil_positions))
				{
					FILE* fp = NULL;
					fopen_s(&fp, wil_file.c_str(), "rb");
					if (fp)
					{
						fseek(fp, 44, SEEK_SET);
						int image_count = 0;
						fread(&image_count, 4, 1, fp);
						if (image_count == wil_positions.size())
						{
							image_info_[index].data = new MirMapData::ImageInfo[image_count];
							image_info_[index].size = image_count;
							for (int i = 0; i < image_count; i++)
							{
								fseek(fp, wil_positions[i], SEEK_SET);
								fread(&image_info_[index].data[i], sizeof(MirMapData::ImageInfo), 1, fp);
							}
							ret = true;
						}
						fclose(fp);
					}
				}
			}
			else
			{
				ret = true;
			}
		}
		return ret;
	}
	/*bool MirMapLoader::LoadOffsetFromFile(const std::string& wil_file, const std::string& wix_file)
	{
		std::vector<int> index;
		{
			FILE* fp_x = NULL;
			fopen_s(&fp_x, wix_file.c_str(), "rb");
			fseek(fp_x, 44, SEEK_SET);
			int count;
			fread(&count, 4, 1, fp_x);
			fseek(fp_x, 0, SEEK_END);
			int size = ftell(fp_x) - 48;
			fseek(fp_x, 48, SEEK_SET);
			if (size == count * 4)
			{
				int v = 0;
				for (int i = 0; i < count; i++)
				{
					fread(&v, 4, 1, fp_x);
					index.push_back(v);
				}
			}
			fclose(fp_x);
		}
		bool ret = false;
		FILE* fp = NULL;
		fopen_s(&fp, wil_file.c_str(), "rb");
		fseek(fp, 44, SEEK_SET);
		int image_count = 0;
		int color_count = 0;
		int palette_size = 0;
		fread(&image_count, 4, 1, fp);
		fread(&color_count, 4, 1, fp);
		fread(&palette_size, 4, 1, fp);
		fseek(fp, palette_size, SEEK_CUR);
		for (int i = 0; i < image_count; i++)
		{
			fseek(fp, index[i], SEEK_SET);
			unsigned short width;
			unsigned short height;
			unsigned short offset_x;
			unsigned short offset_y;
			fread(&width, 2, 1, fp);
			fread(&height, 2, 1, fp);
			fread(&offset_x, 2, 1, fp);
			fread(&offset_y, 2, 1, fp);
			LOG_INFO("wdith:%3d,height:%3d,offset:%3d,%3d", width, height, offset_x, offset_y);
		}
		fclose(fp);
		return ret;
	}*/
	bool MirMapLoader::LoadMirMapFromFile(const std::string& map_file, MirMapData& out)
	{
		bool ret = false;
		FILE* fp = NULL;
		fopen_s(&fp, map_file.c_str(), "rb");
		out.tiles_info_.clear();
		MirMapHeader header;

		if (fp)
		{
			int header_size = sizeof(MirMapHeader);
			fseek(fp, 0L, SEEK_END);
			int tiles_block_size = ftell(fp) - header_size;
			fseek(fp, 0L, SEEK_SET);
			fread(&header, 1, sizeof(header), fp);
			int tile_size = tiles_block_size / (header.map_width * header.map_height);

			out.map_width_ = header.map_width;
			out.map_height_ = header.map_height;
			if (tile_size == 12 || tile_size == 14)
			{
				int tile_count = header.map_width * header.map_height;
				for (int i = 0; i < tile_count; i++)
				{
					MirMapTile tile;
					MirMapData::TileInfo tile_info = {};
					if (tile_size != fread(&tile, 1, tile_size, fp))
					{
						fclose(fp);
						LOG_FATAL("load mir tile err");
						return ret;
					}
					tile_info.can_move = ((tile.object_image_index >> 15) == 0 && (tile.tiles_image_index >> 15) == 0);
					tile_info.tiles_image_index = tile.tiles_image_index & 0x7FFF;
					tile_info.object_image_index = tile.object_image_index & 0x7FFF;
					tile_info.smtiles_image_index = tile.smtiles_image_index;

					if (tile_info.object_image_index >= 0)
					{
						int obj_file_index = tile.object_file_index;
						if (obj_file_index >= 0 && obj_file_index < MIR_OBJECT_FILE_MAX_NUM &&
							image_info_[obj_file_index].data != nullptr &&
							tile_info.object_file_index < image_info_[obj_file_index].size)
						{
							tile_info.object_file_index = obj_file_index;
							tile_info.image_info = image_info_[obj_file_index].data[tile_info.object_image_index];
							out.tiles_info_.push_back(tile_info);
						}
					}
				}
				ret = true;
			}
			else
			{
				LOG_FATAL("unsupport mir map, tile_size:%d,map file:%s", tile_size, map_file.c_str());
			}
			fclose(fp);
		}
		return ret;
	}

	/*void MirMapLoader::PrintMapInfo(const MirMapHeader& header, const std::vector<MirMapTile>& tiles)
	{
		LOG_INFO("map width:%d, height:%d", header.map_width, header.map_height);
		for (int i = 0; i < tiles.size(); i++)
		{
			unsigned short big = (tiles[i].big_tile_image & 0x7FFF);
			int big_can_move = tiles[i].big_tile_image >> 15;

			unsigned short obj = (tiles[i].object_tile_image & 0x7FFF);
			int obj_can_move = tiles[i].object_tile_image >> 15;

			unsigned char door_index = (tiles[i].door_index & 0x7F);
			int have_door = tiles[i].door_index >> 7;

			if (tiles[i].animate_frame_num > 0)
			{
				LOG_INFO("x:%3d,y:%3d,obj index:%5d, can_move:%d,small id:%5d", obj, obj_can_move, tiles[i].small_title_image);
			}
		}
	}*/
}