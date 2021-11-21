#pragma once
#include "../logger.h"
#include "../Syringe.h"

namespace HackHelper
{
	inline void DumpStack(REGISTERS* R, size_t len, int startAt = 0)
	{
		LogDebug("Dumping %X bytes of stack", len);
		auto const end = len / 4;
		auto const* const mem = R->lea_Stack<DWORD*>(startAt);
		for (auto i = 0u; i < end; ++i) {
			LogDebug("esp+%04X = %08X", i * 4, mem[i]);
		}
		LogDebug("Done."); // flushes
	}
}