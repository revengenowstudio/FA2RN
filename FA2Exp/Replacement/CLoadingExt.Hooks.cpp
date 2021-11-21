#include "CLoadingExt.h"

//#include <Helper/Macro.h>

#include <CObjectData.h>
#include <GlobalVars.h>
#include <INI.h>
#include <Drawing.h>

#include "../FA2Expand.h"


DEFINE_HOOK(4808A0, CLoading_LoadObjects, 5)
{
    GET(CLoadingExt*, pThis, ECX);
    REF_STACK(FA2::CString, pRegName, 0x4);

    pThis->CLoadingExt::LoadObjects(pRegName);

    return 0x486173;
}

DEFINE_HOOK(475450, GetObjectName, 7)
{
    GET_STACK(FA2::CString*, pRet, 0x4);
    GET_STACK(LPCSTR, ID, 0x8);
    GET_STACK(int, nFacing, 0xC);
    GET_STACK(DWORD, dwCaller, 0x0);

    const FA2::CString temp = CLoadingExt::GetImageName(ID, nFacing);

    switch (dwCaller)
    {
    case 0x473E63: // Infantry
    case 0x4737DA: // Aircraft
    case 0x4731AD: // Unit
    case 0x4730EA:
    case 0x4B5DF6: // Building
    case 0x4B607D:
    case 0x4B67A0:
    case 0x4B67C3:
    case 0x4B6C02:
    case 0x4B6C30:
        new(pRet) FA2::CString(temp);
        R->EAX(pRet);
        return 0x47623D;
    default:
        return 0;
    }
}

DEFINE_HOOK_AGAIN(471028, CLoading_Draw_PowerUpFacingFix, 7)
DEFINE_HOOK_AGAIN(470E93, CLoading_Draw_PowerUpFacingFix, 7)
DEFINE_HOOK_AGAIN(471FBF, CLoading_Draw_PowerUpFacingFix, 7)
DEFINE_HOOK_AGAIN(471E56, CLoading_Draw_PowerUpFacingFix, 7)
DEFINE_HOOK_AGAIN(47186C, CLoading_Draw_PowerUpFacingFix, 7)
DEFINE_HOOK(4716BB, CLoading_Draw_PowerUpFacingFix, 7)
{
    R->Stack(0, 0);
    return 0;
}

DEFINE_HOOK(42CBFC, CFinalSunDlg_CreateMap_ClearCLoadingExtData, 8)
{
    CLoadingExt::ClearItemTypes();
    return 0;
}

DEFINE_HOOK(49D2C0, CMapData_LoadMap_ClearCLoadingExtData, 5)
{
    CLoadingExt::ClearItemTypes();
    return 0;
}

DEFINE_HOOK(491FD4, sub_491D00_SafeFree, 5)
{
    GET(ImageDataMap::_Imp::TreeNode*, pNode, ESI);
    if (auto& imgBuffer = pNode->_Value.second.pImageBuffer) {
        FAMemory::Deallocate(imgBuffer);
        imgBuffer = nullptr;
    }
    if (auto& pRange = pNode->_Value.second.pPixelValidRanges) {
        FAMemory::Deallocate(pRange);
        pRange = nullptr;
    }
    return 0x491FF1;
}
