#pragma once
#include "../FA2Expand.h"
#include <INI.h>
#include <vector>
#include <map>

using IndiceStorage = std::vector<FA2::CString>;
using SequencedSection = std::vector<FA2::CString>;//std::map<FA2::CString, FA2::CString, INISectionEntriesComparator>

/*
* @brief This class uses to simulation a combination of a few ini dictionaries, to
		 act as they are inside a same ini
*/
class INIMetaGroup
{
	friend class INIMeta;
public:
	INIMetaGroup() = default;


	FA2::CString GetString(const char* pSection, const char* pKey, const char* pDefault = "") const;
	bool GetBool(const char* pSection, const char* pKey, bool def = false) const;
	int GetInteger(const char* pSection, const char* pKey, int def = 0) const;

	IndiceStorage GetIndices(const char* pSection, bool bParseIntoValue = false) const;
	SequencedSection GetSectionItems(const char* pSection) const;

private:
	void AddINI(INIClass& INI);
	void AddINI(INIClass* pINI);
	INIClass* GetINIAt(int idx);

	std::vector<INIClass*> group;
};

class INIMeta
{
	friend class INIMetaGroup;
public:
	static INIMetaGroup GetRules();
	static INIMetaGroup GetDocument();

	static SequencedSection GetSectionItems(const INIClass& ini, const char* pSection);
	static IndiceStorage GetIndicies(const INIClass& ini, const char* pSection);

	static bool IsNullOrEmpty(const FA2::CString& value) { return isNullOrEmpty(value); }

private:
	static bool isNullOrEmpty(const FA2::CString& value);


};