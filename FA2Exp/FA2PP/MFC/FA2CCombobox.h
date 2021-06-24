#pragma once

#include <AFXWIN.H>
#include <FA2PP.h>
#include <MFC/FA2CString.h>
#include <type_traits>

FA2MFC_BEGIN

FA2MFC_CLASS(CComboBox) : public FA2::CWnd
{
public:
_AFXWIN_INLINE int GetCount() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETCOUNT, 0, 0); }
_AFXWIN_INLINE int GetCurSel() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0); }
_AFXWIN_INLINE int SetCurSel(int nSelect)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETCURSEL, nSelect, 0); }
_AFXWIN_INLINE DWORD GetEditSel() const
	{ ASSERT(::IsWindow(m_hWnd)); return ::SendMessage(m_hWnd, CB_GETEDITSEL, 0, 0); }
_AFXWIN_INLINE BOOL LimitText(int nMaxChars)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, CB_LIMITTEXT, nMaxChars, 0); }
_AFXWIN_INLINE BOOL SetEditSel(int nStartChar, int nEndChar)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, CB_SETEDITSEL, 0, MAKELONG(nStartChar, nEndChar)); }
_AFXWIN_INLINE DWORD GetItemData(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return ::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0); }
_AFXWIN_INLINE int SetItemData(int nIndex, DWORD dwItemData)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETITEMDATA, nIndex, (LPARAM)dwItemData); }
_AFXWIN_INLINE void* GetItemDataPtr(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (LPVOID)GetItemData(nIndex); }
_AFXWIN_INLINE int SetItemDataPtr(int nIndex, void* pData)
	{ ASSERT(::IsWindow(m_hWnd)); return SetItemData(nIndex, (DWORD)(LPVOID)pData); }
_AFXWIN_INLINE int GetLBText(int nIndex, LPTSTR lpszText) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText); }
_AFXWIN_INLINE int GetLBTextLen(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0); }
_AFXWIN_INLINE void ShowDropDown(BOOL bShowIt)
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_SHOWDROPDOWN, bShowIt, 0); }
_AFXWIN_INLINE int AddString(LPCTSTR lpszString)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString); }
_AFXWIN_INLINE int DeleteString(UINT nIndex)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_DELETESTRING, nIndex, 0);}
_AFXWIN_INLINE int InsertString(int nIndex, LPCTSTR lpszString)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_INSERTSTRING, nIndex, (LPARAM)lpszString); }
_AFXWIN_INLINE void ResetContent()
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_RESETCONTENT, 0, 0); }
_AFXWIN_INLINE int Dir(UINT attr, LPCTSTR lpszWildCard)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_DIR, attr, (LPARAM)lpszWildCard); }
_AFXWIN_INLINE int FindString(int nStartAfter, LPCTSTR lpszString) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_FINDSTRING, nStartAfter,
		(LPARAM)lpszString); }
_AFXWIN_INLINE int SelectString(int nStartAfter, LPCTSTR lpszString)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SELECTSTRING,
		nStartAfter, (LPARAM)lpszString); }
_AFXWIN_INLINE void Clear()
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_CLEAR, 0, 0); }
_AFXWIN_INLINE void Copy()
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_COPY, 0, 0); }
_AFXWIN_INLINE void Cut()
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_CUT, 0, 0); }
_AFXWIN_INLINE void Paste()
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, WM_PASTE, 0, 0); }
_AFXWIN_INLINE int SetItemHeight(int nIndex, UINT cyItemHeight)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0)); }
_AFXWIN_INLINE int GetItemHeight(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETITEMHEIGHT, nIndex, 0L); }
_AFXWIN_INLINE int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
_AFXWIN_INLINE int SetExtendedUI(BOOL bExtended )
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETEXTENDEDUI, bExtended, 0L); }
_AFXWIN_INLINE BOOL GetExtendedUI() const
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, CB_GETEXTENDEDUI, 0, 0L); }
_AFXWIN_INLINE void GetDroppedControlRect(LPRECT lprect) const
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_GETDROPPEDCONTROLRECT, 0, (DWORD)lprect); }
_AFXWIN_INLINE BOOL GetDroppedState() const
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, CB_GETDROPPEDSTATE, 0, 0L); }
_AFXWIN_INLINE LCID GetLocale() const
	{ ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, CB_GETLOCALE, 0, 0); }
_AFXWIN_INLINE LCID SetLocale(LCID nNewLocale)
	{ ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, CB_SETLOCALE, (WPARAM)nNewLocale, 0); }
#if (WINVER >= 0x400)
_AFXWIN_INLINE int GetTopIndex() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETTOPINDEX, 0, 0); }
_AFXWIN_INLINE int SetTopIndex(int nIndex)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETTOPINDEX, nIndex, 0); }
_AFXWIN_INLINE int InitStorage(int nItems, UINT nBytes)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_INITSTORAGE, (WPARAM)nItems, nBytes); }
_AFXWIN_INLINE void SetHorizontalExtent(UINT nExtent)
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, CB_SETHORIZONTALEXTENT, nExtent, 0); }
_AFXWIN_INLINE UINT GetHorizontalExtent() const
	{ ASSERT(::IsWindow(m_hWnd)); return (UINT)::SendMessage(m_hWnd, CB_GETHORIZONTALEXTENT, 0, 0); }
_AFXWIN_INLINE int SetDroppedWidth(UINT nWidth)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETDROPPEDWIDTH, nWidth, 0); }
_AFXWIN_INLINE int GetDroppedWidth() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETDROPPEDWIDTH, 0, 0); }
#endif

	void GetLBText(int nIndex, FA2::CString& rString) const { JMP_THIS(0x561616); }
	FA2::CString GetLBText(int nIndex) const
	{
		  FA2::CString ret;
		  GetLBText(nIndex, ret);
		  return ret;
	}
	FA2::CString GetText() const
	{
		auto const curSel = GetCurSel();
		return curSel != -1 ? GetLBText(curSel) : GetWindowText();
	}//43EBC0, looks like FA2 own wrapper

private:
};

FA2MFC_END