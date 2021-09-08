#pragma once

#include <FA2PP.h>
#include <MFC/FA2CStatic.h>
#include <MFC/FA2CEdit.h>
#include <MFC/FA2CListBox.h>
#include <MFC/FA2CCombobox.h>

class NOVTABLE CScriptTypes : public FA2::CDialog
{
public:
	CScriptTypes() { JMP_THIS(0x4D5A10); }


	virtual BOOL OnInitDialog() override { JMP_THIS(0x4D8D20); }
	virtual void DoDataExchange(CDataExchange* pDX) { JMP_THIS(0x4D5BE0); }

	//
	// Controls
	//

	void OnScriptTypeSelectChanged() { JMP_THIS(0x4D61B0); }
	void OnActionLineSelectChanged() { JMP_THIS(0x4D6500); }
	void OnNameEditChanged() { JMP_THIS(0x4D6770); }
	void OnCBCurrentActionEditChanged() { JMP_THIS(0x4D6A10); }
	void OnActionTypeSelectChanged() { JMP_THIS(0x4D75D0); }
	void OnActionParameterEditChanged() { JMP_THIS(0x4D7670); }
	void OnActionParameterSelectChanged() { JMP_THIS(0x4D7A50); }
	void OnActionAdd() { JMP_THIS(0x4D7AC0); }
	void OnActionDelete() { JMP_THIS(0x4D7DD0); }
	void OnScriptTypeAdd() { JMP_THIS(0x4D8390); }
	void OnScriptTypeDelete() { JMP_THIS(0x4D8730); }


	//member properties
	FA2::CEdit		EditDescription; //<! description of a specific selected action type
	FA2::CStatic	TextParameterLabel; //<! label that hints the category of action parameter
	FA2::CComboBox	ComboBoxActionType; //<! action type for a selected line of selected script type 
	FA2::CComboBox	ComboBoxScriptType; //<! all script types, and you can select one to edit
	FA2::CComboBox	ComboBoxActionParameter; //<! action parameter for a selected action type
	FA2::CListBox	ListActions; //<! all action listed for a selected script type
	FA2::CString	ScriptName; //<! a human readable name to identify a script
};