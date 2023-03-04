#include "../FA2Expand.h"
#include <CTaskForce.h>
#include <GlobalVars.h>
#include <ObjectOptions.h>
#include <Drawing.h>
#include <CMapData.h>
#include <CScriptTypes.h>
#include "../Meta/INIMeta.h"
#include "../Replacement/CLoadingExt.h"
#include "../Utilities/HackHelper.h"
#include "../Enhancement/CMapData.h"

//fix FA2 would automatically convert file saving prefix
DEFINE_HOOK(42703A, FA2Main_SaveMap_Extension, 9)
{
	return 0x42708D;
}

#if 0
DEFINE_HOOK(428829, FA2Main_SaveMap_MinPlayer, 7)
{
	GET(int const, count, EAX);

	LogWarn(" %d", count);

	return 0;
}

DEFINE_HOOK(42B13F, FA2Main_SaveMap_MinPlayer_Debug2, 7)
{
	GET_STACK(int const, count, 0x28);

	LogDebug(" %d" + count);

	return 0;
}
#endif

DEFINE_HOOK(421BCE, CWinApp_InitInstance_EasyMode, 6)
{
	return 0x421DBD;
}

// Make FA2 use path stored in FinalAlert.ini instead of Reg
DEFINE_HOOK(41FD8A, FA2Main_GetFilePath_1, 6)
{
	return 0x41FD90;
}

DEFINE_HOOK(41FDDB, FA2Main_GetFilePath_2, 6)
{
	return 0x41FDE9;
}

DEFINE_HOOK(4C61C5, CMapData_ResizeMap_SyncLocation, 5)
{
	GET(CellData*, pCell, EAX);
	auto const pSrc = CONTAINING_RECORD(R->ECX(), CellData, LAT);

	pCell->Smudge = pSrc->Smudge;
	pCell->SmudgeType = pSrc->SmudgeType;
	pCell->BaseNode = pSrc->BaseNode;

	return 0;
}


DEFINE_HOOK(4F1670, CTileSetBrowserView_ReloadComboboxes, 6)
{
	auto& rules = GlobalVars::INIFiles::Rules();

	GET_STACK(int, overlayIdx, 0x24);
	// GET(CString, name, ECX);

	//LogWarn(" overlayIdx:" + std::to_string(overlayIdx));

	if (overlayIdx > 255) {
		return 0x4F1695;
	}

	char buf[8];
	_itoa_s(overlayIdx, buf, 10);
	auto const& overlayName = rules.GetString("OverlayTypes", buf);
	//LogWarn(std::string("overlay ") + pOvlName->Value.c_str() + " entries : " + (entries ? "true" : "false"));
	//LogWarn("value: " + std::string(entries ? entries->Items.GetItem("Wall.HasConnection")->Value.c_str() : "null"));
	if (!rules.GetBool(overlayName, "Wall.HasConnection", true)) {
		return 0x4F1695;
	}

	GET(FA2::CString, name, ECX);
	name.Format("%04d (%s)", overlayIdx, name);
	R->ECX<const char*>(name);
	return 0;
}

__declspec(naked) void ret_func()
{
	__asm
	{
		push    ecx
		mov     ecx, esi
		mov dword ptr[esp + 18h], 0
		mov eax, 0x553134
		call  eax
		mov     eax, [esp + 20h]
		cmp     eax, 1Dh
		push esi
		mov ecx, 0x43CE96
		mov esi, 0x43D037
		cmova ecx,esi
		pop esi
		jmp ecx
	};
}

//disable FA2 action hack
DEFINE_HOOK(43CE79, sub_43CE50_SkipHack, B)
{
	//GET(const FA2::CString*, string, ECX);

	//LogWarn(std::string(" :") + string->c_str());
	return  reinterpret_cast<DWORD>(ret_func);
}

DEFINE_HOOK(4D6E4D, CScriptTypeClass_OnItemSelectChanged_Houses, 6)
{
	R->Stack(0x0, true);
	R->Stack(0x4, true);

	return 0;
}

DEFINE_HOOK(473E46, CIsoView_UpdatePaint_InfantryFacing, 9)
{
	GET(int, Facing, EAX);

	//GET_STACK(const CString, ID, 0x590);
	//LogInfo(std::string(" : item ") + static_cast<const char*>(ID) + " facing : " + std::to_string(Facing));

	R->EAX(7 - Facing / 32);

	R->ECX(R->lea_Stack<DWORD>(0x590));

	return 0x473E52;
}

//having problems with patched FA2
#if 1
DEFINE_HOOK(45AF03, CIsoView_StatusBar_YXTOXY_YToX_1, 7)
{
	GET_STACK(int, nPointX, 0x30);
	R->EDI(nPointX);
	R->ECX(R->lea_Stack<DWORD>(0x458));
	return 0x45AF0A;
}

DEFINE_HOOK(45AF21, CIsoView_StatusBar_YXTOXY_YToX_2, 7)
{
	GET_STACK(int, nPointY, 0x38);
	R->EDI(nPointY);
	return 0;
}

DEFINE_HOOK(45AF44, CIsoView_StatusBar_YXTOXY_XToY_1, 7)
{
	GET_STACK(int, nPointY, 0x38);
	R->EBX(nPointY);
	R->EAX(R->lea_Stack<DWORD>(0x458));
	return 0x45AF4B;
}

DEFINE_HOOK(45AF57, CIsoView_StatusBar_YXTOXY_XToY_2, 7)
{
	GET_STACK(int, nPointX, 0x30);
	R->EBX(nPointX);
	return 0;
}
#endif

//fix Turret check using ImageID rather than own type ID
DEFINE_HOOK(48341F, CIsoView_LoadImage_TurretAnim, 5)
{
	GET_BASE(const FA2::CString, ID, 0x8);

	R->EAX(&ID);
	return 0;
}

DEFINE_HOOK(48346B, CIsoView_LoadImage_Turret, 5)
{
	//GET_BASE(const CString, ImageID, -0x1C);
	GET_BASE(const FA2::CString, ID, 0x8);

	R->EDX(&ID);
	return 0;
}

DEFINE_HOOK(4834C7, CIsoView_LoadImage_TurretIsVoxel, 5)
{
	GET_BASE(const FA2::CString, ID, 0x8);

	R->EDX(&ID);
	return 0;
}

DEFINE_HOOK(470986, CIsoView_Draw_BuildingImageDataQuery_1, 8)
{
	REF_STACK(ImageDataClass, image, STACK_OFFS(0xD18, 0xAFC));
	REF_STACK(StructureData, structure, STACK_OFFS(0xD18, 0xC0C));

	int nFacing = 0;
	if (INIMeta::GetRules().GetBool(structure.ID, "Turret")) {
		nFacing = 7 - (structure.Facing / 32) % 8;
	}
	image = *ImageDataMapHelper::GetImageDataFromMap(CLoadingExt::GetImageName(structure.ID, nFacing));

	CMapDataExt::BuildingIndex = R->ESI();

	return 0x4709E1;
}

DEFINE_HOOK(470AE3, CIsoView_Draw_BuildingImageDataQuery_2, 7)
{
	REF_STACK(ImageDataClass, image, STACK_OFFS(0xD18, 0xAFC));
	REF_STACK(StructureData, structure, STACK_OFFS(0xD18, 0xC0C));

	int nFacing = 0;
	if (INIMeta::GetRules().GetBool(structure.ID, "Turret")) {
		nFacing = (7 - structure.Facing / 32) % 8;
	}
	image = *ImageDataMapHelper::GetImageDataFromMap(CLoadingExt::GetImageName(structure.ID, nFacing));

	return 0x470B4D;
}

DEFINE_HOOK(47292E, CIsoView_Draw_BaseNodeImageCrashFix, 8)
{
	GET(const FA2::CString*, pKey, EAX);

	auto const defKey = FA2::CString("GAPOWR");
	auto const& key = pKey ? *pKey : defKey;

	auto const imageLoaded = ImageCacheFlag::Global()[key];

	if (pKey) {
		pKey->~CString();
	} else {
		R->Stack(0xD8, 0);// first item
	}
	R->ECX(imageLoaded);

	return 0x472978;
}

//DEFINE_HOOK(5564FC, operator_delete, 9)
//{
//	if (R->Stack<DWORD>(0x0) > 0x70000000) {
//		HackHelper::DumpStack(R, 0x100);
//	}
//	auto const val = R->Stack<DWORD>(0x4);
//	PUSH_VAR32(val);
//	CALL(0x536106);
//	ADD_ESP(4);
//	return 0x556506;
//}

//Free image buffer
//DEFINE_HOOK(491F36, sub_00491F36, 8)
//{
//	//_DumpStack(R, 0x100);
//	LogDebug("");
//	return 0;
//}

#if 0 //GetBoolean do not work
DEFINE_HOOK(48345B, CIsoView_LoadImage_Turret, 5)
{
	GET_BASE(const CString, ImageID, -0x1C);
	GET_BASE(const FA2::CString, ID, 0x8);

	LogInfo(std::string(" "));


	bool hasTurret = GlobalVars::INIFiles::Rules->GetBoolean(ID, "Turret", false);
	LogInfo(std::string(" : ID ") + static_cast<const char*>(ID));
	LogInfo(std::string(" : ImageID ") + static_cast<const char*>(ImageID) + " hasTurret : " + std::to_string(hasTurret));

	R->AL(hasTurret);
	return 0x483491;
}
#endif

//
DEFINE_HOOK(4ACD28, sub_4ACB60_BuildingInitData, E)
{
	REF_STACK(BuildingOptionValues, Option, STACK_OFFS(0x194, 0x184));
	Option.Facing = "0";
	return 0x4ACD36;
}

enum class Encoding
{
	Unicode = 0,
	UTF8,//we support BOM UTF8 only
	UnicodeBig,
};

Encoding getFileEncodeType(const char* filePath)
{
	unsigned char headBuf[3] = { 0 };
	Encoding type = Encoding::Unicode;

	auto const pFile = _fsopen(filePath, "r", _SH_DENYWR);

	if (!pFile) { 
		return type; 
	}

	fseek(pFile, 0, SEEK_SET);
	fread(headBuf, 3, 1, pFile);
	//utf8-bom ：FF BB BF
	if (headBuf[0] == 0xEF && headBuf[1] == 0xBB && headBuf[2] == 0xBF) {
		type = Encoding::UTF8;
	}//little Endian Unicode：FF FE  intel x86 default
	else if (headBuf[0] == 0xFF && headBuf[1] == 0xFE) {
		type = Encoding::Unicode;
	}//big Endian Unicode：FE FF
	else if (headBuf[0] == 0xFE && headBuf[1] == 0xFF) {
		type = Encoding::UnicodeBig;
	}
	fclose(pFile);
	//force unicode
	return type;
}

void Utf8ToGbk(std::string& szGBK)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, szGBK.c_str(), -1, NULL, 0);

	std::wstring wszGBK;
	wszGBK.resize(len);

	MultiByteToWideChar(CP_UTF8, 0, szGBK.c_str(), -1, wszGBK.data(), len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK.data(), -1, NULL, 0, NULL, NULL);
	//reuse input as output, and override
	szGBK.resize(len);
	WideCharToMultiByte(CP_ACP, 0, wszGBK.data(), -1, szGBK.data(), len, NULL, NULL);
}

char tmpMapPath[MAX_PATH];

DEFINE_HOOK(452D18, INIClass_ReadFile_GuessEncoding, 7)
{
	GET_STACK(const char*, pFileName, STACK_OFFS(0x22FC, -0x4));

	auto const encoding = getFileEncodeType(pFileName);

	if (encoding == Encoding::UTF8) {
		sprintf_s(tmpMapPath, "%s\\tmp.map", GlobalVars::ExePath());
		std::string fileContent;
		std::ifstream iniFile(pFileName);

		if (iniFile.is_open()) {
			iniFile.seekg(0, std::ios::end);
			fileContent.resize(static_cast<size_t>(iniFile.tellg()) - 3);
			iniFile.seekg(3, std::ios::beg);
			iniFile.read(fileContent.data(), fileContent.size());
			iniFile.close();
			Utf8ToGbk(fileContent);
		}
		std::ofstream tmpFile(tmpMapPath);
		if (tmpFile.is_open()) {
			tmpFile.write(fileContent.c_str(), fileContent.size());
			tmpFile.flush();
			tmpFile.close();
			//R->Stack(STACK_OFFS(0x22FC, -0x4), tmpMapPath);
			R->ESI(tmpMapPath);
		}
	}

	return 0;
}

DEFINE_HOOK(4D87BB, CScriptTypeClass_OnScriptTypeDelete_Tip, 5)
{
	GET(CScriptTypes* const, pThis, ESI);

	R->EAX(
		pThis->MessageBoxA(
		"确定要删除这个脚本么？不要忘了删除脚本出现的地方。", 
		"删除脚本", 0x24)
	);
	return 0x4D87CE;
}