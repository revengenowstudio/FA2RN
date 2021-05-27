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
	static FA2::CString GetUIName(const char* pRegName)
	{
		FA2::CString ret;
		_S* _X = (_S*)_H;
		auto wstr = _X->QueryUIName(pRegName);
		auto len = wcslen(wstr);
		int valueBufferSize = WideCharToMultiByte(CP_ACP, NULL, wstr, len, NULL, 0, NULL, NULL) + 1;
		auto buffer = ret.GetBuffer(valueBufferSize);
		WideCharToMultiByte(CP_ACP, NULL, wstr, len, buffer, valueBufferSize, NULL, NULL);
		buffer[valueBufferSize - 1] = '\0';
		return ret;
	}
};
