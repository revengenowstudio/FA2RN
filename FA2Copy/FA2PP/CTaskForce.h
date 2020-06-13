#pragma once
#include <FA2PP.h>

class NOVTABLE CTaskForce : public FA2CDialog
{
public:
	CTaskForce() = default;

	void UpdateMemberTypeComboBox()
		{ JMP_THIS(0x4E1A10); }

	//event handlers
	void OnMemberTypeChanged()
		{ JMP_THIS(0x4E41A0); }

	//member properties
	CComboBox	CCBMemberType;
	CListBox		CLBItems;
	CComboBox	CCBTaskType;
	CString CString_110;
	CString CString_114;
	DWORD last;
};