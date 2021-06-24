#pragma once

#include <FA2PP.h>
#include <MFC/FA2MFC.h>

FA2MFC_BEGIN
class CComboBox;
FA2MFC_END

namespace ControlMeta
{
	class ComboBoxWrapper
	{
	public:
		ComboBoxWrapper(HWND hwnd) : m_hWnd(hwnd)
		{}

		void Clear();
		void EnableWindow(bool enable);
		void SetWindowText(LPCSTR lpsz);
		HWND GetHWND() { return m_hWnd; }
		int GetLBTextLen(int nIndex) const { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETLBTEXTLEN, nIndex, 0); }
		int GetLBText(int nIndex, LPTSTR lpszText) const { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_GETLBTEXT, nIndex, (LPARAM)lpszText); }
		int GetCurSel() const { return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0); }
		int SetCurSel(int nSelect) { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_SETCURSEL, nSelect, 0); }int GetItemData(int nIndex = -1) const;
		int GetWindowText(LPSTR lpszString, int nMaxCount) { ::GetWindowText(m_hWnd, lpszString, nMaxCount); }
		FA2::CString GetWindowText() const;
		FA2::CString GetLBText(int nIndex) const;
		FA2::CString GetText() const;
		void GetLBText(int nIndex, FA2::CString& rString) const;
		int AddString(LPCTSTR lpszString) { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_ADDSTRING, 0, (LPARAM)lpszString); }
		int DeleteString(UINT nIndex) { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, CB_DELETESTRING, nIndex, 0); }

	private:
		HWND m_hWnd;
	};

	class ComboBox
	{
	public:
		static void Clear(FA2::CComboBox& combobox);
		static void LoadHouses(FA2::CComboBox& combobox, bool bShowIndex = false);
		static void LoadCountries(FA2::CComboBox& combobox, bool bShowIndex = false);
		static void LoadGenericList(FA2::CComboBox& combobox, const char* pSection, bool bShowRegName = false, bool bShowIndex = false);
	};
}