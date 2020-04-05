#include "FA2Expand.h"


SYRINGE_HANDSHAKE(pInfo)
{
	if (pInfo) {
		return S_OK;
	}

	return E_POINTER;
}

void __stdcall FA2Expand::ExeRun()
{
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

	char constexpr title[] = "Final Alert for \"Revenge Now  !\"";
	RunTime::ResetMemoryContentAt(0x5CD808, title, sizeof(title));
	//auto const pStr = reinterpret_cast<const char*>(0x5CD808);
	//logger::g_logger.Info(__FUNCTION__" " + std::string(pStr));

}

FA2Expand::FA2Expand()
{
}


FA2Expand::~FA2Expand()
{
}
