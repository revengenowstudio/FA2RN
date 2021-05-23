#include "MultiLanguage.h"
#include <GlobalVars.h>
#include <Windows.h>

std::string FinalAlertConfig::lpPath;
char FinalAlertConfig::pLastRead[0x400];

char Translations::pLanguage[4][0x400];

//member functions
DWORD FinalAlertConfig::ReadString(const char* pSection, const char* pKey, const char* pDefault, char* pBuffer)
{
	DWORD dwRet = GetPrivateProfileString(pSection, pKey, pDefault, FinalAlertConfig::pLastRead, 0x400, lpPath.data());
	if (pBuffer)
		strcpy_s(pBuffer, 0x400, pLastRead);
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

	strcpy_s(Translations::pLanguage[0], FinalAlertConfig::pLastRead);
	strcpy_s(Translations::pLanguage[1], FinalAlertConfig::pLastRead);
	strcpy_s(Translations::pLanguage[2], FinalAlertConfig::pLastRead);
	strcpy_s(Translations::pLanguage[3], FinalAlertConfig::pLastRead);
	strcat_s(Translations::pLanguage[0], "-Strings");
	strcat_s(Translations::pLanguage[1], "-StringsRA2");
	strcat_s(Translations::pLanguage[2], "-Translations");
	strcat_s(Translations::pLanguage[3], "-TranslationsRA2");
}

bool Translations::GetTranslationItem(const char* pLabelName, FA2::CString& ret)
{
	auto& falanguage = GlobalVars::INIFiles::FALanguage();
	for (auto const& languageSection : pLanguage) {
		if (auto const pSection = falanguage.TryGetSection(languageSection)) {
			auto itr = pSection->EntriesDictionary.find(pLabelName);
			if (itr != pSection->EntriesDictionary.end())
			{
				ret = itr->second;
				return true;
			}
		}
	}

	return false;
}