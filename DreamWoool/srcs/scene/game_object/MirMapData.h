#pragma once

namespace DW
{
	class MirMapData
	{
	public:
#pragma pack(1) 
		struct MirMapHeader
		{
			short	map_width;				//��ͼ��
			short	map_height;				//��ͼ��
			char	title[16];				//����
			char	last_update_time[8];	//������ʱ��
			char	map_type;				//�µ�ͼΪ2��ӦMirMapTileΪ14�ֽڣ�������Ӧ��MirMapTileΪ12�ֽ�
			char reserved[23];				//�����ֽ�
		};
		struct MirMapTile
		{
			short big_tile_image;				//�ر�����, ���λ��ʾ�Ƿ����ͨ��
			short small_title_image;			//��ʾ��ͼ��������ͼƬ(��ʱ����Ϊ��������/�м��/С��ͼ��/��ͼ���䱳���ȵ�)
			short object_tile_image;			//�ر��ϲ�����, ���λ��ʾ�Ƿ����ͨ��
			unsigned char door_index;			//�����������λΪ1��ʾ���ţ�Ϊ0��ʾû���š�
			unsigned char door_offset;			//��ƫ�ƣ����λΪ1��ʾ�Ŵ��ˣ�Ϊ0��ʾ��Ϊ�ر�״̬
			unsigned char animate_frame_num;	//��ͼ��֡��
			unsigned char animate_tick;			// ��֡����ָʾ��ǰ��ͼ�鶯̬����Ӧ��ÿ������֡�任��ǰ��ʾ�ľ�̬ͼƬ
			//index = (gAniCount % (btAniFrame * (1 + btAniTick))) / (1 + btAniTick)
			unsigned char object_file_index;		//��Դ�ļ�����
			unsigned char  light;					//һ��Ϊ0/4/5
			unsigned char big_tile_file_index;		// tiles�ļ� ����
			unsigned char small_title__file_index;	//smtiles ����
		};
#pragma pack()
		MirMapData();
		~MirMapData();
	};
}