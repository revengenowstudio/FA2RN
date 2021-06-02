#pragma once

//#include "../FA2.h"

#include <FA2PPCore.h>
#include <MFC/FA2CString.h>
#include <Structure/FAMap.h>
#include <fstream>
#include <Helper/CompileTime.h>
//#include <minwindef.h>

// We can only use C++14 standard for now since our FATree & FAMap
// was a mess, and higher standards are more strict and we cannot
// pass the complie for some reason.
// secsome - 2020/11/3

// Remember that we cannot call CTOR or DTOR for any FAMap/FATree
// because nil & nilrefs haven't been analysed yet.
// Consider to use : auto& iRules = GlobalVars::INIFiles::Rules();

class INISectionEntriesComparator;
class INISection;

using INIDict = std::FAMap<FA2::CString, INISection, 0x5D8CB4, 0>;
using INIStringDict = std::FAMap<FA2::CString, FA2::CString, 0x5D8CB0, 0x5D8CAC, INISectionEntriesComparator>;
using INIIndiceDict = std::FAMap<FA2::CString, unsigned int, 0x5D8CA8, 0x5D8CA4, INISectionEntriesComparator>;

class INIMapFieldUpdate
{
private:
	static constexpr DWORD _H = 0x72CBF8;
	struct _S
	{
		void UpdateMapFieldData(int flag) { JMP_THIS(0x49C280); }
	};

public:
	static void UpdateMapFieldData(int flag)
	{
		_S* _X = (_S*)_H;
		_X->UpdateMapFieldData(flag);
	}
};

class INISectionEntriesComparator
{
public:

	static bool __stdcall __compare(const FA2::CString& a1, const FA2::CString& a2)
		{ JMP_STD(0x452230); }

	bool operator()(const FA2::CString& s1, const FA2::CString& s2) const
	{
		return __compare(s1, s2);
	}
};

INIStringDict::_Pairib INIStringDict::insert(const INIStringDict::value_type& pair) { JMP_THIS(0x40A010); }
INIDict::_Pairib INIDict::insert(const INIDict::value_type& pair) { JMP_THIS(0x4026D0); }
FA2::CString& INIStringDict::operator[](const FA2::CString& Key) { JMP_THIS(0x407CA0); }

class NOVTABLE INISection {
public:
	INISection() { JMP_THIS(0x452880); }
	INISection(const INISection& another) { JMP_THIS(0x4021C0); }

	virtual ~INISection()
		{ JMP_THIS(0x452B20); }

	int GetItemCount(FA2::CString Key) const//0 means section exists but no content, -1 means section not exists
		{ JMP_THIS(0x4023B0); }

	FA2::CString& operator[](const FA2::CString& Key)
	{ return this->EntriesDictionary[Key]; }

//private:
	INIStringDict EntriesDictionary;
	INIIndiceDict IndicesDictionary;
};

class NOVTABLE INIClass
{
public:
	INIClass() 
		{ JMP_THIS(0x452880); }

	virtual ~INIClass() = default;

	static INIClass* GetMapDocument(bool bUpdateMapField = false)
	{
		if (bUpdateMapField)
			INIMapFieldUpdate::UpdateMapFieldData(1);
		return reinterpret_cast<INIClass*>(0x7ACC80);
	}

	// Debug function
	std::FAMap<FA2::CString, INISection, 0x5D8CB4, 0>& GetMap()
	{
		return data;
	}

	bool DebugToFile(const char* path)
	{
		std::ofstream fout;
		fout.open(path, std::ios::out);
		if (!fout.is_open())
			return false;
		for (auto& itrsec : data)
		{
			fout << "[" << itrsec.first << "]\n";
			for (auto& entries : itrsec.second.EntriesDictionary)
			{
				fout << entries.first << "=" << entries.second << "\n";
			}
			fout << "\n";
		}
		fout.flush();
		fout.close();
		return true;
	}

	static FA2::CString* __cdecl GetAvailableIndex(FA2::CString*) { JMP_STD(0x446520); }
	static FA2::CString* GetAvailableKey(FA2::CString* ret,const char* pSection) { JMP_STD(0x499E80); }

	static FA2::CString GetAvailableIndex()
	{
		FA2::CString ret;
		GetAvailableIndex(&ret);
		return ret;
	}
	static FA2::CString GetAvailableKey(const char* pSection)
	{
		FA2::CString ret;
		GetAvailableKey(&ret, pSection);
		return ret;
	}

	bool SectionExists(const char* pSection)
	{
		return data.find(pSection) != data.end();
	}

	int GetKeyCount(const char* pSection)
	{
		auto itr = data.find(pSection);
		if (itr != data.end())
			return itr->second.EntriesDictionary.size();
		return 0;
	}

	std::pair<INIStringDict::iterator, bool> Insert(INIStringDict& dict, const char* pKey, const char* pValue)
	{
		auto const& ins = std::make_pair(pKey, pValue);
		return dict.insert(ins);
	}
	typename INIDict::iterator Insert(const FA2::CString& ID, const INISection& section)
	{
		INIDict::value_type ins{ ID, section };
		auto const& ret = data.insert(ins);
		return ret.first;
	}

	bool WriteString(const char* pSection, const char* pKey, const char* pValue)
	{
		auto itr = data.find(pSection);
		if (itr == data.end()) { 
			return false; 
		}
		auto& dict = itr->second.EntriesDictionary;
		auto pair = Insert(dict, pKey, pValue);
		if (!pair.second) {
			pair.first->second = pValue;
		}
		return true;
	}

	// use it like this to avoid CTOR and crash:
	// auto &iSection = iINI.TryGetSection("E1");
	// secsome - 2020/11/3
	const INISection* TryGetSection(const char* pSection) const
	{
		auto itr = data.find(pSection);
		if (itr != data.end()) {
			return &itr->second;
		}
		return nullptr;
	}
	//this may cause insert of new section
	//INISection& TryGetSection(const FAString& SectionName)
	//	{ JMP_THIS(0x4020A0); }


	bool TryGetString(const char* pSection, const char* pKey, FA2::CString& ret) const
	{
		auto itrSection = data.find(pSection);
		if (itrSection != data.end()) {
			auto pEntries = &itrSection->second.EntriesDictionary;
			auto itrKey = pEntries->find(pKey);
			if (itrKey != pEntries->end()) {
				ret = itrKey->second;
				return true;
			}
		}
		return false;
	}

	FA2::CString GetString(const char* pSection, const char* pKey, const char* pDefault = "") const {
		FA2::CString ret;
		if (!TryGetString(pSection, pKey, ret)) {
			ret = pDefault;
		}
		return ret;
	}

	int GetInteger(const char* pSection, const char* pKey, int nDefault = 0) {
		FA2::CString& pStr = this->GetString(pSection, pKey, "");
		int ret = 0;
		if (sscanf_s(pStr, "%d", &ret) == 1)
			return ret;
		return nDefault;
	}

	float GetSingle(const char* pSection, const char* pKey, float nDefault = 0) {
		FA2::CString& pStr = this->GetString(pSection, pKey, "");
		float ret = 0;
		if (sscanf_s(pStr, "%f", &ret) == 1)
			return ret;
		return nDefault;
	}

	double GetDouble(const char* pSection, const char* pKey, double nDefault = 0) {
		FA2::CString& pStr = this->GetString(pSection, pKey, "");
		double ret = 0;
		if (sscanf_s(pStr, "%lf", &ret) == 1)
			return ret;
		return nDefault;
	}

	bool GetBool(const char* pSection, const char* pKey, bool nDefault = false) {
		FA2::CString& pStr = this->GetString(pSection, pKey, "");
		switch (toupper(static_cast<unsigned char>(*pStr)))
		{
		case '1':
		case 'T':
		case 'Y':
			return true;
		case '0':
		case 'F':
		case 'N':
			return false;
		default:
			return nDefault;
		}
	}
private:
		INIDict data; // no idea about the nilrefs
		char filePath[MAX_PATH];
};