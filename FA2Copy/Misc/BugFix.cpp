#include "../FA2Expand.h"
#include <CTaskForce.h>
#include <INI.h>

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

DEFINE_HOOK(4F1670, CTileSetBrowserView_ReloadComboboxes, 6)
{
	auto& rules = INIClass::RulesIni();

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

	GET(CString, name, ECX);
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
	//GET(const FAString*, string, ECX);

	//logger::g_logger.Warn(std::string(__FUNCTION__" :") + string->c_str());
	return  reinterpret_cast<DWORD>(ret_func);
}

DEFINE_HOOK(4D6E4D, CScriptTypeClass_OnItemSelectChanged_Houses, 6)
{
	R->Stack(0x0, true);
	R->Stack(0x4, true);

	return 0;
}

#if 0
DEFINE_HOOK(43CE8D, sub_43CE50, 9)
{
	REF_STACK(const FAString, string, 0x8);

	logger::g_logger.Warn(std::string("__FUNCTION__ ") + string.c_str());

	return 0;
}
#endif