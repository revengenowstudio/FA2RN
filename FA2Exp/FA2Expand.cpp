#include "FA2Expand.h"
#include <iostream>
#include <stdio.h>
#include "Enhancement/MultiLanguage.h"
#include "Replacement/CFinalSunDlgExt.h"
#include "Replacement/CTaskForceExt.h"
#include "Replacement/CScriptTypesExt.h"
#include "Replacement/CTeamTypesExt.h"
#include "Misc/DrawStuff.h"
#include "Misc/Debug.h"

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

void preinit()
{
#if defined(ReplaceMemoryAllocate)
	class MemoryHelper
	{
	public:
		static void* __cdecl malloc(size_t sz)
		{
			return ::malloc(sz);
		}

		static void __cdecl free(void* ptr)
		{
			::free(ptr);
		}
	};

	RunTime::ReplaceFunction(0x591110, ::HeapFree);
	RunTime::ReplaceFunction(0x591128, ::HeapReAlloc);
	RunTime::ReplaceFunction(0x59112C, ::HeapAlloc);
	RunTime::ReplaceFunction(0x591138, ::HeapSize);
	RunTime::ReplaceFunction(0x591154, ::HeapDestroy);
	RunTime::ReplaceFunction(0x591158, ::HeapCreate);

	RunTime::ReplaceFunction(0x59128C, ::GlobalAlloc);
	RunTime::ReplaceFunction(0x5912FC, ::GlobalFree);
	RunTime::ReplaceFunction(0x591254, ::GlobalReAlloc);

	RunTime::ReplaceFunction(0x591270, ::LocalAlloc);
	RunTime::ReplaceFunction(0x5912B8, ::LocalFree);
	RunTime::ReplaceFunction(0x591248, ::LocalReAlloc);

	RunTime::ReplaceFunction(0x591268, ::TlsAlloc);
	RunTime::ReplaceFunction(0x59125C, ::TlsFree);

	RunTime::ReplaceFunction(0x591160, ::VirtualAlloc);
	RunTime::ReplaceFunction(0x59115C, ::VirtualFree);

	//RunTime::SetRelativeCall(0x536106, reinterpret_cast<RunTime::ptr_type>(MemoryHelper::free));
	//RunTime::SetRelativeCall(0x537CA0, reinterpret_cast<RunTime::ptr_type>(MemoryHelper::malloc));
#endif
}

#if defined(ReplaceMemoryAllocate)
DEFINE_HOOK(537CA0, Hook_malloc, 6)
{
	GET_STACK(const size_t, size, 0x4);
	//Debug::DumpStack(R, 0x100);
	R->EAX(malloc(size));
	return 0x537CB1;
}

DEFINE_HOOK(536106, Hook_free, 5)
{
	GET_STACK(void*, ptr, 0x4);
	//Debug::DumpStack(R, 0x100);
	free(ptr);
	return 0x5361D0;
}
#endif

void __stdcall FA2Expand::ExeRun()
{
	logger::Init();
	preinit();
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
	//LogInfo(" " + std::string(pStr));
	//DWORD oldProtect, newProtect = 0;
	//auto ret = VirtualProtectEx(process, &pplContent[offset], size, PAGE_READWRITE, &oldProtect);
	//g_FA2FindResourceProc = *reinterpret_cast<FindResourceProc*>(0x5911D8);
	//RunTime::ReplaceFunction(0x5911D8, &FetchResource, sizeof(&FetchResource));

	DrawStuff::init();
}

void FA2Expand::ExeTerminate()
{
	DrawStuff::uninit();
}



FA2Expand::FA2Expand()
{
}


FA2Expand::~FA2Expand()
{
}
