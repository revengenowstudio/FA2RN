#pragma once
#include <FA2PP.h>

class CSFTable
{
private:
	static constexpr DWORD _H = 0x72CBF8;
	struct _S
	{
		const wchar_t* QueryUIName(const char* pRegName) {
			JMP_THIS(0x4B2610);
		}
	};

public:
	static CString GetUIName(const char* pRegName)
	{
		_S* _X = (_S*)_H;
		auto wstr = _X->QueryUIName(pRegName);
		char* value = nullptr;
		auto len = wcslen(wstr);
		int valueBufferSize = WideCharToMultiByte(CP_ACP, NULL, wstr, len, value, 0, NULL, NULL) + 1;
		value = new char[valueBufferSize];
		WideCharToMultiByte(CP_ACP, NULL, wstr, len, value, valueBufferSize, NULL, NULL);
		value[valueBufferSize - 1] = '\0';
		CString ret = value;
		delete[] value;
		return ret;
	}
};
