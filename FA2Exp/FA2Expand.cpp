#include "FA2Expand.h"
#include <iostream>
#include <stdio.h>
#include "Enhancement/MultiLanguage.h"
#include "Replacement/CFinalSunDlgExt.h"
#include "Replacement/CTaskForceExt.h"
#include "Replacement/CScriptTypesExt.h"
#include "Replacement/CTeamTypesExt.h"

typedef  HRSRC
(WINAPI *FindResourceProc)(
	_In_opt_ HMODULE hModule,
	_In_     LPCSTR lpName,
	_In_     LPCSTR lpType
	);


SYRINGE_HANDSHAKE(pInfo)
{
	if (pInfo) {
		// generate the output message
		if (pInfo->Message) {
			sprintf_s(pInfo->Message, pInfo->cchMessage, "Found FA2Exp Module");
		}
		return S_OK;
	}

	return E_POINTER;
}

FindResourceProc g_FA2FindResourceProc = nullptr;

HRSRC WINAPI FetchResource(
	_In_opt_ HMODULE hModule,
	_In_     LPCSTR lpName,
	_In_     LPCSTR lpType
)
{

	if (HRSRC hResInfo = FindResource(hModule, lpName, lpType)) {
		//if (HGLOBAL hResData = LoadResource(hModule, hResInfo)) {
		//	LockResource(hResData);
		//
		//	return hResInfo; //Resource locked and loaded (omg what a pun), return!
		//}
		return hResInfo;
	}
	return g_FA2FindResourceProc(hModule, lpName, lpType); //Nothing was found, try the game's own resources.
}

void updateExecutionPath()
{
	GetModuleFileNameA(0, GlobalVars::ExePath(), 0x104u);
	strrchr(GlobalVars::ExePath(), '\\')[1] = 0;
}

void __stdcall FA2Expand::ExeRun()
{
	logger::Init();
#if 0
	auto const process = GetCurrentProcess();
	DWORD_PTR const processAffinityMask = 1; // limit to first processor
	SetProcessAffinityMask(process, processAffinityMask);

#if _MSC_VER >= 1700
	// install a new exception handler, if this version of Windows supports it
	if (FA2Expand::ExceptionMode != FA2Expand::ExceptionHandlerMode::Default) {
		if (HINSTANCE handle = GetModuleHandle(TEXT("kernel32.dll"))) {
			if (GetProcAddress(handle, "AddVectoredExceptionHandler")) {
				FA2Expand::pExceptionHandler = AddVectoredExceptionHandler(1, FA2Ext::ExceptionFilter);
			}
		}
	}
#endif

#endif
	#if defined(DEBUG)
	system("taskkill /F /IM syringe.exe");
	MessageBoxA(nullptr, "Test", "", MB_OK);
	#endif // defined(DEBUG)

	updateExecutionPath();
	//MessageBoxA(nullptr, "Test", "Test", MB_OK);
	//std::locale::global(std::locale(""));
	Translations::Initialize();

	constexpr char title[] = "Final Alert for \"Revenge Now !\"";
	constexpr char delWarn[] = "确定要删除这个脚本吗？不要忘记删除调用这个脚本的小队";
	RunTime::ResetMemoryContentAt(0x5CD808, title, sizeof(title));
	RunTime::ResetMemoryContentAt(0x5D15A0, delWarn, sizeof(delWarn));
	CFinalSunDlgExt::ProgramStartupInit();
	CTaskForceExt::ProgramStarupInit();
	CScriptTypesExt::ProgramStartupInit();
	CTeamTypesExt::ProgramStartupInit();
	//auto const pStr = reinterpret_cast<const char*>(0x5CD808);
	//logger::g_logger.Info(__FUNCTION__" " + std::string(pStr));
	//DWORD oldProtect, newProtect = 0;
	//auto ret = VirtualProtectEx(process, &pplContent[offset], size, PAGE_READWRITE, &oldProtect);
	//g_FA2FindResourceProc = *reinterpret_cast<FindResourceProc*>(0x5911D8);
	//RunTime::ResetMemoryContentAt(0x5911D8, &FetchResource, sizeof(&FetchResource));


}





FA2Expand::FA2Expand()
{
}


FA2Expand::~FA2Expand()
{
}
