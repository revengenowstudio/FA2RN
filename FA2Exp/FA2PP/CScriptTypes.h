#pragma once

#include <FA2PP.h>
#include <MFC/FA2CStatic.h>
#include <MFC/FA2CEdit.h>
#include <MFC/FA2CListBox.h>
#include <MFC/FA2CCombobox.h>

class NOVTABLE CScriptTypes : public FA2CDialog
{
public:
	CScriptTypes() { JMP_THIS(0x4D5A10); }


	virtual BOOL OnInitDialog() override { JMP_THIS(0x4D8D20); }
	virtual void DoDataExchange(CDataExchange* pDX) { JMP_THIS(0x4D5BE0); }

	//
	// Controls
	//

	void OnCBCurrentScriptSelectChanged() { JMP_THIS(0x4D61B0); }
	void OnLBScriptActionsSelectChanged() { JMP_THIS(0x4D6500); }
	void OnNameEditChanged() { JMP_THIS(0x4D6770); }
	void OnCBCurrentActionEditChanged() { JMP_THIS(0x4D6A10); }
	void OnCBCurrentActionSelectChanged() { JMP_THIS(0x4D75D0); }
	void OnCBScriptParameterEditChanged() { JMP_THIS(0x4D7670); }
	void OnCBScriptParameterSelectChanged() { JMP_THIS(0x4D7A50); }
	void OnBNAddActionClicked() { JMP_THIS(0x4D7AC0); }
	void OnBNDeleteActionClicked() { JMP_THIS(0x4D7DD0); }
	void OnBNAddScriptClicked() { JMP_THIS(0x4D8390); }
	void OnBNDeleteScriptClicked() { JMP_THIS(0x4D8730); }


	//member properties
	FA2::CEdit		CETDescription; // 92
	FA2::CStatic	CSTParameterOfSection; // 152
	FA2::CComboBox	CCBCurrentAction; // 212
	FA2::CComboBox	CCBCurrentScript; // 272
	FA2::CComboBox	CCBScriptParameter; // 332
	FA2::CListBox	CLBScriptActions; // 392
	FA2::CString	CString_ScriptName; // 452
};