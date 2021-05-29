#include "CScriptTypesExt.h"
#include "CScriptTypesExtHelper.h"
#include "Helpers.h"
#include "../Utilities/INIParser.h"
#include "../Enhancement/MultiLanguage.h"

#define SAFE_RELEASE(ptr) {if(!ptr) delete[] ptr;}

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
		this->OnBNCloneScriptClicked();
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonCloneLine)->GetSafeHwnd()) {
		this->OnBNCloneItemClicked();
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::CheckBoxToggleInsert)->GetSafeHwnd()) {
		//bool bInsertMode = ::SendMessage(::GetDlgItem(*this, WND_Script::CheckBoxToggleInsert), BM_GETCHECK, 0, 0) == BST_CHECKED;
		//::SendMessage(::GetDlgItem(*this, WND_Script::CheckBoxToggleInsert), BM_SETCHECK, bInsertMode ? BST_UNCHECKED : BST_CHECKED, 0);
		//return FALSE;
	} else if (pMsg->hwnd == this->GetDlgItem(WND_Script::ButtonNewLine)->GetSafeHwnd()) {
		this->OnBNAddActionClickedExt();
		return TRUE;
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

void CScriptTypesExt::UpdateParams(int actionIndex, const char* value)
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
			while (this->CCBScriptParameter.DeleteString(0) != -1);
			//this->CCBScriptParameter.ResetContent();
			if (value) {
				this->CCBScriptParameter.SetWindowTextA(value);//To avoid switching to param type 0 error
			}
			break;
		case 1:
			CScriptTypesFunctions::LoadParams_Target(this->CCBScriptParameter);
			break;
		case 2:
			CScriptTypesFunctions::LoadParams_Waypoint(this->CCBScriptParameter);
			break;
		case 3:
			CScriptTypesFunctions::LoadParams_ScriptLine(
				this->CCBScriptParameter,
				this->CCBCurrentScript,
				this->CLBScriptActions
			);
			break;
		case 4:
			CScriptTypesFunctions::LoadParams_SplitGroup(this->CCBScriptParameter);
			break;
		case 5:
			CScriptTypesFunctions::LoadParams_GlobalVariables(this->CCBScriptParameter);
			break;
		case 6:
			CScriptTypesFunctions::LoadParams_ScriptTypes(this->CCBScriptParameter);
			break;
		case 7:
			CScriptTypesFunctions::LoadParams_TeamTypes(this->CCBScriptParameter);
			break;
		case 8:
			CScriptTypesFunctions::LoadParams_Houses(this->CCBScriptParameter);
			break;
		case 9:
			CScriptTypesFunctions::LoadParams_Speechs(this->CCBScriptParameter);
			break;
		case 10:
			CScriptTypesFunctions::LoadParams_Sounds(this->CCBScriptParameter);
			break;
		case 11:
			CScriptTypesFunctions::LoadParams_Movies(this->CCBScriptParameter);
			break;
		case 12:
			CScriptTypesFunctions::LoadParams_Themes(this->CCBScriptParameter);
			break;
		case 13:
			CScriptTypesFunctions::LoadParams_Countries(this->CCBScriptParameter);
			break;
		case 14:
			CScriptTypesFunctions::LoadParams_LocalVariables(this->CCBScriptParameter);
			break;
		case 15:
			CScriptTypesFunctions::LoadParams_Facing(this->CCBScriptParameter);
			break;
		case 16:
			CScriptTypesFunctions::LoadParams_BuildingTypes(this->CCBScriptParameter);
			break;
		case 17:
			CScriptTypesFunctions::LoadParams_Animations(this->CCBScriptParameter);
			break;
		case 18:
			CScriptTypesFunctions::LoadParams_TalkBubble(this->CCBScriptParameter);
			break;
		case 19:
			CScriptTypesFunctions::LoadParams_Mission(this->CCBScriptParameter);
			break;
		case 20:
			CScriptTypesFunctions::LoadParams_Boolean(this->CCBScriptParameter);
			break;
	}
	this->CSTParameterOfSection.SetWindowText(param.Label_);
	this->CSTParameterOfSection.EnableWindow(action.Editable_);
	this->CCBScriptParameter.EnableWindow(action.Editable_);
	this->CETDescription.SetWindowText(action.Description_);
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

	while (CCBCurrentAction.DeleteString(0) != -1);

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
			int data = CCBCurrentAction.AddString(ent.second.Name_);
			CCBCurrentAction.SetItemData(data, counter);
		}
		++counter;

	}
	return TRUE;
}

//
//void CScriptTypesExt::OnCBCurrentScriptSelectChanged()
//{
//}
//
void CScriptTypesExt::OnLBScriptActionsSelectChanged()
{
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	FA2::CString scriptId, buffer, paramNumStr;
	int scriptIndex, listIndex, actionIndex, selectIndex, L, R, M;

	scriptIndex = this->CCBCurrentScript.GetCurSel();
	listIndex = this->CLBScriptActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		this->CCBCurrentScript.GetLBText(scriptIndex, scriptId);
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
		this->CCBScriptParameter.SetWindowTextA(paramNumStr);

		actionIndex = atoi(buffer.Mid(0, actionIndex));

		// As we know, the data sequence is ¡ü
		// So divide it!
		L = 0;
		R = this->CCBCurrentAction.GetCount() - 1;
		M = (L + R) / 2;
		while (R > L) {
			const int MData = this->CCBCurrentAction.GetItemData(M);
			if (MData == actionIndex) {
				break;
			}
			if (MData > actionIndex) {
				R = M;
			}
			else {
				L = M;
			}
			M = (L + R) / 2;
		}
		if (R > L) {
			selectIndex = M;
		}
		else {
			selectIndex = 0;
		}

		this->CCBCurrentAction.SetCurSel(selectIndex);
		this->UpdateParams(actionIndex, paramNumStr);
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

void CScriptTypesExt::OnCBCurrentActionEditChanged()
{
	auto& doc = *INIClass::GetMapDocument(true);

	const int scriptIndex = this->CCBCurrentScript.GetCurSel();
	const int listIndex = this->CLBScriptActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0)
	{
		FA2::CString scriptId;
		FA2::CString buffer;
		_showCStr(scriptId);
		_showCStr(buffer);
		this->CCBCurrentScript.GetLBText(scriptIndex, scriptId);
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

		int actionIndex = this->CCBCurrentAction.GetCurSel();
		if (actionIndex >= 0)
		{
			const int actionData = this->CCBCurrentAction.GetItemData(actionIndex);
			this->UpdateParams(actionData);
			actionIndex = this->CCBScriptParameter.FindString(0, buffer);
			if (actionIndex != CB_ERR) {
				this->CCBScriptParameter.SetCurSel(actionIndex);
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
//
//void CScriptTypesExt::OnCBCurrentActionSelectChanged()
//{
//}
//
void CScriptTypesExt::OnCBScriptParameterEditChanged()
{
	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	FA2::CString scriptId, buffer, listStr, paramStr, tmp;
	int scriptIndex, listIndex, actionIndex;

	scriptIndex = this->CCBCurrentScript.GetCurSel();
	listIndex = this->CLBScriptActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0) {
		this->CCBCurrentScript.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR) {
			actionIndex = buffer.GetLength();
		}
		buffer = buffer.Mid(0, actionIndex);
		this->CCBScriptParameter.GetWindowTextA(paramStr);
		utilities::trim_index(paramStr);
		tmp.Format("%s,%s", buffer, paramStr);
		listStr.Format("%d", listIndex);
		doc.WriteString(scriptId, listStr, tmp);
	}
}
//
//void CScriptTypesExt::OnCBScriptParameterSelectChanged()
//{
//}
//
void CScriptTypesExt::OnBNAddActionClickedExt()
{
	/*if (this->CCBCurrentScript.GetCount() <= 0 && this->CCBCurrentScript.GetCurSel() < 0)
		return;

	bool bInsertMode = ::SendMessage(::GetDlgItem(*this, WND_Script::CheckBoxToggleInsert), BM_GETCHECK, 0, 0) == BST_CHECKED;
	if (!bInsertMode)
	{
		this->OnBNAddActionClicked();
		return;
	}*/

	// insert mode ON
	/*FA2::CString scriptID;
	this->CCBCurrentScript.GetWindowText(scriptID);
	utilities::trim_index(scriptID);

	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	if (doc.SectionExists(scriptID))
	{
		int curIndex = this->CLBScriptActions.GetCurSel();
		int actionCount = this->CLBScriptActions.GetCount();
		this->OnBNAddActionClicked();

		if (actionCount <= 0)
			return;

		if (curIndex == CB_ERR)
			curIndex = 0;

		FA2::CString srcKey, destKey;

		for (int i = actionCount - 1; i >= curIndex; --i)
		{
			srcKey.Format("%d", i);
			destKey.Format("%d", i + 1);
			FA2::CString temp = doc.GetString(scriptID, srcKey, "0,0");
			Logger::Debug("%s %s %s %s\n", scriptID, srcKey, destKey, temp);
			doc.WriteString(scriptID, destKey, temp);
		}

		srcKey.Format("%d", curIndex);
		Logger::Debug("%s %s %s\n", scriptID, srcKey, "0,0");
		doc.WriteString(scriptID, srcKey, "0,0");
		this->OnCBCurrentScriptSelectChanged();
	}*/

	logger::g_logger.Info("Add Script Member");
	HWND ScriptWnd = this->m_hWnd;

	HWND CheckBox = ::GetDlgItem(ScriptWnd, 9993);
	HWND BtnAdd = ::GetDlgItem(ScriptWnd, 1173);
	HWND ListBox = ::GetDlgItem(ScriptWnd, 1170);
	HWND ComboType = ::GetDlgItem(ScriptWnd, 1064);
	HWND ComboPara = ::GetDlgItem(ScriptWnd, 1196);
	int IsChecked = ::SendMessage(CheckBox, BM_GETCHECK, NULL, NULL);
	int ScriptCount = ::SendMessage(ListBox, LB_GETCOUNT, 0, 0);
	int CurSelIndex = ::SendMessage(ListBox, LB_GETCURSEL, 0, 0);
	if (IsChecked != BST_CHECKED || ScriptCount == 0) {
		logger::g_logger.Info("Script Member - Insert Mode OFF");
		::SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
		::SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
		::SendMessage(ListBox, LB_SETCURSEL, ScriptCount, NULL);
		::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		return;
	}
	logger::g_logger.Info("Script Member - Insert Mode ON");
	std::vector<int> CurType(ScriptCount - CurSelIndex + 1);
	std::vector<TCHAR*> CurPara(ScriptCount - CurSelIndex + 1);
	for (int i = CurSelIndex; i < ScriptCount; ++i) {
		::SendMessage(ListBox, LB_SETCURSEL, i, NULL);
		::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		CurType[i - CurSelIndex] = ::SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
		int strLen = ::GetWindowTextLength(ComboPara) + 1;
		CurPara[i - CurSelIndex] = new TCHAR[strLen];
		::GetWindowText(ComboPara, CurPara[i - CurSelIndex], strLen);
	}
	::SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
	::SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
	++ScriptCount;
	for (int i = CurSelIndex + 1; i < ScriptCount; ++i) {
		::SendMessage(ListBox, LB_SETCURSEL, i, NULL);
		::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
		::SendMessage(ComboType, CB_SETCURSEL, CurType[i - CurSelIndex - 1], NULL);
		::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
		::SetWindowText(ComboPara, CurPara[i - CurSelIndex - 1]);
		::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
	}
	::SendMessage(ListBox, LB_SETCURSEL, CurSelIndex, NULL);
	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);

	::SendMessage(ComboType, CB_SETCURSEL, 0, NULL);
	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
	::SetWindowText(ComboPara, "0");
	::SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
	for (auto x : CurPara) { 
		delete[] x; 
	}

	return;
}
//
//void CScriptTypesExt::OnBNDeleteActionClicked()
//{
//}
//
//void CScriptTypesExt::OnBNAddScriptClicked()
//{
//}
void CScriptTypesExt::OnBNAddScriptClickedExt()
{
	// TODO : Jump to the script we have just inserted!
}

























//
//void CScriptTypesExt::OnBNDeleteScriptClicked()
//{
//}

void CScriptTypesExt::OnBNCloneScriptClicked()
{
	auto& doc = GlobalVars::INIFiles::CurrentDocument();

	int nCurSel = this->CCBCurrentScript.GetCurSel();
	if (nCurSel >= 0)
	{
		FA2::CString label;
		this->CCBCurrentScript.GetLBText(nCurSel, label);
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
		/*INISection& scripttypes = doc.GetSection("ScriptTypes");
		for (auto& x : scripttypes.EntitiesDictionary)
			Logger::Debug("%s %s\n", x.first, x.second);*/

			// objective : reload combobox
		auto& scripts = this->CCBCurrentScript;
		while (this->CCBCurrentScript.DeleteString(0) != CB_ERR);

		if (auto const scripttypes = doc.TryGetSection("ScriptTypes")) {
			for (auto& x : scripttypes->EntriesDictionary) {
				this->CCBCurrentScript.AddString(x.second + " (" + doc.GetString(x.second, "Name") + ")");
			}
		}
		int idx = scripts.FindString(0, id);
		scripts.SetCurSel(idx);
		this->SetDlgItemText(1010, name);
	}
	return;
}

void CScriptTypesExt::OnBNCloneItemClicked()
{
	//::MessageBox(NULL, "ÀÁµÃÐ´ÁË", "¹¾¹¾¹¾", MB_OK);
	//::MessageBox(NULL, "Implement Needed!", "TODO", MB_OK);
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
	pThis->CScriptTypesExt::OnCBScriptParameterEditChanged();
	return 0x4D7A44;
}

DEFINE_HOOK(4D6A10, CScriptTypes_OnCBCurrentActionEditChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	pThis->CScriptTypesExt::OnCBCurrentActionEditChanged();
	return 0x4D7569;
}

DEFINE_HOOK(4D75D0, CScriptTypes_OnCBCurrentActionSelectChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	int curActionIdx = pThis->CCBCurrentAction.GetCurSel();
	if (curActionIdx >= 0)
	{
		int curActionData = pThis->CCBCurrentAction.GetItemData(curActionIdx);
		auto& dict = CScriptTypesExt::ExtActions;
		auto itr = dict.find(curActionData);
		if (itr != dict.end())
		{
			pThis->CScriptTypesExt::OnCBCurrentActionEditChanged();

			pThis->CETDescription.SetWindowTextA(itr->second.Description_);
			pThis->CETDescription.EnableWindow(itr->second.Editable_);
			pThis->CCBScriptParameter.EnableWindow(itr->second.Editable_);
		}
	}
	return 0x4D7662;
}
//totally rewritten
DEFINE_HOOK(4D6500, CScriptTypes_OnLBScriptActionsSelectChanged, 7)
{
	GET(CScriptTypesExt*, pThis, ECX);
	pThis->CScriptTypesExt::OnLBScriptActionsSelectChanged();
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
//	pThis->CScriptTypesExt::OnCBCurrentScriptSelectChanged();
//	return 0x4D64FD;
//}
//
//DEFINE_HOOK(4D6500, CScriptTypes_OnLBScriptActionsSelectChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnLBScriptActionsSelectChanged();
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
//	pThis->CScriptTypesExt::OnCBCurrentActionEditChanged();
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
//	pThis->CScriptTypesExt::OnCBScriptParameterEditChanged();
//	return 0x4D7A44;
//}
//
//DEFINE_HOOK(4D7A50, CScriptTypes_OnCBScriptParameterSelectChanged, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnCBScriptParameterSelectChanged();
//	return 0x4D7AB8;
//}
//
//DEFINE_HOOK(4D7AC0, CScriptTypes_OnBNAddActionClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnBNAddActionClicked();
//	return 0x4D7DC7;
//}
//
//DEFINE_HOOK(4D7DD0, CScriptTypes_OnBNDeleteActionClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnBNDeleteActionClicked();
//	return 0x4D8388;
//}
//
//DEFINE_HOOK(4D8390, CScriptTypes_OnBNAddScriptClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnBNAddScriptClicked();
//	return 0x4D8721;
//}
//DEFINE_HOOK(4D8390, CScriptTypes_OnBNAddScriptClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnBNAddScriptClickedExt();
//	return 0x4D8721;
//}
//
//DEFINE_HOOK(4D8730, CScriptTypes_OnBNDeleteScriptClicked, 7)
//{
//	GET(CScriptTypesExt*, pThis, ECX);
//	pThis->CScriptTypesExt::OnBNDeleteScriptClicked();
//	return 0x4D8AB6;
//}
