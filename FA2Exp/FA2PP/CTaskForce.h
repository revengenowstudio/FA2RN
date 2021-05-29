#pragma once
#include <FA2PP.h>
#include <MFC/FA2CCombobox.h>
#include <MFC/FA2CListBox.h>

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
	FA2::CComboBox	CCBMemberType;
	FA2::CListBox		CLBItems;
	FA2::CComboBox	CCBTaskType;
	FA2::CString	CStringGroup;
	FA2::CString	CStringName;
	FA2::CString	CStringNum;
};