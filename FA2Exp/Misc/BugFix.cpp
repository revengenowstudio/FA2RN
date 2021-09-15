#include "../FA2Expand.h"
#include <CTaskForce.h>
#include <GlobalVars.h>
#include <ObjectOptions.h>

//fix FA2 would automatically convert file saving prefix
DEFINE_HOOK(42703A, FA2Main_SaveMap_Extension, 9)
{
	return 0x42708D;
}


DEFINE_HOOK(428829, FA2Main_SaveMap_MinPlayer, 7)
{
	GET(int const, count, EAX);

	logger::g_logger.Warn(__FUNCTION__" " + std::to_string(count));

	return 0;
}

DEFINE_HOOK(42B13F, FA2Main_SaveMap_MinPlayer_Debug2, 7)
{
	GET_STACK(int const, count, 0x28);

	logger::g_logger.Warn(__FUNCTION__" " + std::to_string(count));

	return 0;
}

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

	//logger::g_logger.Warn(__FUNCTION__" overlayIdx:" + std::to_string(overlayIdx));

	if (overlayIdx > 255) {
		return 0x4F1695;
	}

	char buf[8];
	_itoa_s(overlayIdx, buf, 10);
	auto const& overlayName = rules.GetString("OverlayTypes", buf);
	//logger::g_logger.Warn(std::string("overlay ") + pOvlName->Value.c_str() + " entries : " + (entries ? "true" : "false"));
	//logger::g_logger.Warn("value: " + std::string(entries ? entries->Items.GetItem("Wall.HasConnection")->Value.c_str() : "null"));
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

	//logger::g_logger.Warn(std::string(__FUNCTION__" :") + string->c_str());
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
	//logger::g_logger.Info(std::string(__FUNCTION__" : item ") + static_cast<const char*>(ID) + " facing : " + std::to_string(Facing));

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

#if 0 //GetBoolean do not work
DEFINE_HOOK(48345B, CIsoView_LoadImage_Turret, 5)
{
	GET_BASE(const CString, ImageID, -0x1C);
	GET_BASE(const FA2::CString, ID, 0x8);

	logger::g_logger.Info(std::string(__FUNCTION__" "));


	bool hasTurret = GlobalVars::INIFiles::Rules->GetBoolean(ID, "Turret", false);
	logger::g_logger.Info(std::string(__FUNCTION__" : ID ") + static_cast<const char*>(ID));
	logger::g_logger.Info(std::string(__FUNCTION__" : ImageID ") + static_cast<const char*>(ImageID) + " hasTurret : " + std::to_string(hasTurret));

	R->AL(hasTurret);
	return 0x483491;
}
#endif

// Fix the bug of SHP turret draws only one face
DEFINE_HOOK(483EA4, CLoading_LoadObjects_SHPTurretFix_1, 7)
{
	REF_STACK(int, nFacingCount, STACK_OFFS(0x868, 0x4C));
	// GET_STACK(bool, bIsBuilding, STACK_OFFS(0x868, 0x8C));
	GET_STACK(FA2::CString, pRegName, STACK_OFFS(0x868, -0x4));

	if (GlobalVars::INIFiles::Rules->GetBool(pRegName, "Turret")) {
		nFacingCount = 8;
	}
	else {
		nFacingCount = 1;
	}
	return 0x483EAB;
}

DEFINE_HOOK(483F9D, CLoading_LoadObjects_SHPTurretFix_2, 5)
{
	REF_STACK(int, nFlags, STACK_OFFS(0x868, 0x4C));
	GET_STACK(bool, bIsBuilding, STACK_OFFS(0x868, 0x8C));
	GET_STACK(FA2::CString, pRegName, STACK_OFFS(0x868, -0x4));

	if (GlobalVars::INIFiles::Rules->GetBool(pRegName, "Turret") && bIsBuilding) {
		return 0x483FA4;
	}
	
	return 0x483FC2;
}

DEFINE_HOOK(4ACD28, sub_4ACB60_BuildingInitData, E)
{
	REF_STACK(BuildingOptionValues, Option, STACK_OFFS(0x194, 0x184));
	Option.Facing = "0";
	return 0x4ACD36;
}

//tracers
#if defined(TRACE_DRAW_BUILDING)
DEFINE_HOOK(470B6C, CIsoView_UpdatePaint_Building_Debug, B)
{
	GET_STACK(const FA2::CString, BuildingID, 0x320);

	logger::info(std::string(__FUNCTION__" : ID ") + static_cast<const char*>(BuildingID));

	return 0;
}

DEFINE_HOOK(4709AC, CIsoView_UpdatePaint_Building_GetFrameInfo, A)
{
	GET(int, itemIdx, ESI);
	REF_STACK(const BuildingOptionValueEx, BuildingID, 0x10C);

	logger::info(std::string(__FUNCTION__" : BuildingID ") + static_cast<const char*>(BuildingID.Name) + " facing : " + std::to_string(BuildingID.FacingValue));

	LogDebug(__FUNCTION__" : itemIdx %d", itemIdx);

	for (auto offset = 0; offset < 8; offset++) {
		auto const& frame = SHPImageCache[itemIdx].Frames[offset];
		LogDebug(__FUNCTION__" : cache offIdx : %d, ptr = %X", offset, frame.ptr_0);
	}



	return 0;
}

DEFINE_HOOK(4709CE, CIsoView_UpdatePaint_Building_GetFrameInfo2, 6)
{
	REF_STACK(const BuildingOptionValueEx, BuildingID, 0x10C);
	GET(DWORD, offset, EDI);
	logger::info(std::string(__FUNCTION__" : fetch again, BuildingID ") + static_cast<const char*>(BuildingID.Name) + " facing : " + std::to_string(BuildingID.FacingValue));
	logger::info(" offset : " + std::to_string(offset));
	logger::info(" offset / 0x114 : " + std::to_string(offset / 0x114));
	return 0;
}

DEFINE_HOOK(470AD7, CIsoView_UpdatePaint_Building_FirstTimeLoad, 7)
{
	GET(const char*, pString, EAX);
	logger::info(std::string(__FUNCTION__" : ID ") + pString);
	return 0;
}

DEFINE_HOOK(4B606D, sub_4B5460, 5)
{
	REF_STACK(FA2::CString, key, 0x1C);
	GET(int, frameSeqIdx, EBX);

	logger::info(std::string(__FUNCTION__" : ID ") + static_cast<const char*>(key) + " frameSeqIdx : " + std::to_string(frameSeqIdx));


	return 0;
}


//DEFINE_HOOK(470BC5, CIsoView_UpdatePaint_Building_Debug2, 7)
//{
//	REF_STACK(const BuildingOptionValueEx, BuildingID, 0x10C);
//
//	auto const& BuildingInfo = R->ref_Stack<const BuildingFrameInfo>(0x21C);
//
//	logger::g_logger.Info(std::string(__FUNCTION__" : BuildingID ") + static_cast<const char*>(BuildingID.Name));
//
//	logger::g_logger.Info(std::string(__FUNCTION__" : BuildingInfo ") + reinterpret_cast<const char*>(&BuildingInfo));
//
//
//	return 0;
//}

DEFINE_HOOK(48344F, CIsoView_LoadImage_Debug, 6)
{
	REF_BASE(const FA2::CString, buildingTypeItemStr, -88);
	//GET_BASE(DWORD, buildingTypeItemExists, -88);
	REF_BASE(const FA2::CString, ImageID, -0x1C);
	REF_BASE(const FA2::CString, TurretAnimID, -0x78);

	LogDebug(__FUNCTION__" : ImageID %s,  buildingTypeItemStr : %s", ImageID, buildingTypeItemStr);
	LogDebug(__FUNCTION__" : TurretAnimID %s", TurretAnimID);


	return buildingTypeItemStr ? 0x48345B : 0x483B6D;
}

DEFINE_HOOK(483670, CIsoView_LoadImage_SHPTurretDebug, 5)
{
	GET(int, unk, ESI);
	GET(const char*, pString, EAX);

	LogDebug(__FUNCTION__" : pString %s unk : %d", pString, unk);

	PUSH_REG(unk);
	PUSH_REG(pString);
	CALL(0x525C50);

	return 0x483675;
}

DEFINE_HOOK(483491, CIsoView_LoadImage_Turret_Debug, 5)
{
	REF_BASE(const FA2::CString, ImageID, -0x1C);
	REF_BASE(const FA2::CString, ID, 0x8);
	GET(bool, HasTurret, EAX);

	LogDebug(__FUNCTION__" : ImageID %s hasTurret : %d", ImageID, HasTurret);
	LogDebug(__FUNCTION__" : ID %s", ID);

	return 0;
}

DEFINE_HOOK(483709, CIsoView_LoadImage_SHPTurret, 6)
{
	GET_BASE(const FA2::CString, ImageID, -0x1C);
	GET_BASE(const FA2::CString, ID, 0x8);
	GET(bool, HasTurret, EAX);

	logger::g_logger.Info(std::string(__FUNCTION__" : ImageID ") + static_cast<const char*>(ImageID) + " hasTurret : " + std::to_string(HasTurret));
	logger::g_logger.Info(std::string(__FUNCTION__" : ID ") + static_cast<const char*>(ID));

	return 0;
}

DEFINE_HOOK(4B6BE5, BuildingDrawImage_Debug, 8)
{
	GET_STACK(const FA2::CString, ImageID, STACK_OFFS(0x580, 0x568));
	logger::g_logger.Info(std::string(__FUNCTION__" : ImageID ") + static_cast<const char*>(ImageID));

	if (ImageID == "GAGAP") {
		R->Stack(0x0, 1);
	}

	return 0;
}

DEFINE_HOOK(4B6C25, BuildingDrawImage_Debug2, 5)
{
	GET(int, FrameIndex, EDI);
	GET(const char*, pID, EDX);

	logger::info(std::string(__FUNCTION__" : ID ") + static_cast<const char*>(pID) + " FrameIndex : " + std::to_string(FrameIndex));

	return 0;
}

DEFINE_HOOK(4B6C33, BuildingDrawImage_FrameID, 5)
{
	GET(const FA2::CString*, FrameID, EAX);

	logger::info(std::string(__FUNCTION__" : FrameID : ") + static_cast<const char*>(*FrameID));

	return 0;
}

DEFINE_HOOK(475450, sub475450_Debug, 7)
{
	GET_STACK(DWORD, retAddr, 0x0);
	GET_STACK(const char*, imageID, 0x8);

	char buf[32];

	sprintf_s(buf, "%X", retAddr);

	logger::g_logger.Info(std::string(__FUNCTION__" : retAddr ") + buf + " imageID : " + imageID);

	return 0;
}

DEFINE_HOOK(47AE90, sub_47AB50_FileName, A)
{
	GET(const FA2::CString*, strFileName, EAX);

	logger::g_logger.Info(std::string(__FUNCTION__" : strFileName  ") + static_cast<const char*>(*strFileName));


	return 0;
}

DEFINE_HOOK(47AC7D, sub_47AB50_NoBMPS, 5)
{
	logger::g_logger.Info(std::string(__FUNCTION__" : check "));
	return 0;
}

DEFINE_HOOK(4956BA, sub_494B60_LoadFile_Debug, 5)
{
	GET_BASE(char*, pFileName, -0x14);

	logger::g_logger.Info(std::string(__FUNCTION__" : ") + pFileName);

	return 0;
}
#endif

#if 0
DEFINE_HOOK(43CE8D, sub_43CE50, 9)
{
	REF_STACK(const FA2::CString, string, 0x8);

	logger::g_logger.Warn(std::string("__FUNCTION__ ") + string.c_str());

	return 0;
}
#endif