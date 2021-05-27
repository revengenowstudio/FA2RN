#pragma once

#include <AFXWIN.H>
#include <MFC/FA2CString.h>
#include <type_traits>

FA2MFC_BEGIN

FA2MFC_CLASS(CComboBox) : /*public*/ ::CComboBox
{
public:
	using ::CComboBox::AddString;
	using ::CComboBox::DeleteString;
	using ::CComboBox::FindString;
	using ::CComboBox::SetItemData;
	using ::CComboBox::GetCurSel;
	using ::CComboBox::SetCurSel;
	using ::CComboBox::EnableWindow;
	using ::CComboBox::SetWindowText;
	using ::CComboBox::GetCount;
	using ::CComboBox::GetItemData;

	void GetLBText(int nIndex, FA2::CString& rString) const { JMP_THIS(0x561616); }
	void GetWindowText(FA2::CString& rString) const { JMP_THIS(0x553134); };


private:
};

FA2MFC_END