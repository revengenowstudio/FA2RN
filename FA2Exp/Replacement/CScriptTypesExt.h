#pragma once

#include <CScriptTypes.h>
#include <map>

struct CScriptTypeAction {
	const char* Name_;
	int ParamCode_;
	bool Hide_;
	bool Editable_;
	const char* Description_;
};

struct CScriptTypeParam {
	const char* Label_;
	int Param_;
};

class CScriptTypesExt : public CScriptTypes
{
public:
	static void ProgramStartupInit();
	BOOL PreTranslateMessageHook(MSG * pMsg);


	void UpdateParams(int actionIndex);

	//
	// Ext Functions
	//

	/*static void* GetMessageMap();*/

	BOOL OnInitDialogHook();

	void OnActionTypeEditChangedExt();
	void OnActionParameterEditChangedExt();
	void OnActionLineSelectChangedExt();

	/*void DoDataExchange(CDataExchange* pDX);

	void OnScriptTypeSelectChanged();
	void OnETScriptNameChanged();

	void OnCBCurrentActionSelectChanged();
	void OnActionParameterSelectChanged();
	*/
	void OnActionTypeAddExt();
	/*
	void OnActionDelete();
	void OnScriptTypeAdd();
	void OnScriptTypeDelete();*/

	//void OnActionDelete();
	void OnScriptTypeAddExt();
	//void OnScriptTypeDelete();


	CScriptTypesExt() {};
	~CScriptTypesExt() {};

	// Functional Functions
	void OnScriptTypeCloneExt();
	void OnActionLineCloneExt();
	void OnTemplateLoadExt();

	static std::map<int, CScriptTypeAction> ExtActions;
	static std::map<int, CScriptTypeParam> ExtParams;

private:
	BOOL onMessageKeyDown(MSG* pMsg);
	BOOL onMessageKeyUp(MSG* pMsg);
};