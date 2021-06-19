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
		ComboBoxWrapper(HWND hwnd) : m_hwnd(hwnd)
		{}

		void Clear();
		void EnableWindow(bool enable);
		void SetWindowText(LPCSTR lpsz);

	private:
		HWND m_hwnd;
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