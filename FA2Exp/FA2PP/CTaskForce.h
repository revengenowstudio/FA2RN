#pragma once
#include <FA2PP.h>

class NOVTABLE CTaskForce : public FA2CDialog
{
public:
	CTaskForce() = default;

	void UpdateMemberType() { JMP_THIS(0x4E1A10); }

	//event handlers
	void OnMemberEditChanged() { JMP_THIS(0x4E41A0); }
	void OnTaskForceNameChanged() { JMP_THIS(0x4E3F00); }
	void OnMemberCountChanged() { JMP_THIS(0x4E3B00); }
	void OnGroupIDChanged() { JMP_THIS(0x4E5550); }

	//member properties
	CComboBox		CCBMemberType;
	CListBox		CLBItems;
	CComboBox		CCBTaskType;
	CString			CStringGroup;
	CString			CStringName;
	CString			CStringNum;
};