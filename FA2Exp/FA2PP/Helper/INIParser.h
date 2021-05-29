#pragma once

#include "Parser.h"

#include "../INI.h"
#include <algorithm>

using byte = unsigned char;

namespace FAINIHelper
{

}

class INI_EX {
	INIClass* IniFile;

public:
	explicit INI_EX(INIClass* pIniFile)
		: IniFile(pIniFile)
	{ }

	FA2::CString& value() {
		return Buffer;
	}
	const FA2::CString& value() const {
		return Buffer;
	}

	// basic string reader
	size_t ReadString(const char* pSection, const char* pKey) {
		Buffer = IniFile->GetString(pSection, pKey);
		return static_cast<size_t>(Buffer.GetLength());
	}

	// parser template
	template <typename T, size_t Count>
	bool Read(const char* pSection, const char* pKey, T* pBuffer) {
		if (this->ReadString(pSection, pKey)) {
			return Parser<T, Count>::Parse(this->value(), pBuffer) == Count;
		}
		return false;
	}

	// helpers

	bool ReadBool(const char* pSection, const char* pKey, bool* bBuffer) {
		return Read<bool, 1>(pSection, pKey, bBuffer);
	}

	bool ReadInteger(const char* pSection, const char* pKey, int* nBuffer) {
		return Read<int, 1>(pSection, pKey, nBuffer);
	}

	bool Read2Integers(const char* pSection, const char* pKey, int* nBuffer) {
		return Read<int, 2>(pSection, pKey, nBuffer);
	}

	bool Read3Integers(const char* pSection, const char* pKey, int* nBuffer) {
		return Read<int, 3>(pSection, pKey, nBuffer);
	}

	bool Read4Integers(const char* pSection, const char* pKey, int* nBuffer) {
		return Read<int, 4>(pSection, pKey, nBuffer);
	}

	bool Read3Bytes(const char* pSection, const char* pKey, byte* nBuffer) {
		return Read<byte, 3>(pSection, pKey, nBuffer);
	}

	bool ReadDouble(const char* pSection, const char* pKey, double* nBuffer) {
		return Read<double, 1>(pSection, pKey, nBuffer);
	}

private:
	FA2::CString Buffer;

};
