#include "MultiLanguage.h"
#include <GlobalVars.h>
#include <Windows.h>

std::string FinalAlertConfig::lpPath;
char FinalAlertConfig::ReadBuffer[0x400];

char Translations::Languages[4][0x400];

//member functions
DWORD FinalAlertConfig::ReadString(const char* pSection, const char* pKey, const char* pDefault, char* pBuffer)
{
	DWORD dwRet = GetPrivateProfileString(pSection, pKey, pDefault, FinalAlertConfig::ReadBuffer, 0x400, lpPath.data());
	if (pBuffer)
		strcpy_s(pBuffer, 0x400, ReadBuffer);
	return dwRet;
}
void FinalAlertConfig::WriteString(const char* pSection, const char* pKey, const char* pContent)
{
	WritePrivateProfileString(pSection, pKey, pContent, lpPath.data());
};


//====================================
void Translations::Initialize()
{
	FinalAlertConfig::lpPath = GlobalVars::ExePath();
	FinalAlertConfig::lpPath += "FinalAlert.ini";
	FinalAlertConfig::ReadString("FinalSun", "Language", "English");

	strcpy_s(Translations::Languages[0], FinalAlertConfig::ReadBuffer);
	strcpy_s(Translations::Languages[1], FinalAlertConfig::ReadBuffer);
	strcpy_s(Translations::Languages[2], FinalAlertConfig::ReadBuffer);
	strcpy_s(Translations::Languages[3], FinalAlertConfig::ReadBuffer);
	strcat_s(Translations::Languages[0], "-Strings");
	strcat_s(Translations::Languages[1], "-StringsRA2");
	strcat_s(Translations::Languages[2], "-Translations");
	strcat_s(Translations::Languages[3], "-TranslationsRA2");
}

FA2::CString Translations::Translate(const char* pLabelName)
{
	FA2::CString ret;
	auto& falanguage = GlobalVars::INIFiles::FALanguage();
	for (auto const& languageSection : Languages) {
		if (auto const pSection = falanguage.TryGetSection(languageSection)) {
			auto itr = pSection->EntriesDictionary.find(pLabelName);
			if (itr != pSection->EntriesDictionary.end()) {
				ret = itr->second;
				break;
			}
		}
	}

	return ret;
}

FA2::CString Translations::TranslateOrDefault(const char* pLabelName, const FA2::CString& def)
{
	auto ret = Translate(pLabelName);
	return ret.GetLength() ? ret : def;
}
