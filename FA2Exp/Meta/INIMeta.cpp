#include "INIMeta.h"

#include <GlobalVars.h>
#include <set>

void INIMetaGroup::AddINI(INIClass* pINI)
{
	group.push_back(pINI);
}
void INIMetaGroup::AddINI(INIClass& INI)
{
	AddINI(&INI);
}


INIClass* INIMetaGroup::GetINIAt(int idx)
{
	return group.at(idx);
}

FA2::CString INIMetaGroup::GetString(const char* pSection, const char* pKey, const char* pDefault) const
{
	bool found = false;
	FA2::CString ret;
	for (auto const& ini : group) {
		if (ini->TryGetString(pSection, pKey, ret)) {
			found = true;
			break;
		}
	}
	if (!found) {
		ret = pDefault;
	}
	return ret;
}

IndiceStorage INIMetaGroup::GetIndices(const char* pSection, bool bParseIntoValue) const
{
	std::vector<FA2::CString> ret;
	std::map<unsigned int, FA2::CString> tmp;
	std::map<FA2::CString, unsigned int> tmp2;
	std::map<FA2::CString, FA2::CString> tmp3; // Value - Key
	std::map<unsigned int, INIClass*> belonging;

	for (auto& pINI : group) {
		if (pINI) {
			auto const& curSequencedValues = INIMeta::GetIndicies(*pINI, pSection);
			for (auto& item : curSequencedValues) {
				FA2::CString value = pINI->GetString(pSection, item, item);
				auto& unitr = tmp2.find(value);
				if (unitr == tmp2.end()) {
					belonging[tmp2.size()] = pINI;
					tmp2[value] = tmp2.size();
				} else {
					belonging[unitr->second] = pINI;
				}
				tmp3[value] = item;
			}
		}
	}

	for (auto& pair : tmp2) {
		tmp[pair.second] = pair.first;
	}

	ret.resize(tmp.size());
	size_t idx = 0;
	for (auto& x : tmp) {
		ret[idx++] = x.second;
	}

	if (!bParseIntoValue) {
		for (size_t i = 0, sz = ret.size(); i < sz; ++i) {
			ret[i] = tmp3[ret[i]];
		}
	}

	return ret;
}

//#define SkipEmpty
SequencedSection INIMetaGroup::GetSection(const char* pSection) const
{
	SequencedSection ret;
	for (auto& pINI : group) {
		if (!pINI) {
			continue;
		}
		if (auto const pSectionData = pINI->TryGetSection(pSection)) {
			for (auto& pair : pSectionData->EntriesDictionary)
			{
#if defined(SkipEmpty)
				if (INIMeta::isNullOrEmpty(pair.first) ||
					INIMeta::isNullOrEmpty(pair.second)
					) {
					ret.push_back("");
					continue;
				}
#endif
				if (pair.first == "Name") {
					ret.push_back("");
					continue;
				}
				ret.push_back(pair.second);
			}
		}
	}

	return ret;
}

INIMetaGroup INIMeta::GetRules()
{
	INIMetaGroup group;
	group.AddINI(GlobalVars::INIFiles::Rules());
	group.AddINI(GlobalVars::INIFiles::CurrentDocument());
	return group;
}

INIMetaGroup INIMeta::GetDocument()
{
	INIMetaGroup group;
	group.AddINI(&GlobalVars::INIFiles::CurrentDocument());
	return group;
}

bool INIMeta::isNullOrEmpty(const FA2::CString& value)
{
	return !value.GetLength() || value == "none" || value == "<none>";
}

IndiceStorage INIMeta::GetIndicies(const INIClass& ini, const char* pSection)
{
	std::map<unsigned int, FA2::CString> collection;
	IndiceStorage ret;
	auto const section = ini.TryGetSection(pSection);
	if (!section) {
		return ret;
	}
	for (auto const& ent : section->EntriesDictionary) {
		auto const& indexDict = section->IndicesDictionary;
		auto const& idxitr = indexDict.find(ent.first);
		if (idxitr != indexDict.end())
			collection[idxitr->second] = idxitr->first;
	}
	ret.reserve(collection.size());
	for (auto& x : collection) {
		ret.push_back(x.second);
	}
	return ret;
}