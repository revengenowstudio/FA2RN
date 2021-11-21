#pragma once

#include <FA2PP.h>

class NOVTABLE CINIEditor : public FA2::CDialog
{
public:
	CINIEditor() = default;

	//member properties
	CEdit				CETValue; // 92
	CListBox		CLBKeys; // 152
	CButton			CBNInsertAnotherINIFileContent; // 212
	CButton			CBNDeleteSection; // 272
	CButton			CBNDeleteKey; // 332
	CButton			CBNAddSection; // 392
	CButton			CBNAddKey; // 452
	CComboBox	CCBSectionList; // 512


	virtual void __thiscall DoDataExchange(CDataExchange* pDX) override
	{
		JMP_STD(0x40A440);
	}
};