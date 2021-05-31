#include "CScriptTypesExt.h"
#include "CScriptTypesExtHelper.h"
#include "Helpers.h"
#include "../Utilities/INIParser.h"
#include "../Enhancement/MultiLanguage.h"
#include "../Config.h"
#include <set>

#define SAFE_RELEASE(ptr) {if(!ptr) delete[] ptr;}

std::vector<ScriptTemplate> g_ScriptTemplates;

int scriptTypeIndexToComboBoxIndex(FA2::CComboBox& comboBox, int scriptTypeIndex)
{
	// As we know, the data sequence is ¡ü
	// So divide it!
	int selectIndex = 0;
	auto L = 0;
	auto R = comboBox.GetCount() - 1;
	auto M = (L + R) / 2;
	while (R > L) {
		const int MData = comboBox.GetItemData(M);
		if (MData == scriptTypeIndex) {
			break;
		}
		if (MData > scriptTypeIndex) {
			R = M;
		} else {
			L = M;
		}
		M = (L + R) / 2;
	}
	if (R > L) {
		selectIndex = M;
	} else {
		selectIndex = 0;
	}
	return selectIndex;
}

void CScriptTypesExt::ProgramStartupInit()
{
	HackHelper::ResetMessageType(0x595FC8, EN_KILLFOCUS); // name update
	HackHelper::ResetMessageType(0x596010, CBN_KILLFOCUS); // param update

	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596148), &CScriptTypesExt::PreTranslateMessageHook);
	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596174), &CScriptTypesExt::OnInitDialogHook);
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
	}
	return -1;
}

BOOL CScriptTypesExt::PreTranslateMessageHook(MSG * pMsg)
{
	int ret = -1;
	if (pMsg->message == WM_KEYDOWN) {
		ret = onMessageKeyDown(pMsg);
	} else if (pMsg->message == WM_LBUTTONUP) {
		ret = onMessageKeyUp(pMsg);
	}
	return ret < 0 ? this->FA2CDialog::PreTranslateMessage(pMsg) : ret;
}

void CScriptTypesExt::UpdateParams(int actionIndex)
{
	static int LastActionID = -1;
	auto const& action = ExtActions[actionIndex];
	auto const& param = ExtParams[action.ParamCode_];
	auto const paramType = param.Param_;
	auto const lastActionID = std::exchange(LastActionID, actionIndex);
	
	//logger::g_logger.Debug(__FUNCTION__
	//	" LastActionID = " + std::to_string(lastActionID) +
	//	" actionIndex = " + std::to_string(actionIndex) +
	//	" paramType = " + std::to_string(paramType)
	//);

	if (lastActionID == actionIndex) {
		return;
	}

	switch (paramType)
	{
		default:
		case 0:
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
		case 16:
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
	this->TextParameterLabel.SetWindowText(param.Label_);
	this->TextParameterLabel.EnableWindow(action.Editable_);
	this->ComboBoxActionParameter.EnableWindow(action.Editable_);
	this->EditDescription.SetWindowText(action.Description_);
}

//
// Ext Functions
//

std::map<int, CScriptTypeAction> CScriptTypesExt::ExtActions;
std::map<int, CScriptTypeParam> CScriptTypesExt::ExtParams;
BOOL CScriptTypesExt::OnInitDialogHook()
{
	if (!FA2CDialog::OnInitDialog()) {
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
		curAction.ParamCode_ = 0;
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
					ExtParams[id].Param_ = atoi((const char*)pParseBuffer[1]);
				case 1:
					ExtParams[id].Label_ = _strdup((const char*)pParseBuffer[0]);
				case 0:
					continue;
			}
		}
		SAFE_RELEASE(pParseBuffer[0]);
		SAFE_RELEASE(pParseBuffer[1]);
	}

	if (fadata.SectionExists("ScriptsRA2")) {
		auto const entities = fadata.TryGetSection("ScriptsRA2");
		char* pParseBuffer[5];
		for (auto& pair : entities->EntriesDictionary)
		{
			int id = atoi(pair.first);
			if (id < 0) continue;
			auto count =
				utilities::parse_list(pair.second, (const char**)(pParseBuffer), 5);
			switch (count)
			{
				case 5:
				default:
					ExtActions[id].Description_ = _strdup((const char*)pParseBuffer[4]);
				case 4:
					ExtActions[id].Editable_ = utilities::parse_bool((const char*)pParseBuffer[3]);
				case 3:
					ExtActions[id].Hide_ = utilities::parse_bool((const char*)pParseBuffer[2]);
				case 2:
					ExtActions[id].ParamCode_ = atoi((const char*)pParseBuffer[1]);
				case 1:
					ExtActions[id].Name_ = _strdup((const char*)pParseBuffer[0]);
				case 0:
					continue;
			}
		}
		SAFE_RELEASE(pParseBuffer[0]);
		SAFE_RELEASE(pParseBuffer[1]);
		SAFE_RELEASE(pParseBuffer[2]);
		SAFE_RELEASE(pParseBuffer[3]);
		SAFE_RELEASE(pParseBuffer[4]);
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
		this->UpdateParams(actionIndex);
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
		utilities::trim_index(paramStr);
		tmp.Format("%s,%s", buffer, paramStr);
		listStr.Format("%d", listIndex);
		doc.WriteString(scriptId, listStr, tmp);
	}
}
//
//void CScriptTypesExt::OnActionParameterSelectChanged()
//{
//}
//
void CScriptTypesExt::OnActionTypeAddExt()
{
	logger::g_logger.Debug("Add Script Member");
	HWND ScriptWnd = this->m_hWnd;

	HWND CheckBox = ::GetDlgItem(ScriptWnd, WND_Script::CheckBoxToggleInsert);
	HWND BtnAdd = ::GetDlgItem(ScriptWnd, WND_Script::ButtonFA2NewLine);
	HWND ListBox = ::GetDlgItem(ScriptWnd, WND_Script::ListBoxActions);
	HWND ComboType = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxActionType);
	HWND ComboPara = ::GetDlgItem(ScriptWnd, WND_Script::ComboBoxParameter);
	int IsChecked = ::SendMessage(CheckBox, BM_GETCHECK, NULL, NULL);
	int ScriptCount = ::SendMessage(ListBox, LB_GETCOUNT, 0, 0);
	int CurSelIndex = ::SendMessage(ListBox, LB_GETCURSEL, 0, 0);

	if (IsChecked == BST_CHECKED && ScriptCount > 0) {
		logger::g_logger.Info("Script Member - Insert Mode ON");
		MessageBoxA("Insert mode not supported yet, fallback to insert at the end", "Error");
		//std::vector<int> CurType(ScriptCount - CurSelIndex + 1);
		//std::vector<TCHAR*> CurPara(ScriptCount - CurSelIndex + 1);
		//for (int i = CurSelIndex; i < ScriptCount; ++i) {
		//	::SendMessage(ListBox, LB_SETCURSEL, i, NULL);
		//	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		//	CurType[i - CurSelIndex] = ::SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
		//	int strLen = ::GetWindowTextLength(ComboPara) + 1;
		//	CurPara[i - CurSelIndex] = new TCHAR[strLen];
		//	::GetWindowText(ComboPara, CurPara[i - CurSelIndex], strLen);
		//}
		//::SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
		//::SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
		//++ScriptCount;
		//for (int i = CurSelIndex + 1; i < ScriptCount; ++i) {
		//	::SendMessage(ListBox, LB_SETCURSEL, i, NULL);
		//	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		//	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		//	::SendMessage(ComboType, CB_SETCURSEL, CurType[i - CurSelIndex - 1], NULL);
		//	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
		//	::SetWindowText(ComboPara, CurPara[i - CurSelIndex - 1]);
		//	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
		//}
		//::SendMessage(ListBox, LB_SETCURSEL, CurSelIndex, NULL);
		//::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);

		//::SendMessage(ComboType, CB_SETCURSEL, 0, NULL);
		//::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
		//::SetWindowText(ComboPara, "0");
		//::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
		//for (auto x : CurPara) { 
		//	delete[] x; 
		//}
		//return;
	}

	logger::g_logger.Info("Script Member - Insert Mode OFF");
	this->OnActionAdd();
	//::SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
	//::SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
	::SendMessage(ListBox, LB_SETCURSEL, ScriptCount, NULL);
	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);

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

	HWND CheckBox = ::GetDlgItem(ScriptWnd, 9993);
	HWND BtnAdd = ::GetDlgItem(ScriptWnd, 1173);
	HWND ListBox = ::GetDlgItem(ScriptWnd, 1170);
	HWND ComboType = ::GetDlgItem(ScriptWnd, 1064);
	HWND ComboPara = ::GetDlgItem(ScriptWnd, 1196);
	int IsChecked = ::SendMessageA(CheckBox, BM_GETCHECK, NULL, NULL);
	int ScriptCount = ::SendMessageA(ListBox, LB_GETCOUNT, 0, 0);
	int CurSelIndex = ::SendMessageA(ListBox, LB_GETCURSEL, 0, 0);
	if (ScriptCount == 0) {
		::SendMessageA(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
		::SendMessageA(BtnAdd, WM_LBUTTONUP, 1173, NULL);
		::SendMessageA(ListBox, LB_SETCURSEL, 0, NULL);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		return;
	}
	if (IsChecked != BST_CHECKED) {
		logger::g_logger.Debug("Script Member - Insert Mode OFF");
		int t_Type = ::SendMessageA(ComboType, CB_GETCURSEL, NULL, NULL);
		TCHAR t_Para[256];
		::GetWindowTextA(ComboPara, t_Para, 256);
		::SendMessageA(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
		::SendMessageA(BtnAdd, WM_LBUTTONUP, 1173, NULL);
		::SendMessageA(ListBox, LB_SETCURSEL, ScriptCount, NULL);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		::SendMessageA(ComboType, CB_SETCURSEL, t_Type, NULL);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
		::SetWindowTextA(ComboPara, t_Para);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
		return;
	}
	int CopyType = ::SendMessageA(ComboType, CB_GETCURSEL, NULL, NULL);
	TCHAR CopyPara[256];
	::GetWindowTextA(ComboPara, CopyPara, 256);
	logger::g_logger.Debug("Script Member - Insert Mode ON");
	std::vector<int> CurType(ScriptCount - CurSelIndex + 1);
	std::vector<TCHAR*> CurPara(ScriptCount - CurSelIndex + 1);
	for (int i = CurSelIndex; i < ScriptCount; ++i) {
		::SendMessageA(ListBox, LB_SETCURSEL, i, NULL);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		CurType[i - CurSelIndex] = ::SendMessageA(ComboType, CB_GETCURSEL, NULL, NULL);
		int strLen = ::GetWindowTextLengthA(ComboPara) + 1;
		CurPara[i - CurSelIndex] = new TCHAR[strLen];
		::GetWindowTextA(ComboPara, CurPara[i - CurSelIndex], strLen);
	}
	::SendMessageA(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
	::SendMessageA(BtnAdd, WM_LBUTTONUP, 1173, NULL);
	++ScriptCount;
	for (int i = CurSelIndex + 1; i < ScriptCount; ++i) {
		::SendMessageA(ListBox, LB_SETCURSEL, i, NULL);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		::SendMessageA(ComboType, CB_SETCURSEL, CurType[i - CurSelIndex - 1], NULL);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
		::SetWindowTextA(ComboPara, CurPara[i - CurSelIndex - 1]);
		::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
	}
	::SendMessageA(ListBox, LB_SETCURSEL, CurSelIndex, NULL);
	::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);

	::SendMessageA(ComboType, CB_SETCURSEL, CopyType, NULL);
	::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
	::SetWindowTextA(ComboPara, CopyPara);
	::SendMessageA(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
	for (auto x : CurPara) { 
		delete[] x; 
	}

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


//
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
//DEFINE_HOOK(4D7670, CScriptTypes_OnCBScriptParameterEditChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnActionParameterEditChangedExt();
//	return 0x4D7A44;
//}
//
//DEFINE_HOOK(4D7A50, CScriptTypes_OnCBScriptParameterSelectChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnActionParameterSelectChanged();
//	return 0x4D7AB8;
//}
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
