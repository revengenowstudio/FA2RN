#pragma once
//#include <MFC/FA2CString.h>
#include <FA2PPCore.h>
#include <string>

//forward declaration
namespace FA2 {
	class CString;
}

// FinalAlert.ini
class FinalAlertConfig
{
public:
	static DWORD ReadString(const char* pSection, const char* pKey, const char* pDefault = "", char* pBuffer = nullptr);
	static void WriteString(const char* pSection, const char* pKey, const char* pContent);
	static char ReadBuffer[0x400];
	static std::string lpPath;
};

class Translations
{
public:
	static char Languages[4][0x400];

	static void Initialize();
	static FA2::CString Translate(const char* pLabelName);
	static FA2::CString TranslateOrDefault(const char* pLabelName, const FA2::CString& def);
};