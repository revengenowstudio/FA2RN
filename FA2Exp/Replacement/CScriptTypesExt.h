#pragma once

#include <CScriptTypes.h>
#include <map>

enum /*class*/ ParameterType
{
	PRM_None = 0,
	PRM_BuildingType = 16,
};

enum class ExtraParameterType
{
	None = 0,
	ScanType,
	Counter,
};

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

	void updateExtraParamComboBox(ExtraParameterType type, int value = 0);
	void updateExtraValue(int paramType, FA2::CString& paramNumStr);
	void UpdateParams(int actionIndex, FA2::CString& paramNumStr = _placeholderCstr);

	//
	// Ext Functions
	//

	/*static void* GetMessageMap();*/

	BOOL OnInitDialogHook();

	void OnActionTypeEditChangedExt();
	void OnActionParameterEditChangedExt();
	void OnActionLineSelectChangedExt();
	void OnActionTypeSelectChangedExt();

	/*void DoDataExchange(CDataExchange* pDX);

	void OnScriptTypeSelectChanged();
	void OnETScriptNameChanged();

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
	void insertAction(int curSel, const FA2::CString& scriptTypeId, const FA2::CString& value);
	FA2::CString getCurrentTypeID();

	static FA2::CString _placeholderCstr;
};