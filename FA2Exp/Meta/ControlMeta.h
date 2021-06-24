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
		int GetItemData(int nIndex = -1)	{ 
			if (nIndex < 0) {
				nIndex = GetCurSel();
			}
			return ::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0); 
		}
		int GetWindowText(LPSTR lpszString, int nMaxCount) { ::GetWindowText(m_hWnd, lpszString, nMaxCount); }
		FA2::CString GetWindowText() const {
			FA2::CString ret;
			auto const len = GetWindowTextLengthA(this->m_hWnd);
			auto const bufferSiz = len + 1;
			::GetWindowText(this->m_hWnd, ret.GetBufferSetLength(len), bufferSiz);
			ret.ReleaseBuffer();
			return ret;
		}
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
		}
		void GetLBText(int nIndex, FA2::CString& rString) const {
			auto const dataLen = GetLBTextLen(nIndex);
			GetLBText(nIndex, rString.GetBufferSetLength(dataLen));
			rString.ReleaseBuffer();
		}

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