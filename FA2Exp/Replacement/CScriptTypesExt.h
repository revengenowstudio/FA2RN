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

	void OnCBCurrentActionEditChanged();
	void OnCBScriptParameterEditChanged();
	void OnLBScriptActionsSelectChanged();

	/*void DoDataExchange(CDataExchange* pDX);

	void OnCBCurrentScriptSelectChanged();
	void OnETScriptNameChanged();

	void OnCBCurrentActionSelectChanged();
	void OnCBScriptParameterSelectChanged();
	*/
	void OnBNAddActionClickedExt();
	/*
	void OnBNDeleteActionClicked();
	void OnBNAddScriptClicked();
	void OnBNDeleteScriptClicked();*/

	//void OnBNDeleteActionClicked();
	void OnBNAddScriptClickedExt();
	//void OnBNDeleteScriptClicked();


	CScriptTypesExt() {};
	~CScriptTypesExt() {};

	// Functional Functions
	void OnBNCloneScriptClicked();
	void OnBNCloneItemClicked();

	static int ExtParamID;
	static std::map<int, CScriptTypeAction> ExtActions;
	static std::map<int, CScriptTypeParam> ExtParams;

private:
	BOOL onMessageKeyDown(MSG* pMsg);
	BOOL onMessageKeyUp(MSG* pMsg);
};