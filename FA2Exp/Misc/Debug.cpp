#include "Debug.h"
#include "../logger.h"

void Debug::DumpObj(void const* const data, size_t const len) {
	LogDebug("Dumping %u bytes of object at %p", len, data);
	auto const bytes = static_cast<BYTE const*>(data);

	LogDebug("       |");
	for (auto i = 0u; i < 0x10u; ++i) {
		LogDebug(" %02X |", i);
	}
	LogDebug("-------|");
	for (auto i = 0u; i < 0x10u; ++i) {
		LogDebug("----|", i);
	}
	auto const bytesToPrint = (len + 0x10 - 1) / 0x10 * 0x10;
	for (auto startRow = 0u; startRow < bytesToPrint; startRow += 0x10) {
		LogDebug(" %05X |", startRow);
		auto const bytesInRow = std::min(len - startRow, 0x10u);
		for (auto i = 0u; i < bytesInRow; ++i) {
			LogDebug(" %02X |", bytes[startRow + i]);
		}
		for (auto i = bytesInRow; i < 0x10u; ++i) {
			LogDebug(" -- |");
		}
		for (auto i = 0u; i < bytesInRow; ++i) {
			auto const& sym = bytes[startRow + i];
			LogDebug("%c", isprint(sym) ? sym : '?');
		}
	}
	LogDebug("End of dump."); // flushes
}

void Debug::DumpStack(REGISTERS* R, size_t len, int startAt) {
	LogDebug("Dumping %X bytes of stack", len);
	auto const end = len / 4;
	auto const* const mem = R->lea_Stack<DWORD*>(startAt);
	for (auto i = 0u; i < end; ++i) {
		LogDebug("esp+%04X = %08X", i * 4, mem[i]);
	}
	LogDebug("Done."); // flushes
}