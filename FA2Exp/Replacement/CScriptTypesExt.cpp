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
}

BOOL CScriptTypesExt::PreTranslateMessageHook(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		{
			switch (::GetDlgCtrlID(pMsg->hwnd)) {
			case DLG_ScriptTypes_Edit_Name: this->OnNameEditChanged();
			default:
				break;
			}

		}

		//do not exit dialog when enter key pressed
		return TRUE;
		default:
			break;
		}
	}

	return this->FA2CDialog::PreTranslateMessage(pMsg);
}



int CScriptTypesExt::ExtParamID = -1;
void CScriptTypesExt::UpdateParams(int actionIndex)
{
	auto& action = ExtActions[actionIndex];
	auto& param = ExtParams[action.ParamCode_];
	if (param.Param_ == ExtParamID)
		return;
	switch (param.Param_)
	{
	default:
	case 0:
		while (this->CCBScriptParameter.DeleteString(0) != -1);
		break;
	case 1:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Target(this->CCBScriptParameter);
		break;
	case 2:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Waypoint(this->CCBScriptParameter);
		break;
	case 3:
		CScriptTypesFunctions::CScriptTypes_LoadParams_ScriptLine(
			this->CCBScriptParameter,
			this->CCBCurrentScript,
			this->CLBScriptActions
		);
		break;
	case 4:
		CScriptTypesFunctions::CScriptTypes_LoadParams_SplitGroup(this->CCBScriptParameter);
		break;
	case 5:
		CScriptTypesFunctions::CScriptTypes_LoadParams_GlobalVariables(this->CCBScriptParameter);
		break;
	case 6:
		CScriptTypesFunctions::CScriptTypes_LoadParams_ScriptTypes(this->CCBScriptParameter);
		break;
	case 7:
		CScriptTypesFunctions::CScriptTypes_LoadParams_TeamTypes(this->CCBScriptParameter);
		break;
	case 8:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Houses(this->CCBScriptParameter);
		break;
	case 9:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Speechs(this->CCBScriptParameter);
		break;
	case 10:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Sounds(this->CCBScriptParameter);
		break;
	case 11:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Movies(this->CCBScriptParameter);
		break;
	case 12:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Themes(this->CCBScriptParameter);
		break;
	case 13:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Countries(this->CCBScriptParameter);
		break;
	case 14:
		CScriptTypesFunctions::CScriptTypes_LoadParams_LocalVariables(this->CCBScriptParameter);
		break;
	case 15:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Facing(this->CCBScriptParameter);
		break;
	case 16:
		CScriptTypesFunctions::CScriptTypes_LoadParams_BuildingTypes(this->CCBScriptParameter);
		break;
	case 17:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Animations(this->CCBScriptParameter);
		break;
	case 18:
		CScriptTypesFunctions::CScriptTypes_LoadParams_TalkBubble(this->CCBScriptParameter);
		break;
	case 19:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Status(this->CCBScriptParameter);
		break;
	case 20:
		CScriptTypesFunctions::CScriptTypes_LoadParams_Boolean(this->CCBScriptParameter);
		break;
	}
	this->CSTParameterOfSection.SetWindowText(param.Label_);
	this->CSTParameterOfSection.EnableWindow(action.Editable_);
	this->CCBScriptParameter.EnableWindow(action.Editable_);
	this->CETDescription.SetWindowText(action.Description_);
	ExtParamID = param.Param_;
}

//
// Ext Functions
//

std::map<int, CScriptTypeAction> CScriptTypesExt::ExtActions;
std::map<int, CScriptTypeParam> CScriptTypesExt::ExtParams;
BOOL CScriptTypesExt::OnInitDialogExt()
{
	BOOL ret = FA2CDialog::OnInitDialog();
	if (!ret)
		return FALSE;

	auto TranslateDlgItem = [this](int nID, const char* lpKey)
	{
		CString ret = Translations::Translate(lpKey);
		if (ret.GetLength()) {
			this->SetDlgItemText(nID, ret);
		}
	};

	auto TranslateCItem = [](CWnd* pWnd, const char* lpKey)
	{
		CString ret = Translations::Translate(lpKey);
		if (ret.GetLength()) {
			pWnd->SetWindowText(ret);
		}
	};

	TranslateCItem(this, "ScriptTypesTitle");

	TranslateDlgItem(50700, "ScriptTypesDesc");
	TranslateDlgItem(50701, "ScriptTypesSelectedScript");
	TranslateDlgItem(50702, "ScriptTypesName");
	TranslateDlgItem(50703, "ScriptTypesActions");
	TranslateDlgItem(50704, "ScriptTypesActionType");
	TranslateDlgItem(1198, "ScriptTypesActionParam");//sbFA2
	TranslateDlgItem(50705, "ScriptTypesActionDesc");

	TranslateDlgItem(1154, "ScriptTypesAddScript");
	TranslateDlgItem(1066, "ScriptTypesDelScript");
	TranslateDlgItem(6300, "ScriptTypesCloScript");
	TranslateDlgItem(1173, "ScriptTypesAddAction");
	TranslateDlgItem(1174, "ScriptTypesDelAction");
	TranslateDlgItem(6301, "ScriptTypesCloAction");
	TranslateDlgItem(6302, "ScriptTypesInsertMode");

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
			if (id < 0) continue;
			auto count =
				utilities::parse_list(pair.second, (const char**)(pParseBuffer), 2);
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
	CString scriptId, buffer, tmp;
	int scriptIndex, listIndex, actionIndex, selectIndex, L, R, M;

	scriptIndex = this->CCBCurrentScript.GetCurSel();
	listIndex = this->CLBScriptActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0)
	{
		this->CCBCurrentScript.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR)
		{
			buffer += ",0";
			actionIndex = buffer.GetLength() - 2;
		}
		tmp = buffer.Mid(actionIndex + 1);
		utilities::trim_index(tmp);
		this->CCBScriptParameter.SetWindowText(tmp);

		actionIndex = atoi(buffer.Mid(0, actionIndex));

		// As we know, the data sequence is ¡ü
		// So divide it!
		L = 0;
		R = this->CCBCurrentAction.GetCount() - 1;
		M = (L + R) >> 1;
		while (R > L)
		{
			const int MData = this->CCBCurrentAction.GetItemData(M);
			if (MData == actionIndex)
				break;
			if (MData > actionIndex)
				R = M;
			else
				L = M;
			M = (L + R) >> 1;
		}
		if (R > L)
			selectIndex = M;
		else
			selectIndex = 0;

		this->CCBCurrentAction.SetCurSel(selectIndex);
		this->UpdateParams(actionIndex);
	}
}
//
//void CScriptTypesExt::OnETScriptNameChanged()
//{
//}
//
void CScriptTypesExt::OnCBCurrentActionEditChanged()
{
	auto& doc = *INIClass::GetMapDocument(true);
	CString scriptId, buffer, listStr, tmp;
	int scriptIndex, listIndex, actionIndex, actionData;

	scriptIndex = this->CCBCurrentScript.GetCurSel();
	listIndex = this->CLBScriptActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0)
	{
		this->CCBCurrentScript.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR)
			buffer = "0";
		else
			buffer = buffer.Mid(actionIndex + 1);

		actionIndex = this->CCBCurrentAction.GetCurSel();
		if (actionIndex >= 0)
		{
			actionData = this->CCBCurrentAction.GetItemData(actionIndex);
			this->UpdateParams(actionData);
			actionIndex = this->CCBScriptParameter.FindString(0, buffer);
			if (actionIndex != CB_ERR)
				this->CCBScriptParameter.SetCurSel(actionIndex);
			tmp.Format("%d,%s", actionData, buffer);
			listStr.Format("%d", listIndex);
			doc.WriteString(scriptId, listStr, tmp);
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
	CString scriptId, buffer, listStr, paramStr, tmp;
	int scriptIndex, listIndex, actionIndex;

	scriptIndex = this->CCBCurrentScript.GetCurSel();
	listIndex = this->CLBScriptActions.GetCurSel();
	if (scriptIndex >= 0 && listIndex >= 0)
	{
		this->CCBCurrentScript.GetLBText(scriptIndex, scriptId);
		utilities::trim_index(scriptId);
		buffer.Format("%d", listIndex);
		buffer = doc.GetString(scriptId, buffer, "0,0");
		actionIndex = buffer.Find(',');
		if (actionIndex == CB_ERR)
			actionIndex = buffer.GetLength();
		buffer = buffer.Mid(0, actionIndex);
		this->CCBScriptParameter.GetWindowText(paramStr);
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

	bool bInsertMode = ::SendMessage(::GetDlgItem(*this, 6302), BM_GETCHECK, 0, 0) == BST_CHECKED;
	if (!bInsertMode)
	{
		this->OnBNAddActionClicked();
		return;
	}*/

	// insert mode ON
	/*CString scriptID;
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

		CString srcKey, destKey;

		for (int i = actionCount - 1; i >= curIndex; --i)
		{
			srcKey.Format("%d", i);
			destKey.Format("%d", i + 1);
			CString temp = doc.GetString(scriptID, srcKey, "0,0");
			Logger::Debug("%s %s %s %s\n", scriptID, srcKey, destKey, temp);
			doc.WriteString(scriptID, destKey, temp);
		}

		srcKey.Format("%d", curIndex);
		Logger::Debug("%s %s %s\n", scriptID, srcKey, "0,0");
		doc.WriteString(scriptID, srcKey, "0,0");
		this->OnCBCurrentScriptSelectChanged();
	}*/

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
		CString label;
		this->CCBCurrentScript.GetLBText(nCurSel, label);
		utilities::trim_index(label);
		INISection copied(*doc.TryGetSection(label));
		FA2::CString name;
		name = copied.EntriesDictionary["Name"];
		name += " Clone";
		((FA2::CString*)(&copied.EntriesDictionary["Name"]))->AssignCopy(strlen(name), name);
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
	::MessageBox(NULL, "ÀÁµÃÐ´ÁË", "¹¾¹¾¹¾", MB_OK);
	::MessageBox(NULL, "Implement Needed!", "TODO", MB_OK);
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

			pThis->CETDescription.SetWindowText(itr->second.Description_);
			pThis->CETDescription.EnableWindow(itr->second.Editable_);
			pThis->CCBScriptParameter.EnableWindow(itr->second.Editable_);
		}
	}
	return 0x4D7662;
}

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
