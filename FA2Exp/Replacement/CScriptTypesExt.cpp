#include "CScriptTypesExt.h"
#include "CScriptTypesExtHelper.h"
#include "Helpers.h"
#include "../Utilities/INIParser.h"
#include "../Enhancement/MultiLanguage.h"
#include "../Config.h"
#include <set>

#if defined(Standalone)
#define SAFE_RELEASE(ptr) {if(!ptr) delete[] ptr;}

std::vector<ScriptTemplate> g_ScriptTemplates;
FA2::CString CScriptTypesExt::_placeholderCstr;

std::map<int, CScriptTypeAction> CScriptTypesExt::ExtActions;
std::map<int, CScriptTypeParam> CScriptTypesExt::ExtParams;

int scriptTypeIndexToComboBoxIndex(FA2::CComboBox& comboBox, int scriptTypeIndex)
{
	auto const totalCount = comboBox.GetCount();
	for (auto idx = 0; idx < totalCount; ++idx) {
		const int MData = comboBox.GetItemData(idx);
		if (MData == scriptTypeIndex) {
			return idx;
		}
	}

	return 0;
}

void CScriptTypesExt::ProgramStartupInit()
{
	HackHelper::ResetMessageType(0x595FC8, EN_KILLFOCUS); // name update
	HackHelper::ResetMessageType(0x596010, CBN_KILLFOCUS); // param update

	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596148), &CScriptTypesExt::PreTranslateMessageHook);
	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596174), &CScriptTypesExt::OnInitDialogHook);
	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596130), &CScriptTypesExt::OnCommandHook);

	HackHelper::ResetMessageMemberFunction(AFX_MSG_FUNC(0x596028), &CScriptTypesExt::OnActionParameterSelectChangedExt);
	
}

BOOL CScriptTypesExt::onMessageKeyDown(MSG* pMsg)
{
	int ret = -1;
	switch (pMsg->wParam)
	{
		case VK_RETURN:
		{
			switch (::GetDlgCtrlID(pMsg->hwnd)) {
				case DLG_ScriptTypes_Edit_Name: this->OnNameEditChanged();
					ret = TRUE;
					break;
				default:
					break;
			}

		}
		default:
			break;
	}
	return -1;
}
BOOL CScriptTypesExt::onMessageKeyUp(MSG* pMsg)
{
	if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonClone)->GetSafeHwnd()) {
		this->OnScriptTypeCloneExt();
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonCloneLine)->GetSafeHwnd()) {
		this->OnActionLineCloneExt();
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonNewLine)->GetSafeHwnd()) {
		this->OnActionTypeAddExt();
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonReload)->GetSafeHwnd()) {
		this->OnTemplateLoadExt();
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonNew)->GetSafeHwnd()) {
		this->OnScriptTypeAddExt();
	}/* else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ComboBoxExtParameter)->GetSafeHwnd()) {
		this->OnActionParameterSelectChangedExt();
	}*/
	return -1;
}

BOOL CScriptTypesExt::PreTranslateMessageHook(MSG* pMsg)
{
	int ret = -1;
	if (pMsg->message == WM_KEYDOWN) {
		ret = onMessageKeyDown(pMsg);
	} else if (pMsg->message == WM_LBUTTONUP) {
		ret = onMessageKeyUp(pMsg);
	}

	return ret < 0 ? this->FA2::CDialog::PreTranslateMessage(pMsg) : ret;
}

BOOL CScriptTypesExt::OnCommandHook(WPARAM wParam, LPARAM lParam)
{
	int ret = -1;
	auto const msgType = HIWORD(wParam);
	auto const nID = LOWORD(wParam);
	LogDebug(" nID %d, msgType %X", nID, msgType);

	if (nID == WND_Script::ComboBoxExtParameter) {
		if (msgType == CBN_SELCHANGE || msgType == CBN_EDITCHANGE) {
			this->OnActionParameterEditChangedExt(); 
		}
	}

	return ret < 0 ? this->FA2::CDialog::OnCommand(wParam, lParam) : ret;
}

void CScriptTypesExt::updateExtraParamComboBox(ExtraParameterType type, int value)
{
	//LogDebug(__FUNCTION__" type = %d", type);
	HWND text = ::GetDlgItem(this->m_hWnd, WND_Script::TextExtParameter);
	ControlMeta::ComboBoxWrapper extParamCmbBox(::GetDlgItem(this->m_hWnd, WND_Script::ComboBoxExtParameter));
	switch (type) {
		default:
		case ExtraParameterType::None:
			::EnableWindow(text, FALSE);
			extParamCmbBox.Clear();
			extParamCmbBox.EnableWindow(false);
			extParamCmbBox.SetWindowTextA("");
			break;
		case ExtraParameterType::ScanType: {
			::EnableWindow(text, TRUE);			
			extParamCmbBox.Clear();
			extParamCmbBox.EnableWindow(true);
			extParamCmbBox.AddString("0 - Least threat");
			extParamCmbBox.AddString("1 - Most threat");
			extParamCmbBox.AddString("2 - Least distant");
			extParamCmbBox.AddString("3 - Most distant");
			extParamCmbBox.SetCurSel(value);
			char buffer[0x20];
			_itoa_s(value, buffer, 10);
			extParamCmbBox.SetWindowTextA(buffer);
			//LogDebug(__FUNCTION__" [%X] window enabled", extParamCmbBox.GetHWND());
		}
			break;
		case ExtraParameterType::Counter: {
			::EnableWindow(text, TRUE);
			extParamCmbBox.Clear();
			extParamCmbBox.EnableWindow(true);
			char buffer[0x20];
			_itoa_s(value, buffer, 10);
			extParamCmbBox.SetWindowTextA(buffer);
			break;
		}
	}
}

const CScriptTypeAction& getActionData(int actionCbIndex)
{
	return CScriptTypesExt::ExtActions[actionCbIndex];
}

const CScriptTypeParam& getParamData(int paramIndex)
{
	return CScriptTypesExt::ExtParams[paramIndex];
}
ParameterType getParameterType(int actionCbIndex)
{
	return getParamData(getActionData(actionCbIndex).ParamTypeIndex_).Type_;
}

ExtraParameterType getExtraParamType(ParameterType paramType)
{
	switch (paramType) {
		default:
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 17:
		case 18:
		case 19:
		case 20:
			return ExtraParameterType::None;
		case PRM_BuildingType:
			return ExtraParameterType::ScanType;
	}
}

void CScriptTypesExt::updateExtraValue(ParameterType paramType, FA2::CString& paramNumStr)
{
	int extraValue = 0;
	if (paramType == PRM_BuildingType) {
		DWORD rawNum = atoi(paramNumStr);
		paramNumStr.Format("%d", LOWORD(rawNum));
		extraValue = HIWORD(rawNum);
	}
	logger::debug(__FUNCTION__" paramType = %d" + std::to_string(paramType));
	updateExtraParamComboBox(getExtraParamType(paramType), extraValue);
}

void CScriptTypesExt::UpdateParams(int actionIndex, FA2::CString& paramNumStr)
{
	static int LastActionID = -1;
	auto const& actionDefinition = getActionData(actionIndex);
	auto const& paramDefinition = getParamData(actionDefinition.ParamTypeIndex_);
	auto const paramType = paramDefinition.Type_;
	auto const lastActionID = std::exchange(LastActionID, actionIndex);
	
	//logger::g_logger.Debug(__FUNCTION__
	//	" LastActionID = " + std::to_string(lastActionID) +
	//	" actionIndex = " + std::to_string(actionIndex) +
	//	" paramType = " + std::to_string(paramType)
	//);
	updateExtraValue(paramType, paramNumStr);
	if (lastActionID == actionIndex) {
		return;
	}
	switch (paramType)
	{
		default:
		case PRM_None:
			while (this->ComboBoxActionParameter.DeleteString(0) != -1);
			break;
		case 1:
			CScriptTypesFunctions::LoadParams_Target(this->ComboBoxActionParameter);
			break;
		case 2:
			CScriptTypesFunctions::LoadParams_Waypoint(this->ComboBoxActionParameter);
			break;
		case 3:
			CScriptTypesFunctions::LoadParams_ScriptLine(
				this->ComboBoxActionParameter,
				this->ComboBoxScriptType,
				this->ListActions
			);
			break;
		case 4:
			CScriptTypesFunctions::LoadParams_SplitGroup(this->ComboBoxActionParameter);
			break;
		case 5:
			CScriptTypesFunctions::LoadParams_GlobalVariables(this->ComboBoxActionParameter);
			break;
		case 6:
			CScriptTypesFunctions::LoadParams_ScriptTypes(this->ComboBoxActionParameter);
			break;
		case 7:
			CScriptTypesFunctions::LoadParams_TeamTypes(this->ComboBoxActionParameter);
			break;
		case 8:
			CScriptTypesFunctions::LoadParams_Houses(this->ComboBoxActionParameter);
			break;
		case 9:
			CScriptTypesFunctions::LoadParams_Speechs(this->ComboBoxActionParameter);
			break;
		case 10:
			CScriptTypesFunctions::LoadParams_Sounds(this->ComboBoxActionParameter);
			break;
		case 11:
			CScriptTypesFunctions::LoadParams_Movies(this->ComboBoxActionParameter);
			break;
		case 12:
			CScriptTypesFunctions::LoadParams_Themes(this->ComboBoxActionParameter);
			break;
		case 13:
			CScriptTypesFunctions::LoadParams_Countries(this->ComboBoxActionParameter);
			break;
		case 14:
			CScriptTypesFunctions::LoadParams_LocalVariables(this->ComboBoxActionParameter);
			break;
		case 15:
			CScriptTypesFunctions::LoadParams_Facing(this->ComboBoxActionParameter);
			break;
		case PRM_BuildingType:
			CScriptTypesFunctions::LoadParams_BuildingTypes(this->ComboBoxActionParameter);
			break;
		case 17:
			CScriptTypesFunctions::LoadParams_Animations(this->ComboBoxActionParameter);
			break;
		case 18:
			CScriptTypesFunctions::LoadParams_TalkBubble(this->ComboBoxActionParameter);
			break;
		case 19:
			CScriptTypesFunctions::LoadParams_Mission(this->ComboBoxActionParameter);
			break;
		case 20:
			CScriptTypesFunctions::LoadParams_Boolean(this->ComboBoxActionParameter);			
			break;
	}
	this->TextParameterLabel.SetWindowText(paramDefinition.Label_);
	this->TextParameterLabel.EnableWindow(actionDefinition.Editable_);
	this->ComboBoxActionParameter.EnableWindow(actionDefinition.Editable_);
	this->EditDescription.SetWindowText(actionDefinition.Description_);
}

//
// Ext Functions
//
BOOL CScriptTypesExt::OnInitDialogHook()
{
	if (!FA2::CDialog::OnInitDialog()) {
		return FALSE;
	}

	auto TranslateDlgItem = [this](int nID, const char* lpKey)
	{
		FA2::CString ret = Translations::Translate(lpKey);
		if (ret.GetLength()) {
			this->SetDlgItemTextA(nID, ret);
		}
	};

	auto TranslateCItem = [](FA2::CWnd* pWnd, const char* lpKey)
	{
		FA2::CString ret = Translations::Translate(lpKey);
		if (ret.GetLength()) {
			pWnd->SetWindowTextA(ret);
		}
	};

	TranslateCItem(this, "ScriptTypesTitle");

	TranslateDlgItem(WND_Script::TextDescription, "ScriptTypesDesc");
	TranslateDlgItem(WND_Script::TextScriptType, "ScriptTypesSelectedScript");
	TranslateDlgItem(WND_Script::TextName, "ScriptTypesName");
	TranslateDlgItem(WND_Script::TextActions, "ScriptTypesActions");
	TranslateDlgItem(WND_Script::TextType, "ScriptTypesActionType");
	TranslateDlgItem(WND_Script::TextActionParam, "ScriptTypesActionParam");//sbFA2
	TranslateDlgItem(WND_Script::TextActionDescription, "ScriptTypesActionDesc");

	TranslateDlgItem(WND_Script::ButtonNew, "ScriptTypesAddScript");
	TranslateDlgItem(WND_Script::ButtonDelete, "ScriptTypesDelScript");
	TranslateDlgItem(WND_Script::ButtonClone, "ScriptTypesCloScript");
	TranslateDlgItem(WND_Script::ButtonNewLine, "ScriptTypesAddAction");
	TranslateDlgItem(WND_Script::ButtonDeleteLine, "ScriptTypesDelAction");
	TranslateDlgItem(WND_Script::ButtonCloneLine, "ScriptTypesCloAction");
	TranslateDlgItem(WND_Script::CheckBoxToggleInsert, "ScriptTypesInsertMode");

	while (ComboBoxActionType.DeleteString(0) != -1);

	// Initialize defaults
	const char** pNames = reinterpret_cast<const char**>(0x5D035C);
	const char** pDescriptions = reinterpret_cast<const char**>(0x5D0448);

	/*auto& errAct = ExtActions[-1];
	errAct.Description_ = "FA2sp Error Action Holder";
	errAct.Editable_ = false;
	errAct.Hide_ = true;
	errAct.Name_ = "INVALID SCRIPT ACTION";
	errAct.ParamCode_ = 0;*/

	for (int i = 0; i < 59; ++i)
	{
		auto& curAction = ExtActions[i];
		curAction.Name_ = _strdup(pNames[i]);
		curAction.Description_ = _strdup(pDescriptions[i]);
		curAction.Editable_ = true;
		curAction.Hide_ = false;
		curAction.ParamTypeIndex_ = 0;
	}

	auto& fadata = GlobalVars::INIFiles::FAData();

	if (auto const entities = fadata.TryGetSection("ScriptParams")) {
		char* pParseBuffer[2];
		for (auto& pair : entities->EntriesDictionary)
		{
			int id = atoi(pair.first);
			if (id < 0) { 
				continue; 
			}
			auto count = utilities::parse_list(pair.second, (const char**)(pParseBuffer), 2);
			switch (count)
			{
				default:
				case 2:
					ExtParams[id].Type_ = ParameterType(atoi((const char*)pParseBuffer[1]));
				case 1:
					ExtParams[id].Label_ = _strdup((const char*)pParseBuffer[0]);
				case 0:
					continue;
			}
		}
		SAFE_RELEASE(pParseBuffer[0]);
		SAFE_RELEASE(pParseBuffer[1]);
	}

	if (auto const entities = fadata.TryGetSection("ScriptsRA2")) {
		for (auto& pair : entities->EntriesDictionary) {
			int id = atoi(pair.first);
			//LogDebug(" id = %d", id);
			if (id < 0) { 
				continue; 
			}
			auto const& strings = utilities::split_string(pair.second);
			//LogDebug("pair.second = %s, count = %d", pair.second, strings.size());
			switch (strings.size())
			{
				case 5:
					ExtActions[id].Description_ = _strdup(strings[4].c_str());
					//LogDebug(" Description_ = %s", ExtActions[id].Description_);
				case 4:
					ExtActions[id].Editable_ = utilities::parse_bool(strings[3].c_str());
				case 3:
					ExtActions[id].Hide_ = utilities::parse_bool(strings[2].c_str());
				case 2:
					ExtActions[id].ParamTypeIndex_ = atoi(strings[1].c_str());
				case 1:
					ExtActions[id].Name_ = _strdup(strings[0].c_str());
				case 0:
				default:
					continue;
			}
		}
	}

	int counter = 0;
	for (auto& ent : ExtActions)
	{
		if (!ent.second.Hide_)
		{
			int data = ComboBoxActionType.AddString(ent.second.Name_);
			ComboBoxActionType.SetItemData(data, counter);
		}
		++counter;

	}
	this->OnTemplateLoadExt();
	return TRUE;
}

//
//void CScriptTypesExt::OnScriptTypeSelectChanged()
//{
//}
//
void CScriptTypesExt::OnActionLineSelectChangedExt()
{
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	FA2::CString scriptId, buffer, paramNumStr;
	int listIndex, actionIndex, selectIndex;

	auto const scriptIndex = this->ComboBoxScriptType.GetCurSel();
	listIndex = this->ListActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		this->ComboBoxScriptType.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		auto const idxs = std::to_string(listIndex);
		buffer.Format("%d", listIndex);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex < 0) {
			buffer += ",0";
			actionIndex = buffer.GetLength() - 2;
		}
		paramNumStr = buffer.Mid(actionIndex + 1);
		utilities::trim_index(paramNumStr);

		actionIndex = atoi(buffer.Mid(0, actionIndex));

		selectIndex = scriptTypeIndexToComboBoxIndex(this->ComboBoxActionType, actionIndex);

		this->ComboBoxActionType.SetCurSel(selectIndex);
		this->UpdateParams(actionIndex, paramNumStr);
		this->ComboBoxActionParameter.SetWindowTextA(paramNumStr);
	}
}
//
//void CScriptTypesExt::OnETScriptNameChanged()
//{
//}
//

void _showCStr(const FA2::CString& str)
{
	char buffer[0x20];
	sprintf_s(buffer, "Len : %d, Str : %s", str.GetLength(), str.operator LPCTSTR());
	logger::g_logger.Debug(buffer);
	//MessageBoxA(NULL, buffer, "", MB_OK);
}

void CScriptTypesExt::OnActionTypeEditChangedExt()
{
	auto& doc = *INIClass::GetMapDocument(true);

	const int scriptIndex = this->ComboBoxScriptType.GetCurSel();
	const int listIndex = this->ListActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0)
	{
		FA2::CString scriptId;
		FA2::CString buffer;
		_showCStr(scriptId);
		_showCStr(buffer);
		this->ComboBoxScriptType.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		_showCStr(scriptId);
		buffer.Format("%d", listIndex);
		_showCStr(buffer);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		_showCStr(buffer);
		int commaPos = buffer.Find(',');
		if (commaPos == CB_ERR) {
			buffer = "0";
		}
		else {
			buffer = buffer.Mid(commaPos + 1);
		}

		int actionIndex = this->ComboBoxActionType.GetCurSel();
		if (actionIndex >= 0)
		{
			const int actionData = this->ComboBoxActionType.GetItemData(actionIndex);
			this->UpdateParams(actionData);
			actionIndex = this->ComboBoxActionParameter.FindString(0, buffer);
			if (actionIndex != CB_ERR) {
				this->ComboBoxActionParameter.SetCurSel(actionIndex);
			}
			FA2::CString listStr;
			FA2::CString tmp;
			tmp.Format("%d,%s", actionData, buffer);
			listStr.Format("%d", listIndex);
			doc.WriteString(scriptId, listStr, tmp);
			_showCStr(tmp);
		}
	}
}

void CScriptTypesExt::OnActionTypeSelectChangedExt()
{
	int curActionIdx = this->ComboBoxActionType.GetCurSel();
	if (curActionIdx >= 0)
	{
		int curActionData = this->ComboBoxActionType.GetItemData(curActionIdx);
		auto& dict = CScriptTypesExt::ExtActions;
		auto itr = dict.find(curActionData);
		if (itr != dict.end())
		{
			this->OnActionTypeEditChangedExt();

			this->EditDescription.SetWindowTextA(itr->second.Description_);
			this->EditDescription.EnableWindow(itr->second.Editable_);
			this->ComboBoxActionParameter.EnableWindow(itr->second.Editable_);
		}
	}
}

int CScriptTypesExt::getExtraValue()
{
	auto const curActionSel = this->ComboBoxActionType.GetCurSel();
	const int actionData = this->ComboBoxActionType.GetItemData(curActionSel);
	auto const paramType = getParameterType(actionData);
	auto const extParamType = getExtraParamType(paramType);
	ControlMeta::ComboBoxWrapper extParamCmbBox(::GetDlgItem(this->m_hWnd, WND_Script::ComboBoxExtParameter));
	auto& curExtParamContent = extParamCmbBox.GetText();
	LogDebug(" curExtParamContent = %s", curExtParamContent);
	return atoi(curExtParamContent);
}

void CScriptTypesExt::OnActionParameterEditChangedExt()
{
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	FA2::CString scriptId, buffer, listStr, paramStr, tmp;
	int scriptIndex, listIndex, actionIndex;

	scriptIndex = this->ComboBoxScriptType.GetCurSel();
	listIndex = this->ListActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		this->ComboBoxScriptType.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR) {
			actionIndex = buffer.GetLength();
		}
		buffer = buffer.Mid(0, actionIndex);
		this->ComboBoxActionParameter.GetWindowTextA(paramStr);
		//paramStr = this->ComboBoxActionParameter.GetText();
		utilities::trim_index(paramStr);
		//
		//LogDebug(__FUNCTION__" actionData = %d, paramType = %d, extParamType = %d", actionData, paramType, extParamType);
		if (auto const extValue = getExtraValue()) {
			auto const paramInt = MAKEWPARAM(atoi(paramStr), extValue);
			paramStr.Format("%d", paramInt);
		}
		//
		tmp.Format("%s,%s", buffer, paramStr);
		listStr.Format("%d", listIndex);
		doc.WriteString(scriptId, listStr, tmp);
		_showCStr(tmp);
	}
}

void CScriptTypesExt::OnActionParameterSelectChangedExt()
{
#if 0//This functions is not really necessary, since we will finally get to 'OnActionParameterEditChangedExt' when lose focus
	FA2::CString paramUnified;
	auto const curActionSel = this->ComboBoxActionType.GetCurSel();
	const int actionData = this->ComboBoxActionType.GetItemData(curActionSel);
	auto const paramType = getParameterType(actionData);
	auto const extParamType = getExtraParamType(paramType);
	LogDebug(__FUNCTION__" actionData = %d, paramType = %d, extParamType = %d", actionData, paramType, extParamType);
	if (extParamType == ExtraParameterType::None) {
		this->OnActionParameterSelectChanged();
		return;
	}
	auto& curParamContent = this->ComboBoxActionParameter.GetText();
	utilities::trim_index(curParamContent);
	//unify data by extra type
	if (extParamType == ExtraParameterType::ScanType) {
		auto const paramInt = MAKEWPARAM(atoi(curParamContent), getExtraValue());
		paramUnified.Format("%d", paramInt);
	}

	//this->UpdateParams(actionData);

	FA2::CString actionINIIndexValue;
	FA2::CString actionINIValue;
	auto const listIndex = this->ListActions.GetCurSel();
	actionINIValue.Format("%d,%s", actionData, paramUnified);
	actionINIIndexValue.Format("%d", listIndex);
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	auto const scriptId = getCurrentTypeID();
	doc.WriteString(scriptId, actionINIIndexValue, actionINIValue);
#endif
}


void CScriptTypesExt::insertAction(int curSel, const FA2::CString& scriptTypeId, const FA2::CString& value)
{
	FA2::CString idxStr;
	std::vector<FA2::CString> oldItems;
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	//record old values from next line
	for (auto idx = curSel + 1; idx < this->ListActions.GetCount(); ++idx) {
		idxStr.Format("%d", idx);
		oldItems.push_back(doc.GetString(scriptTypeId, idxStr, "0,0"));
	}
	idxStr.Format("%d", curSel + 1);
	doc.WriteString(scriptTypeId, idxStr, value);
	auto newIdx = curSel + 2;
	for (auto& val : oldItems) {
		idxStr.Format("%d", newIdx);
		doc.WriteString(scriptTypeId, idxStr, val);
		newIdx++;
	}
	this->ListActions.InsertString(this->ListActions.GetCount(), idxStr);
	this->ListActions.SetCurSel(curSel + 1);
	this->OnActionLineSelectChangedExt();
}

FA2::CString CScriptTypesExt::getCurrentTypeID()
{
	FA2::CString scriptTypeId;
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	this->ComboBoxScriptType.GetLBText(this->ComboBoxScriptType.GetCurSel(), scriptTypeId);
	utilities::trim_index(scriptTypeId);
	return scriptTypeId;
}

void CScriptTypesExt::OnActionTypeAddExt()
{
	logger::g_logger.Debug("Add Script Member");
	HWND ScriptWnd = this->GetSafeHwnd();
	HWND CheckBox = ::GetDlgItem(ScriptWnd, WND_Script::CheckBoxToggleInsert);
	bool isInsert = ::SendMessageA(CheckBox, BM_GETCHECK, NULL, NULL) == BST_CHECKED;
	auto const curSel = this->ListActions.GetCurSel();
	if (curSel < 0) {
		isInsert = false;
	}
	//get selected value :
	auto const insertIdx = (isInsert ? curSel : this->ListActions.GetCount()) - 1;
	insertAction(insertIdx, getCurrentTypeID(), "0,0");
	return;
}
//
//void CScriptTypesExt::OnActionDelete()
//{
//}
//
//void CScriptTypesExt::OnScriptTypeAdd()
//{
//}
void CScriptTypesExt::OnScriptTypeAddExt()
{
	logger::g_logger.Info("Add Script");
	HWND ScriptWnd = this->m_hWnd;
	HWND ComboScriptTemplate = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxTemplate);
	HWND EditName = ::GetDlgItem(ScriptWnd, WND_Script::EditScriptName);
	HWND ListBox = ::GetDlgItem(ScriptWnd, WND_Script::ListBoxActions);
	HWND ComboType = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxActionType);
	HWND ComboPara = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxParameter);
	int curTemplateComboCount = ::SendMessageA(ComboScriptTemplate, CB_GETCOUNT, NULL, NULL);
	if (curTemplateComboCount <= 0) {
		HWND BtnLoad = ::GetDlgItem(ScriptWnd, WND_Script::ButtonReload);
		::SendMessageA(BtnLoad, WM_LBUTTONDOWN, WND_Script::ButtonReload, NULL);
		::SendMessageA(BtnLoad, WM_LBUTTONUP, WND_Script::ButtonReload, NULL);
	}
	int curTemplateIndex = ::SendMessageA(ComboScriptTemplate, CB_GETCURSEL, NULL, NULL);
	ScriptTemplate& curTemplate = g_ScriptTemplates[curTemplateIndex];
	logger::g_logger.Info("Now using Script Template " + curTemplate[0].first);

	HWND AllScriptCombo = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxScriptType);
	int ScriptCount = ::SendMessageA(AllScriptCombo, CB_GETCOUNT, 0, 0);
	std::set<std::string> ScriptDictionary;

	for (int i = 0; i < ScriptCount; ++i) {
		int strLen = ::SendMessageA(AllScriptCombo, CB_GETLBTEXTLEN, i, NULL);
		std::string scriptID;
		scriptID.resize(strLen);
		::SendMessageA(AllScriptCombo, CB_GETLBTEXT, i, (LPARAM)scriptID.data());
		ScriptDictionary.emplace(std::move(scriptID));
	}

	this->OnScriptTypeAdd();

	int newIdx = 0;
	for (; newIdx < ScriptCount; ++newIdx) {
		std::string str;
		int strLen = ::SendMessageA(AllScriptCombo, CB_GETLBTEXTLEN, newIdx, NULL);
		str.resize(strLen);
		::SendMessageA(AllScriptCombo, CB_GETLBTEXT, newIdx, (LPARAM)str.data());
		if (ScriptDictionary.find(str) == ScriptDictionary.end()) {
			break;
		}
	}

	//::SendMessageA(AllScriptCombo, CB_SETCURSEL, i, NULL);
	this->ComboBoxScriptType.SetCurSel(newIdx);//select the end
	this->SetDlgItemTextA(WND_Script::EditScriptName, curTemplate[0].second.c_str());
	this->OnNameEditChanged();

	//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::ComboBoxScriptType, CBN_SELCHANGE), (LPARAM)AllScriptCombo);
	this->OnScriptTypeSelectChanged();

	for (int idx = 0; idx < curTemplate.Count(); ++idx) {
		this->OnActionAdd();
		auto const& templateItem = curTemplate[idx + 1];//first one is reserved for name
		//::SendMessageA(ListBox, LB_SETCURSEL, idx, NULL);
		this->ListActions.SetCurSel(idx);
		//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::ListBoxActions, LBN_SELCHANGE), (LPARAM)ListBox);
		this->OnActionLineSelectChangedExt();
		//::SendMessageA(ComboType, CB_SETCURSEL, atoi(templateItem->first.c_str()), NULL);
		auto const scripIndex = atoi(templateItem.first.c_str());
		logger::g_logger.Debug(__FUNCTION__" ComboBoxActionType cur idx : " + std::to_string(atoi(templateItem.first.c_str())));
		this->ComboBoxActionType.SetCurSel(scriptTypeIndexToComboBoxIndex(this->ComboBoxActionType, scripIndex));
		//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::ComboBoxActionType, CBN_SELCHANGE), (LPARAM)ComboType);
		this->OnActionTypeSelectChangedExt();
		if (templateItem.second == "EMPTY") { 
			continue; 
		}
		//::SetWindowTextA(ComboPara, templateItem->second.c_str());
		this->ComboBoxActionParameter.SetWindowTextA(templateItem.second.c_str());
		this->OnActionParameterEditChangedExt();
		//::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(WND_Script::ComboBoxParameter, CBN_SELCHANGE), (LPARAM)ComboPara);
	}
}

//
//void CScriptTypesExt::OnScriptTypeDelete()
//{
//}

void CScriptTypesExt::OnScriptTypeCloneExt()
{
	auto& doc = GlobalVars::INIFiles::CurrentDocument();

	int nCurSel = this->ComboBoxScriptType.GetCurSel();
	if (nCurSel >= 0)
	{
		FA2::CString label;
		this->ComboBoxScriptType.GetLBText(nCurSel, label);
		utilities::trim_index(label);
		INISection copied(*doc.TryGetSection(label));
		FA2::CString name;
		name = copied.EntriesDictionary["Name"];
		name += " Clone";
		copied.EntriesDictionary["Name"].AssignCopy(strlen(name), name);
		//Logger::Debug("new name = %s\n", name);
		FA2::CString id;
		id = INIClass::GetAvailableIndex();
		//Logger::Debug("available index get, id = %s\n", id);
		doc.Insert(id, copied);
		/*Logger::Debug("section inserted!\n");
		Logger::Debug("section detail:\n");
		for (auto& x : copied.EntitiesDictionary)
			Logger::Debug("%s %s\n", x.first, x.second);*/
		FA2::CString key;
		key = INIClass::GetAvailableKey("ScriptTypes");
		//Logger::Debug("available section get, key = %s\n", key);
		doc.WriteString("ScriptTypes", key, id);
		//Logger::Debug("key inserted!\n");
		/*INISection& scripttypes = doc.GetSectionItems("ScriptTypes");
		for (auto& x : scripttypes.EntitiesDictionary)
			Logger::Debug("%s %s\n", x.first, x.second);*/

			// objective : reload combobox
		auto& scripts = this->ComboBoxScriptType;
		while (this->ComboBoxScriptType.DeleteString(0) != CB_ERR);

		if (auto const scripttypes = doc.TryGetSection("ScriptTypes")) {
			for (auto& x : scripttypes->EntriesDictionary) {
				this->ComboBoxScriptType.AddString(x.second + " (" + doc.GetString(x.second, "Name") + ")");
			}
		}
		int idx = scripts.FindString(0, id);
		scripts.SetCurSel(idx);
		this->SetDlgItemText(WND_Script::EditScriptName, name);
	}
	return;
}

void CScriptTypesExt::OnActionLineCloneExt()
{
	logger::g_logger.Debug(__FUNCTION__);

	HWND ScriptWnd = this->GetSafeHwnd();
	HWND CheckBox = ::GetDlgItem(ScriptWnd, WND_Script::CheckBoxToggleInsert);
	bool isInsert = ::SendMessageA(CheckBox, BM_GETCHECK, NULL, NULL) == BST_CHECKED;
	auto const curSel = this->ListActions.GetCurSel();
	if (curSel < 0) {
		return;
	}

	//get selected value :
	FA2::CString idxStr;
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	FA2::CString scriptTypeId = getCurrentTypeID();
	idxStr.Format("%d", curSel);
	auto value = doc.GetString(scriptTypeId, idxStr, "0,0");
	insertAction(isInsert ? curSel : this->ListActions.GetCount() - 1, scriptTypeId, value);
}

void LoadScriptTemplates() {
	Ini& ini = Ini::ConfigIni;
	g_ScriptTemplates.clear();

	//Read Team Templates
	int ScriptTemplatesCount = atoi(ini.Read("ScriptTemplates", "Counts").c_str());
	if (ini.Exist() == FALSE) {
		MessageBox(
			NULL,
			MessageBoxConfig::Instance.Message.IniNotExist.c_str(),
			MessageBoxConfig::Instance.Captain.Error.c_str(),
			MB_OK
		);
		ScriptTemplatesCount = 0;
	}
	if (ScriptTemplatesCount < 0) { 
		ScriptTemplatesCount = 0; 
	}

	logger::g_logger.Info(std::to_string(ScriptTemplatesCount) + " Script Templates Loading");

	g_ScriptTemplates.resize(ScriptTemplatesCount + 1);
	g_ScriptTemplates[0].Resize(1);
	g_ScriptTemplates[0][0].first = ini.Read("ScriptTemplates", "DefaultName");
	g_ScriptTemplates[0][0].second = "New script";

	for (int i = 1; i <= ScriptTemplatesCount; ++i) {
		std::string curstr = ini.Read("ScriptTemplates", std::to_string(i));
		ScriptTemplate scriptTemplate = ini.Split(curstr, ',');
		g_ScriptTemplates[i] = scriptTemplate;
	}

	return;
}

void CScriptTypesExt::OnTemplateLoadExt()
{
	logger::g_logger.Debug("Load Script Templates");
	LoadScriptTemplates();
	HWND ScriptWnd = this->m_hWnd;
	HWND ComboScriptTemplate;
	this->GetDlgItem(9978, &ComboScriptTemplate);
	::SendMessage(ComboScriptTemplate, CB_RESETCONTENT, NULL, NULL);
	int ScriptTemplateCount = g_ScriptTemplates.size();
	for (int i = 0; i < ScriptTemplateCount; ++i) {
		::SendMessage(ComboScriptTemplate, CB_ADDSTRING, NULL, (LPARAM)(g_ScriptTemplates[i][0].first.c_str()));
	}
	::SendMessage(ComboScriptTemplate, CB_SETCURSEL, 0, NULL);
	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(9978, CBN_SELCHANGE), (LPARAM)ComboScriptTemplate);
}
//****************************************** Hooks ****************************************************

// Cleanning up
DEFINE_HOOK(4D5B20, CScriptTypes_DTOR, 7)
{
	auto& dict_1 = CScriptTypesExt::ExtActions;
	for (auto& x : dict_1)
	{
		delete[] x.second.Description_;
		delete[] x.second.Name_;
	}
	dict_1.clear();

	auto& dict_2 = CScriptTypesExt::ExtParams;
	for (auto& x : dict_2)
		delete[] x.second.Label_;

	return 0;
}

DEFINE_HOOK(4D7670, CScriptTypes_OnCBScriptParameterEditChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	pThis->OnActionParameterEditChangedExt();
	return 0x4D7A44;
}

DEFINE_HOOK(4D6A10, CScriptTypes_OnCBCurrentActionEditChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	pThis->OnActionTypeEditChangedExt();
	return 0x4D7569;
}

DEFINE_HOOK(4D75D0, CScriptTypes_OnCBCurrentActionSelectChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	pThis->OnActionTypeSelectChangedExt();
	return 0x4D7662;
}
//totally rewritten
DEFINE_HOOK(4D6500, CScriptTypes_OnLBScriptActionsSelectChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	pThis->OnActionLineSelectChangedExt();
	return 0x4D676C;
}

//DEFINE_HOOK(4D5BE0, CScriptTypes_DoDataExchange, 8)
//{
//	return 0;
//
//	//GET(CScriptTypesExt*, pThis, ECX);
//	//GET_STACK(CDataExchange*, pDX, 4);
//	//pThis->CScriptTypesExt::DoDataExchange(pDX);
// //   return 0x4D5C65;
//}
//
//DEFINE_HOOK(4D5C70, CScriptTypes_GetMessageMap, 5)
//{
//	R->EAX<void*>(CScriptTypesExt::GetMessageMap());
//	return 0x4D5C75;
//}
//
//DEFINE_HOOK(4D61B0, CScriptTypes_OnCBCurrentScriptSelectChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnScriptTypeSelectChanged();
//	return 0x4D64FD;
//}
//
//DEFINE_HOOK(4D6500, CScriptTypes_OnLBScriptActionsSelectChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnActionLineSelectChangedExt();
//	return 0x4D676C;
//}
//
//DEFINE_HOOK(4D6770, CScriptTypes_OnETScriptNameChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnETScriptNameChanged();
//	return 0x4D6A0D;
//}
//
//DEFINE_HOOK(4D6A10, CScriptTypes_OnCBCurrentActionEditChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnActionTypeEditChangedExt();
//	return 0x4D7569;
//}
//
//DEFINE_HOOK(4D75D0, CScriptTypes_OnCBCurrentActionSelectChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnCBCurrentActionSelectChanged();
//	return 0x4D7662;
//}
//
//
//
//DEFINE_HOOK(4D7AC0, CScriptTypes_OnBNAddActionClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnActionAdd();
//	return 0x4D7DC7;
//}
//
//DEFINE_HOOK(4D7DD0, CScriptTypes_OnBNDeleteActionClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnActionDelete();
//	return 0x4D8388;
//}
//
//DEFINE_HOOK(4D8390, CScriptTypes_OnBNAddScriptClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnScriptTypeAdd();
//	return 0x4D8721;
//}
//DEFINE_HOOK(4D8390, CScriptTypes_OnBNAddScriptClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnScriptTypeAddExt();
//	return 0x4D8721;
//}
//
//DEFINE_HOOK(4D8730, CScriptTypes_OnBNDeleteScriptClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnScriptTypeDelete();
//	return 0x4D8AB6;
//}

#endif