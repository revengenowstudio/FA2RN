#include "RunTime.h"
#include "logger.h"
#include <FA2PP.h>

RunTime::RunTime()
{
}


RunTime::~RunTime()
{
}

void RunTime::ResetMemoryContentAt(ptr_type addr, const void* content, size_t size, size_t offset)
{
	BYTE* pplContent = (BYTE*)(addr);
	auto process = GetCurrentProcess();
	DWORD oldProtect, newProtect = 0;
	auto ret = VirtualProtectEx(process, &pplContent[offset], size, PAGE_READWRITE, &oldProtect);
	if (ret != TRUE) {
		LogDebug("ResetMemoryContentAt , addr = %x, PAGE_READWRITE set failure, ret = %d", addr, ret);
	}
	memcpy(&pplContent[offset], content, size);
	ret = VirtualProtectEx(process, &pplContent[offset], size, oldProtect, &newProtect);
	if (ret != TRUE) {
		//Debug::Log("ResetMemoryContentAt , addr = %x, set back error, ret = %d\n", addr, ret);
	}
	//Debug::Log("ResetMemoryContentAt , new vftable address = %x\n", pplVrtable[9]);
}