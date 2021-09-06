#pragma once

#include <CLoading.h>
#include "../FA2Expand.h"

#include <vector>
#include <map>

class ImageDataClass;
class Palette;

class NOVTABLE CLoadingExt : public CLoading
{
public:

	//hook function to replace in virtual function map
	//BOOL PreTranslateMessageExt(MSG* pMsg);

	//static void ProgramStartupInit();

	CLoadingExt() {};
	~CLoadingExt() {};

	void LoadObjects(const FA2::CString& pRegName);
	static FA2::CString GetImageName(const FA2::CString& ID, int nFacing);
	static void ClearItemTypes();
private:
	void GetFullPaletteName(FA2::CString& PaletteName);
	static FA2::CString* __cdecl GetDictName(FA2::CString* ret, const char* ID, int nFacing) { JMP_STD(0x475450); }
	static FA2::CString GetDictName(const FA2::CString& ID, int nFacing)
	{
		FA2::CString buffer;
		GetDictName(&buffer, ID, nFacing);
		return buffer;
	}

	void LoadBuilding(const FA2::CString& ID);
	void LoadInfantry(const FA2::CString& ID);
	void LoadTerrainOrSmudge(const FA2::CString& ID);
	void LoadVehicleOrAircraft(const FA2::CString& ID);

	void SetImageData(unsigned char* pBuffer, const FA2::CString& NameInDict, int FullWidth, int FullHeight, Palette* pPal);
	void SetImageData(unsigned char* pBuffer, ImageDataClass* pData, int FullWidth, int FullHeight, Palette* pPal);
	void ShrinkSHP(unsigned char* pIn, int InWidth, int InHeight, unsigned char*& pOut, int* OutWidth, int* OutHeight);
	void UnionSHP_Add(unsigned char* pBuffer, int Width, int Height, int DeltaX = 0, int DeltaY = 0, bool UseTemp = false);
	void UnionSHP_GetAndClear(unsigned char*& pOutBuffer, int* OutWidth, int* OutHeight, bool UseTemp = false);
	void VXL_Add(unsigned char* pCache, int X, int Y, int Width, int Height);
	void VXL_GetAndClear(unsigned char*& pBuffer, int* OutWidth, int* OutHeight);

	void SetValidBuffer(ImageDataClass* pData, int Width, int Height);

	enum class ObjectType
	{
		Unknown = -1,
		Infantry = 0,
		Vehicle = 1,
		Aircraft = 2,
		Building = 3,
		Terrain = 4,
		Smudge = 5
	};

	FA2::CString GetArtID(const FA2::CString& ID);
	FA2::CString GetVehicleOrAircraftFileID(const FA2::CString& ID);
	FA2::CString GetTerrainOrSmudgeFileID(const FA2::CString& ID);
	FA2::CString GetBuildingFileID(const FA2::CString& ID);
	FA2::CString GetInfantryFileID(const FA2::CString& ID);
	ObjectType GetItemType(const FA2::CString& ID);

	void DumpFrameToFile(unsigned char* pBuffer, Palette* pPal, int Width, int Height, const FA2::CString& name);

	struct SHPUnionData
	{
		unsigned char* pBuffer;
		int Width;
		int Height;
		int DeltaX;
		int DeltaY;
	};

	static std::vector<SHPUnionData> UnionSHP_Data[2];
	static std::map<FA2::CString, ObjectType> ObjectTypes;
	static unsigned char VXL_Data[0x10000];
};