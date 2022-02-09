#pragma once
#include "MirMapData.h"
namespace DW
{
	class MirMapLoader
	{
	public:
		static MirMapLoader& GetInstance();
		bool LoadImageInfo(const std::string& wil_file, const std::string& wix_file);
		bool LoadMirMapFromFile(const std::string& map_file, MirMapData& out);
	private:
#pragma pack(1) 
		struct MirMapHeader
		{
			short	map_width;				//��ͼ��
			short	map_height;				//��ͼ��
			char	title[16];				//����
			char	last_update_time[8];	//������ʱ��
			char	map_type;				//�µ�ͼΪ2��ӦMirMapTileΪ14�ֽڣ�������Ӧ��MirMapTileΪ12�ֽ�
			char	reserved[23];			//�����ֽ�
		};
		struct MirMapTile
		{
			unsigned short tiles_image_index;				//�ر�����, ���λ��ʾ�Ƿ����ͨ��
			unsigned short smtiles_image_index;			//��ʾ��ͼ��������ͼƬ(��ʱ����Ϊ��������/�м��/С��ͼ��/��ͼ���䱳���ȵ�)
			unsigned short object_image_index;			//�ر��ϲ�����, ���λ��ʾ�Ƿ����ͨ��
			unsigned char door_index;			//�����������λΪ1��ʾ���ţ�Ϊ0��ʾû���š�
			unsigned char door_offset;			//��ƫ�ƣ����λΪ1��ʾ�Ŵ��ˣ�Ϊ0��ʾ��Ϊ�ر�״̬
			unsigned char animate_frame_num;	//��ͼ��֡��
			unsigned char animate_tick;			// ��֡����ָʾ��ǰ��ͼ�鶯̬����Ӧ��ÿ������֡�任��ǰ��ʾ�ľ�̬ͼƬ
			//index = (gAniCount % (btAniFrame * (1 + btAniTick))) / (1 + btAniTick)
			unsigned char object_file_index;		//��Դ�ļ�����
			unsigned char  light;					//һ��Ϊ0/4/5
			unsigned char big_tile_file_index;		// tiles�ļ� ����
			unsigned char smtiles_file_index;		//smtiles ����
		};
#pragma pack()
	private:
		MirMapLoader();
		~MirMapLoader();
		MirMapLoader(const MirMapLoader&) = delete;
		MirMapLoader& operator =(const MirMapLoader&) = delete;
		int GetFileIndex(const std::string& wil_file_name);
		bool LoadWixInfo(const std::string& wix_file, std::vector<int>& out);
	private:
		struct ImageInfoWrapper
		{
			int						size;
			MirMapData::ImageInfo*	data;
		};
		ImageInfoWrapper*  image_info_;
#define MIR_OBJECT_FILE_MAX_NUM 500
	};
}