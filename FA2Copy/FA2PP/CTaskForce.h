#pragma once
#include <FA2PP.h>

class NOVTABLE CTaskForce : CDialog
{
public:
	CTaskForce() = default;

	void UpdateMemberTypeComboBox()
		{ JMP_THIS(0x4E1A10); }


	//member properties
	CComboBox	CCBMemberType;
	CListBox		CLBItems;
	CComboBox	CCBTaskType;
	CString CString_110;
	CString CString_114;
	DWORD last;
};