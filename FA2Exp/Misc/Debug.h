#pragma once
#include "../Syringe.h"

class Debug
{
public:
	// dumping

	static void DumpStack(REGISTERS* R, size_t len, int startAt = 0);

	static void DumpObj(void const* data, size_t len);

	template <typename T>
	static void DumpObj(const T& object) {
		DumpObj(&object, sizeof(object));
	}
};