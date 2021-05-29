#pragma once
#include <FA2PP.h>

FA2MFC_BEGIN


FA2MFC_CLASS(CListBox) : public FA2::CWnd
{
public:
_AFXWIN_INLINE int GetCount() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0); }
_AFXWIN_INLINE int GetCurSel() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETCURSEL, 0, 0); }
_AFXWIN_INLINE int SetCurSel(int nSelect)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETCURSEL, nSelect, 0); }
_AFXWIN_INLINE int GetHorizontalExtent() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETHORIZONTALEXTENT,
		0, 0); }
_AFXWIN_INLINE void SetHorizontalExtent(int cxExtent)
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETHORIZONTALEXTENT, cxExtent, 0); }
_AFXWIN_INLINE int GetSelCount() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETSELCOUNT, 0, 0); }
_AFXWIN_INLINE int GetSelItems(int nMaxItems, LPINT rgIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex); }
_AFXWIN_INLINE int GetTopIndex() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETTOPINDEX, 0, 0); }
_AFXWIN_INLINE int SetTopIndex(int nIndex)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETTOPINDEX, nIndex, 0);}
_AFXWIN_INLINE DWORD GetItemData(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return ::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0); }
_AFXWIN_INLINE int SetItemData(int nIndex, DWORD dwItemData)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETITEMDATA, nIndex, (LPARAM)dwItemData); }
_AFXWIN_INLINE void* GetItemDataPtr(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (LPVOID)::SendMessage(m_hWnd, LB_GETITEMDATA, nIndex, 0); }
_AFXWIN_INLINE int SetItemDataPtr(int nIndex, void* pData)
	{ ASSERT(::IsWindow(m_hWnd)); return SetItemData(nIndex, (DWORD)(LPVOID)pData); }
_AFXWIN_INLINE int GetItemRect(int nIndex, LPRECT lpRect) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETITEMRECT, nIndex, (LPARAM)lpRect); }
_AFXWIN_INLINE int GetSel(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETSEL, nIndex, 0); }
_AFXWIN_INLINE int SetSel(int nIndex, BOOL bSelect)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETSEL, bSelect, nIndex); }
_AFXWIN_INLINE int GetText(int nIndex, LPTSTR lpszBuffer) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer); }
_AFXWIN_INLINE int GetTextLen(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETTEXTLEN, nIndex, 0); }
_AFXWIN_INLINE void SetColumnWidth(int cxWidth)
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETCOLUMNWIDTH, cxWidth, 0); }
_AFXWIN_INLINE BOOL SetTabStops(int nTabStops, LPINT rgTabStops)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops); }
_AFXWIN_INLINE void SetTabStops()
	{ ASSERT(::IsWindow(m_hWnd)); VERIFY(::SendMessage(m_hWnd, LB_SETTABSTOPS, 0, 0)); }
_AFXWIN_INLINE BOOL SetTabStops(const int& cxEachStop)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop); }
_AFXWIN_INLINE int SetItemHeight(int nIndex, UINT cyItemHeight)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0)); }
_AFXWIN_INLINE int GetItemHeight(int nIndex) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETITEMHEIGHT, nIndex, 0L); }
_AFXWIN_INLINE int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind); }
_AFXWIN_INLINE int GetCaretIndex() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0L); }
_AFXWIN_INLINE int SetCaretIndex(int nIndex, BOOL bScroll)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0)); }
_AFXWIN_INLINE int AddString(LPCTSTR lpszItem)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_ADDSTRING, 0, (LPARAM)lpszItem); }
_AFXWIN_INLINE int DeleteString(UINT nIndex)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_DELETESTRING, nIndex, 0); }
_AFXWIN_INLINE int InsertString(int nIndex, LPCTSTR lpszItem)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_INSERTSTRING, nIndex, (LPARAM)lpszItem); }
_AFXWIN_INLINE void ResetContent()
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_RESETCONTENT, 0, 0); }
_AFXWIN_INLINE int Dir(UINT attr, LPCTSTR lpszWildCard)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_DIR, attr, (LPARAM)lpszWildCard); }
_AFXWIN_INLINE int FindString(int nStartAfter, LPCTSTR lpszItem) const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_FINDSTRING,
		nStartAfter, (LPARAM)lpszItem); }
_AFXWIN_INLINE int SelectString(int nStartAfter, LPCTSTR lpszItem)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_SELECTSTRING,
		nStartAfter, (LPARAM)lpszItem); }
_AFXWIN_INLINE int SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
	{ ASSERT(::IsWindow(m_hWnd)); return bSelect ?
		(int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nFirstItem, nLastItem) :
		(int)::SendMessage(m_hWnd, LB_SELITEMRANGEEX, nLastItem, nFirstItem); }
_AFXWIN_INLINE void SetAnchorIndex(int nIndex)
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LB_SETANCHORINDEX, nIndex, 0); }
_AFXWIN_INLINE int GetAnchorIndex() const
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_GETANCHORINDEX, 0, 0); }
_AFXWIN_INLINE LCID GetLocale() const
	{ ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0); }
_AFXWIN_INLINE LCID SetLocale(LCID nNewLocale)
	{ ASSERT(::IsWindow(m_hWnd)); return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0); }
#if (WINVER >= 0x400)
_AFXWIN_INLINE int InitStorage(int nItems, UINT nBytes)
	{ ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, LB_INITSTORAGE, (WPARAM)nItems, nBytes); }
#endif
};

FA2MFC_END