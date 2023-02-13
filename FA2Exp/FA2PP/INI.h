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

template<typename T>
class FA2Allocater
{
public:
	using value_type = T;
	constexpr FA2Allocater() noexcept {}

	constexpr FA2Allocater(const FA2Allocater&) noexcept = default;
	template <class _Other>
	constexpr FA2Allocater(const FA2Allocater<_Other>&) noexcept {}

	_NODISCARD __declspec(allocator) T* allocate(const size_t size)
	{
		return reinterpret_cast<T*>(FAMemory::Allocate(sizeof(T) * size));
	}

	void deallocate(T* const _Ptr, const size_t _Count)
	{
		// no overflow check on the following multiply; we assume _Allocate did that check
		FAMemory::Deallocate(_Ptr);
	}

	template <class _Objty, class... _Types>
	void construct(_Objty* const _Ptr, _Types&&... _Args)
	{
		::new (_Voidify_iter(_Ptr)) _Objty(std::forward<_Types>(_Args)...);
	}

	template <class Ty>
	void destroy(Ty* const ptr)
	{
		ptr->~Ty();
	}

	_NODISCARD size_t max_size() const noexcept
	{
		return static_cast<size_t>(-1) / sizeof(T);
	}
};

using INIDict = std::FAMap<FA2::CString, INISection, 0x5D8CB4, 0>;
using INIStringDict 
	= std::FAMap<FA2::CString, FA2::CString, 0x5D8CB0, 0x5D8CAC, 
		INISectionEntriesComparator, 
		FA2Allocater<std::pair<FA2::CString, FA2::CString>>
		>;
using INIIndiceDict 
	= std::FAMap<FA2::CString, unsigned int, 0x5D8CA8, 0x5D8CA4,
		INISectionEntriesComparator, FA2Allocater<std::pair<FA2::CString, unsigned int>>
		>;

class INIHelper
{
public:
	static bool StingToBool(const FA2::CString& str, bool def)
	{
		switch (toupper(static_cast<unsigned char>(*str))) {
			case '1':
			case 'T':
			case 'Y':
				return true;
			case '0':
			case 'F':
			case 'N':
				return false;
			default:
				return def;
		}
	}
	static int StringToInteger(const FA2::CString& str, int def)
	{
		int ret = 0;
		if (sscanf_s(str, "%d", &ret) == 1) {
			return ret;
		}
		return def;
	}
};

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
	INISection() noexcept : INISection(noinit_t()) { JMP_THIS(0x452880); }
	INISection(const INISection& another) { JMP_THIS(0x4021C0); }
	INISection(INISection&& another) :
		EntriesDictionary(std::move(another.EntriesDictionary)),
		IndicesDictionary(std::move(another.IndicesDictionary))
	{
	}

	virtual ~INISection()
		{ JMP_THIS(0x452B20); }

	int GetItemCount(const FA2::CString& Key) const//0 means section exists but no content, -1 means section not exists
		{ JMP_THIS(0x4023B0); }

	size_t Size() const { return EntriesDictionary.size(); }

	FA2::CString& operator[](const FA2::CString& Key)
	{ return this->EntriesDictionary[Key]; }

	void Insert(const FA2::CString& key, const FA2::CString& value)
	{
		EntriesDictionary.insert({ key, value });
		IndicesDictionary.insert({ key, EntriesDictionary.size() });
	}

private:
	explicit __forceinline INISection(noinit_t) noexcept {}

public:
//private:
		// In fact we can use a union here. However, to make it
	// being more secured, just use a char array here and 
	// access it by using functions.
	union
	{
		BYTE _EntriesDictionary[sizeof(INIStringDict)];
		INIStringDict EntriesDictionary;
	};
	union {
		BYTE _IndicesDictionary[sizeof(INIIndiceDict)];
		INIIndiceDict IndicesDictionary;
	};
};

class NOVTABLE INIClass
{
public:
	INIClass(noinit_t())
		{ JMP_THIS(0x452880); }

	virtual ~INIClass() = default;

	static INIClass* GetMapDocument(bool bUpdateMapField = false)
	{
		if (bUpdateMapField)
			INIMapFieldUpdate::UpdateMapFieldData(1);
		return reinterpret_cast<INIClass*>(0x7ACC80);
	}

	// Debug function
	auto& Data() { return data; }

	#if 0
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
	#endif

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
		if (itr != data.end()) {
			return itr->second.EntriesDictionary.size();
		}
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
	//typename INIDict::iterator Insert(const FA2::CString& ID, INISection&& section)
	//{
	//	INIDict::value_type ins{ ID, std::move(section) };
	//	auto const& ret = data.insert(ins);
	//	return ret.first;
	//}

	void Remove(const FA2::CString& section)
	{
		data.erase(section);
	}

	void Remove(const FA2::CString& section, const FA2::CString& key)
	{
		auto it = data.find(section);
		if (it != data.end()) {
			it->second.EntriesDictionary.erase(key);
			it->second.IndicesDictionary.erase(key);
		}
	}

	void WriteString(const char* pSection, const char* pKey, const char* pValue)
	{
		auto itr = data.find(pSection);
		if (itr == data.end()) { 
			itr = Insert(pSection, {});
		}
		auto& dict = itr->second.EntriesDictionary;
		auto pair = Insert(dict, pKey, pValue);
		if (!pair.second) {
			pair.first->second = pValue;
		}
	}

	void SetIndice(const char* pSection, const char* pKey, unsigned index)
	{
		auto it = data.find(pSection);
		if (it != data.end()) {
			it->second.IndicesDictionary.insert({ pKey, index });
		}
	}

	bool Save(const char* pFilePath)
	{
		std::ofstream fout(pFilePath);
		if (!fout.is_open()) {
			return false;
		}
		for (auto const& section : data) {
			fout << "[" << section.first << "]\n";
			for (auto const& pair : section.second.EntriesDictionary) {
				fout << pair.first << "=" << pair.second << "\n";
			}
			fout << "\n";
		}

		fout.flush();
		fout.close();
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
		return INIHelper::StringToInteger(this->GetString(pSection, pKey, ""), nDefault);
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
		return INIHelper::StingToBool(this->GetString(pSection, pKey, ""), nDefault);
	}
private:
		INIDict data; // no idea about the nilrefs
		char filePath[MAX_PATH];
};