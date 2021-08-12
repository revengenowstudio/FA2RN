#pragma once
#include <FA2PP.h>
#include <Helper/CompileTime.h>

struct BuildingOptionValues
{
	FA2::CString House;
	FA2::CString TypeID;
	FA2::CString HealthRate;
	FA2::CString CellX;
	FA2::CString CellY;
	FA2::CString Facing;
	FA2::CString Tag;
	FA2::CString AISellable;
	FA2::CString AIRebuildable;
	FA2::CString PoweredOn;
	FA2::CString Upgrades;
	FA2::CString SpotLight;
	FA2::CString Upgrade1;
	FA2::CString Upgrade2;
	FA2::CString Upgrade3;
	FA2::CString AIRepairable;
	FA2::CString Nominal;
	char field_44;
	PROTECTED_PROPERTY(BYTE, gap45[2]);
	char b;
};

struct BuildingOptionValueEx
{
	char field_0;
	char gap_1[2];
	char field_3;
	FA2::CString Name;
	char field_8;
	char gap_9[1];
	__int16 field_A;
	__int16 FacingValue;
	char field_E;
	char field_F;
	FA2::CString field_10;
	FA2::CString field_14;
	FA2::CString field_18;
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
