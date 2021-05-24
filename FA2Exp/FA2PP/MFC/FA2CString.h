#pragma once

#include <AFX.H>
#include <MFC/FA2MFC.h>
#include <type_traits>

FA2MFC_BEGIN

FA2MFC_STRUCT(CStringData)
{
    long nRefs;     // reference count
    int nDataLength;
    int nAllocLength;
    // TCHAR data[nAllocLength]

    TCHAR* data()
    {
        return (TCHAR*)(this + 1);
    }
};

FA2MFC_CLASS(CString) : public ::CString
{
public:
    CString() FA2MFC_THISCALL(0x406F70);
	CString(const CString& stringSrc) FA2MFC_THISCALL(0x555C84);
	CString(TCHAR ch, int nRepeat = 1) = delete;//not analyzed
	CString(LPCSTR lpsz) FA2MFC_THISCALL(0x555F7D);
    CString(LPCWSTR lpsz) FA2MFC_THISCALL(0x555FCF);
	CString(LPCTSTR lpch, int nLength) FA2MFC_THISCALL(0x54F598);
	CString(const unsigned char* psz) = delete;//not analyzed
	~CString() FA2MFC_THISCALL(0x555F0F);

    const CString& operator=(TCHAR ch)
        FA2MFC_THISCALL(0x54F5CC);

    int Delete(int at, int length)
        FA2MFC_THISCALL(0x54F5E1);

    int Insert(int at, LPCSTR lpsz)
        FA2MFC_THISCALL(0x54F639);

    int Replace(LPCSTR from, LPCSTR to)
        FA2MFC_THISCALL(0x54F6EE);

    CString* Right(CString* stringMem, int at)
        FA2MFC_THISCALL(0x54F851);

    CString Right(int at)
        { CString ret; Right(&ret, at); return ret; }

    CString* Left(CString* stringMem, int at)
        FA2MFC_THISCALL(0x54F8CD);

    CString Left(int at)
        { CString ret; Left(&ret, at); return ret; }

    int ReverseFind(TCHAR ch) const
        FA2MFC_THISCALL(0x54F945);

    int Find(LPCTSTR lpszSub) const
        FA2MFC_THISCALL(0x54F967);

    int Find(LPCTSTR lpszSub, int from) const
        FA2MFC_THISCALL(0x54F975);

    void TrimRight()
        FA2MFC_THISCALL(0x54F9A0);

    void TrimLeft()
        FA2MFC_THISCALL(0x54F9EC);

    void Trim()
        { TrimLeft(); TrimRight(); }

	// simple formatting
	void FA2MFC_CDECL Format(LPCTSTR lpszFormat, ...) {
		va_list args;
		va_start(args, lpszFormat);
		format(lpszFormat, args);
		va_end(args);
	}

    void AllocBuffer(int nLen)
        FA2MFC_THISCALL(0x555D7C);

	CStringData* GetData() const {
		FA2MFC_ASSERT(m_pchData != NULL); return ((CStringData*)m_pchData) - 1;
	}

    void FreeData()
        FA2MFC_THISCALL(0x555DFE);

    void Release()
        FA2MFC_THISCALL(0x555E46);

    static void FA2MFC_API Release(CStringData* pStringData)
        FA2MFC_STDCALL(0x555E77);

    void Empty() const
        FA2MFC_THISCALL(0x555E9A);

	int GetLength() const {
		return GetData()->nDataLength;
	}

    void CopyBeforeWrite()
        FA2MFC_THISCALL(0x555EB8);

    void AllocBeforeWrite(int nLen)
        FA2MFC_THISCALL(0x555EE6);

    void AllocCopy(CString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
        FA2MFC_THISCALL(0x555F39);

    void AssignCopy(int nSrcLen, LPCTSTR lpszSrcData)
        FA2MFC_THISCALL(0x55601B);

    const CString& operator=(CString& str)
        FA2MFC_THISCALL(0x556048);

    const CString& operator=(LPCSTR lpsz)
        FA2MFC_THISCALL(0x556098);

    const CString& operator=(LPCWSTR lpsz)
        FA2MFC_THISCALL(0x5560BF);

	TCHAR operator[](int nIndex) const // same as GetAt
		{ return GetAt(nIndex); }

    void ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data)
        FA2MFC_THISCALL(0x556100);

    friend CString FA2MFC_API operator+(const CString& string1,
        const CString& string2)
        FA2MFC_STDCALL(0x55613E);

    friend CString FA2MFC_API operator+(const CString& string, LPCTSTR lpsz)
        FA2MFC_STDCALL(0x5561A4);

    friend CString FA2MFC_API operator+(LPCTSTR lpsz, const CString& string)
        FA2MFC_STDCALL(0x556218);

    void ConcatInPlace(int nSrcLen, LPCTSTR lpszSrcData)
        FA2MFC_THISCALL(0x55628C);

    //const CString& operator+=(LPCTSTR lpsz)
    //    FA2MFC_THISCALL(0x5562EB);
	const CString& operator+=(LPCTSTR lpsz) = delete;//this function causes dead loop, do not use until we find out why

    const CString& operator+=(TCHAR ch)
        FA2MFC_THISCALL(0x556312);

    const CString& operator+=(const CString& string)
        FA2MFC_THISCALL(0x556327);

	// Compare helpers
	friend bool FA2MFC_API operator==(const CString& s1, const CString& s2) { return s1.Compare(s2) == 0; }
	friend bool FA2MFC_API operator==(const CString& s1, LPCTSTR s2) { return s1.Compare(s2) == 0; }
	friend bool FA2MFC_API operator==(LPCTSTR s1, const CString& s2) { return s2.Compare(s1) == 0; }
	friend bool FA2MFC_API operator!=(const CString& s1, const CString& s2) { return s1.Compare(s2) != 0; }
	friend bool FA2MFC_API operator!=(const CString& s1, LPCTSTR s2) { return s1.Compare(s2) != 0; }
	friend bool FA2MFC_API operator!=(LPCTSTR s1, const CString& s2) { return s2.Compare(s1) != 0; }
	friend bool FA2MFC_API operator<(const CString& s1, const CString& s2) { return s1.Compare(s2) < 0; }
	friend bool FA2MFC_API operator<(const CString& s1, LPCTSTR s2) { return s1.Compare(s2) < 0; }
	friend bool FA2MFC_API operator<(LPCTSTR s1, const CString& s2) { return s2.Compare(s1) > 0; }
	friend bool FA2MFC_API operator>(const CString& s1, const CString& s2) { return s1.Compare(s2) > 0; }
	friend bool FA2MFC_API operator>(const CString& s1, LPCTSTR s2) { return s1.Compare(s2) > 0; }
	friend bool FA2MFC_API operator>(LPCTSTR s1, const CString& s2) { return s2.Compare(s1) < 0; }
	friend bool FA2MFC_API operator<=(const CString& s1, const CString& s2) { return s1.Compare(s2) <= 0; }
	friend bool FA2MFC_API operator<=(const CString& s1, LPCTSTR s2) { return s1.Compare(s2) <= 0; }
	friend bool FA2MFC_API operator<=(LPCTSTR s1, const CString& s2) { return s2.Compare(s1) >= 0; }
	friend bool FA2MFC_API operator>=(const CString& s1, const CString& s2) { return s1.Compare(s2) >= 0; }
	friend bool FA2MFC_API operator>=(const CString& s1, LPCTSTR s2) { return s1.Compare(s2) >= 0; }
	friend bool FA2MFC_API operator>=(LPCTSTR s1, const CString& s2) { return s2.Compare(s1) <= 0; }

	TCHAR GetAt(int nIndex) const
	{
		FA2MFC_ASSERT(nIndex >= 0);
		FA2MFC_ASSERT(nIndex < GetData()->nDataLength);
		return m_pchData[nIndex];
	}

    LPTSTR GetBuffer(int nMinBufLength)
        FA2MFC_THISCALL(0x55633F);

    void ReleaseBuffer(int nNewLength = -1)
        FA2MFC_THISCALL(0x55638E);

    LPTSTR GetBufferSetLength(int nNewLength)
        FA2MFC_THISCALL(0x5563B6);

    int Find(TCHAR ch) const
        FA2MFC_THISCALL(0x5563D6);

    int Find(TCHAR ch, int from) const
        FA2MFC_THISCALL(0x5563E4);

    void MakeUpper()
        FA2MFC_THISCALL(0x556411);

    void MakeLower()
        FA2MFC_THISCALL(0x556423);

    void SetAt(int nIndex, TCHAR ch)
        FA2MFC_THISCALL(0x556423);

    BOOL LoadString(UINT nID)          // load from string resource, 255 chars max
        FA2MFC_THISCALL(0x556507);

	operator LPCTSTR() const           // as a C string
		{ return m_pchData; }
private:
	void FA2MFC_CDECL format(LPCTSTR lpszFormat, const va_list args) {
		size_t len = 0;
		len = vsnprintf(nullptr, 0, lpszFormat, args);
		if (len) {
			vsnprintf(GetBufferSetLength(len + 1), len + 1, lpszFormat, args);
		}
	}
};

//BOOL _PPMFC_API AfxExtractSubString(CString& rString, LPCTSTR lpszFullString,
//    int iSubString, TCHAR chSep = '\n')
//    FA2MFC_STDCALL(0x5565B2);
//
//void _PPMFC_API AfxFormatStrings(CString& rString, UINT nIDS,
//    LPCTSTR const* rglpsz, int nString)
//    FA2MFC_STDCALL(0x55FBB7);
//
//void _PPMFC_API AfxFormatStrings(CString& rString, LPCTSTR lpszFormat,
//    LPCTSTR const* rglpsz, int nString)
//    FA2MFC_STDCALL(0x55FBF1);
//
//void _PPMFC_API AfxFormatString1(CString& rString, UINT nIDS, LPCTSTR lpsz1)
//    FA2MFC_STDCALL(0x55FCF0);
//
//void _PPMFC_API AfxFormatString2(CString& rString, UINT nIDS,
//    LPCTSTR lpsz1, LPCTSTR lpsz2)
//    FA2MFC_STDCALL(0x55FD07);

FA2MFC_END