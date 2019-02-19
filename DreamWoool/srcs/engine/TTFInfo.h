#pragma once
#include "pch.h"
//This is TTF file header
typedef struct _tagTT_OFFSET_TABLE {
	USHORT uMajorVersion;
	USHORT uMinorVersion;
	USHORT uNumOfTables;
	USHORT uSearchRange;
	USHORT uEntrySelector;
	USHORT uRangeShift;
}TT_OFFSET_TABLE;

//Tables in TTF file and there placement and name (tag)
typedef struct _tagTT_TABLE_DIRECTORY {
	char szTag[4]; //table name
	ULONG uCheckSum; //Check sum
	ULONG uOffset; //Offset from beginning of file
	ULONG uLength; //length of the table in bytes
}TT_TABLE_DIRECTORY;

//Header of names table
typedef struct _tagTT_NAME_TABLE_HEADER {
	USHORT uFSelector; //format selector. Always 0
	USHORT uNRCount; //Name Records count
	USHORT uStorageOffset; //Offset for strings storage, 
						   //from start of the table
}TT_NAME_TABLE_HEADER;

//Record in names table
typedef struct _tagTT_NAME_RECORD {
	USHORT uPlatformID;
	USHORT uEncodingID;
	USHORT uLanguageID;
	USHORT uNameID;
	USHORT uStringLength;
	USHORT uStringOffset; //from start of storage area
}TT_NAME_RECORD;

#define SWAPWORD(x) MAKEWORD(HIBYTE(x), LOBYTE(x))
#define SWAPLONG(x) MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))