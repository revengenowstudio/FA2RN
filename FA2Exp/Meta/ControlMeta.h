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
		int GetCurSel() { return (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0); }
		int GetItemData(int nIndex = -1)	{ 
			if (nIndex < 0) {
				nIndex = GetCurSel();
			}
			return ::SendMessage(m_hWnd, CB_GETITEMDATA, nIndex, 0); 
		}
		int GetWindowText(LPSTR lpszString, int nMaxCount) { ::GetWindowText(m_hWnd, lpszString, nMaxCount); }
		FA2::CString GetWindowText() {
			FA2::CString ret;
			auto const len = GetWindowTextLengthA(this->m_hWnd);
			auto const bufferSiz = len + 1;
			::GetWindowText(this->m_hWnd, ret.GetBufferSetLength(len), bufferSiz);
			ret.ReleaseBuffer();
			return ret;
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