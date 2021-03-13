#pragma once
#include <FA2PP.h>
#include <Helper/CompileTime.h>

struct BuildingOptionValueEx
{
	char field_0;
	char gap_1[2];
	char field_3;
	CString Name;
	char field_8;
	char gap_9[1];
	__int16 field_A;
	__int16 FacingValue;
	char field_E;
	char field_F;
	CString field_10;
	CString field_14;
	CString field_18;
	char field_1C;
	char field_1D;
	__int16 word_1E;
};

struct SHPFrame
{
	int a;
	int field_4;
	char field_8;
	char gap_9[8];
	char unknown_type_11;
	char gap_12[5];
	char b;
};

struct BuildingFrameInfo
{
	int ptr_0;
	int ptr_4;
	SHPFrame Frame;
};

struct SHPStructCache
{
	BuildingFrameInfo Frames[8];
	char field_100;
	char gap_101[3];
	char ListIndex;
	char FoundationEnum;
	char gap_106[2];
	char isType1;
	char gap_109[3];
	char isType2;
	char gap_10D[3];
	char isType3;
	char gap_111[3];
};

constexpr reference<SHPStructCache, 0x627930, 276> SHPImageCache{};
