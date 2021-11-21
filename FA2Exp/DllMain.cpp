﻿// FA2Copy DllMain.cpp
// Programed by SEC-SOME

// In fact I was about to program a MFC dll
// but then found it unnecessary XD
#include "stdafx.h"
#include "FA2Expand.h"
#include "Config.h"
#include "Replacement/CTileSetDialogBarExt.h"
#include <GlobalVars.h>

//#define OldScript

#pragma region Global Variables
// Global Variables

HWND g_FA2Wnd; // Final Alert 2 Window Handle
HMODULE g_hModule;// Final Alert 2 HModule, HInstance as well

HHOOK g_CallWndHook, g_GetMsgHook;
// CallWndHook: Catch messages
// GetMsgHook: For initializations

// Some special flags
BOOL g_GetMsgHooked;
BOOL g_TaskforcesRead;
BOOL g_TaskforceComboFlag;
bool g_TerrainTheaterFlag = false;

WNDPROC g_oldProc; // Save old Main Window Proc

HACCEL g_hAccel; // Accelerator

// Some Handle needs to be global for some reason XD
HWND g_TerrainWnd;
HWND g_SysTreeView;

// Store templates
std::vector<TeamTemplate> g_TeamTemplates;

// Global strings for further use
std::string g_TerrainTheater;
std::string g_Path;

// Necessary items for FindWindow
struct FindWindowConfig {
	std::string DialogClass, MapWnd, IniWnd, HouseWnd, TriggerWnd, TagWnd, 
		TriggerGlobalWnd, TriggerEventWnd, TriggerActionWnd,
		TaskforceWnd, ScriptWnd, TeamWnd, AITriggerWnd,
		SaveWnd, LoadWnd, NewWnd1, NewWnd2, NewWnd3;
}g_FindWindowConfig;


#pragma endregion

#pragma region Function Definations
// Function Definations

// Set hooks and Unhook
BOOL StartHook();
BOOL EndHook();

// Hook Proc
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam);


// Replace for Hotkeys
LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Get Window
HWND GetWindowHandle();
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumChildWindowsProc(HWND hwnd, LPARAM lParam);

// Dialog Proc
BOOL CALLBACK HouseDlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// Functionality Functions
std::string GetPath();

void LoadTeamTemplates();
void GetTreeViewHwnd();
void LoadINI();
void LoadFA2CopyConfig();
#pragma endregion

#pragma region Function Realizations
//Function Realizations

// Set hooks and Unhook
BOOL StartHook()
{
	g_CallWndHook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, g_hModule, GetCurrentThreadId());
	g_GetMsgHook = SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, GetModuleHandle(NULL), GetCurrentThreadId());
	return ((g_CallWndHook != NULL) && (g_GetMsgHook != NULL));
}
BOOL EndHook()
{
	return (UnhookWindowsHookEx(g_CallWndHook) && UnhookWindowsHookEx(g_GetMsgHook));
}

// Hook Proc
LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//logger::g_logger.Info("nCode = " + std::to_string(nCode));

	if (nCode >= 0)
	{
		CWPSTRUCT* cwps = (CWPSTRUCT*)lParam;

		//logger::g_logger.Info("CallWndProc : message = " + std::to_string(cwps->message) 
		//	+ ", wParam =  " + std::to_string(cwps->wParam)
		//	+ ", lParam =  " + std::to_string(cwps->lParam)
		//);

		if (cwps->message == WM_COMMAND)
		{
			INT wmId = LOWORD(cwps->wParam);
			INT wmHi = HIWORD(cwps->wParam);
			//logger::g_logger.Info("nCode = " + std::to_string(nCode));
			//logger::g_logger.Info("wmId = " + std::to_string(wmId));
			switch (wmId)
			{
			//Used ID
			//9970 - 9999
			
			//House
			case WND_Houses::Button: {//Allies Manager
				HWND HouseWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.HouseWnd.c_str()
				);
				if (HouseWnd == NULL) {
					logger::g_logger.Error("Cannot locate the House window.");
					MessageBox(
						NULL,
						MessageBoxConfig::Instance.Message.HouseWndNotFound.c_str(),
						MessageBoxConfig::Instance.Captain.Error.c_str(),
						MB_OK
					);
					break;
				}
				logger::g_logger.Info("Successfully located the House window.");

				HWND ComboBox, Edit;
				ComboBox = GetDlgItem(HouseWnd, 1091);
				Edit = GetDlgItem(HouseWnd, 1099);

				int HouseCount = SendMessage(ComboBox, CB_GETCOUNT, NULL, NULL);
				int CurHouseIndex = SendMessage(ComboBox, CB_GETCURSEL, NULL, NULL);
				TCHAR* str;
				int strLen = GetWindowTextLength(ComboBox) + 1;
				str = new TCHAR[strLen];
				GetWindowText(ComboBox, str, strLen);
				std::string CurHouseStr = str;
				Ini::ConfigIni.Trim(CurHouseStr);

				std::unordered_map<std::string, bool> Houses;
				for (int i = 0; i < HouseCount; ++i) {
					SendMessage(ComboBox, CB_SETCURSEL, i, NULL);
					strLen = GetWindowTextLength(ComboBox) + 1;
					delete[] str;
					str = new TCHAR[strLen];
					GetWindowText(ComboBox, str, strLen);
					std::string key = str;
					Houses[key] = false;
				}
				delete[] str;
				SendMessage(ComboBox, CB_SETCURSEL, HouseCount >= 0 ? CurHouseIndex : -1, NULL);
				TCHAR* AllieStr;
				strLen= GetWindowTextLength(Edit) + 1;
				AllieStr = new TCHAR[strLen];
				GetWindowText(Edit, AllieStr, strLen);
				std::string AlliedText = AllieStr;
				delete[] AllieStr;

				std::vector<std::string> AllieStrs = Ini::ConfigIni.Split(AlliedText, ',');
				for (int i = 0, cnt = AllieStrs.size(); i < cnt; ++i) {
					Ini::ConfigIni.Trim(AllieStrs[i]);
					if (Houses.find(AllieStrs[i]) != Houses.end()) //House Exists
						Houses[AllieStrs[i]] = true;
				}
				
				Houses.erase(CurHouseStr);

				std::pair<std::unordered_map<std::string, bool>*, std::pair<std::string*, std::string*>*> Param;
				std::pair<std::string*, std::string*> StringParam = std::make_pair(&AlliedText, &CurHouseStr);
				
				Param.first = &Houses;
				Param.second = &StringParam;

				DialogBoxParam(g_hModule, MAKEINTRESOURCE(IDD_DIALOG1), HouseWnd, HouseDlgProc, (LPARAM)&Param);
				
				SetWindowText(Edit, AlliedText.c_str());
				
				SendMessage(HouseWnd, WM_COMMAND, MAKEWPARAM(1099, EN_SETFOCUS), (LPARAM)Edit);
				SendMessage(HouseWnd, WM_COMMAND, MAKEWPARAM(1099, EN_KILLFOCUS), (LPARAM)Edit);
				SendMessage(HouseWnd, WM_COMMAND, MAKEWPARAM(1091, CBN_DROPDOWN), (LPARAM)ComboBox);
				SendMessage(HouseWnd, WM_COMMAND, MAKEWPARAM(1091, CBN_CLOSEUP), (LPARAM)ComboBox);

				break;
			}
			//Tree View Debug
			/*case 9974: {
				logger::g_logger.Info("Tree View Debug");
				break;
			}*/
			//Terrain Group
			case IDC_TerrainListWindow::ComboBox_Sub: {//Sub ComboBox
				switch (wmHi) {
				case CBN_SELCHANGE: {
					CTileSetDialogBarExt::OnSubTypeComboboxSelectionChanged();
					break;
				}
				default:
					break;
				}
				break;
			}
			case  IDC_TerrainListWindow::ComboBox_Main: {//Main ComboBox
				switch (wmHi) {
				case CBN_SELCHANGE: {
					CTileSetDialogBarExt::OnGroupComboboxSelectionChanged();
					break;
				}
				default:
					break;
				}
				break;
			}
			case IDC_TerrainListWindow::Button_Reload: {//Reload Button
				logger::g_logger.Info(" wmHi:" + std::to_string(wmHi));
				CTileSetDialogBarExt::ReloadTileSets(g_TerrainTheater);
				break;
			}
			//INI
			case WND_INI_Editor::Button_Search: {
				HWND INIWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.IniWnd.c_str()
				);
				HWND SearchEdit = GetDlgItem(INIWnd, 9972);
				HWND CheckBox = GetDlgItem(INIWnd, 9970);
				HWND ComboBox = GetDlgItem(INIWnd, 1036);
				int strLen = GetWindowTextLength(SearchEdit) + 1;
				TCHAR* str = new TCHAR[strLen];
				GetWindowText(SearchEdit, str, strLen);
				int FindStartIndex = (SendMessage(CheckBox, BM_GETCHECK, NULL, NULL) == BST_CHECKED)
					? -1
					: SendMessage(ComboBox, CB_GETCURSEL, NULL, NULL);
				int FindResult = SendMessage(
					ComboBox,
					CB_FINDSTRING, 
					FindStartIndex,
					(LPARAM)str
				);

				if (FindResult == CB_ERR || (FindResult < FindStartIndex && FindStartIndex >= 0))
					MessageBox(
						NULL,
						MessageBoxConfig::Instance.Message.INISectionNotFound.c_str(),
						MessageBoxConfig::Instance.Captain.Hint.c_str(),
						MB_OK
					);
				else {
					SendMessage(ComboBox, CB_SETCURSEL, FindResult, NULL);
					SendMessage(INIWnd, WM_COMMAND, MAKELPARAM(1036, CBN_SELCHANGE), (LPARAM)ComboBox);
				}
				delete[] str;
				break;
			}
			case 9982: {//Override the Add Button
				logger::g_logger.Info("Add INI Section");
				HWND INIWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.IniWnd.c_str()
				);
				HWND BtnAdd = GetDlgItem(INIWnd, 1037);
				HWND ComboBox = GetDlgItem(INIWnd, 1036);

				//Remember Sections
				int SectionCount = SendMessage(ComboBox, CB_GETCOUNT, NULL, NULL);
				std::vector<TCHAR*> KeyDictionary(SectionCount);
				for (int i = 0; i < SectionCount; ++i) {
					int strLen = SendMessage(ComboBox, CB_GETLBTEXTLEN, NULL, NULL) + 1;
					KeyDictionary[i] = new TCHAR[strLen];
					SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)KeyDictionary[i]);
				}

				logger::g_logger.Info(std::to_string(SectionCount) + "INI section loaded");

				//New Section
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1037, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1037, NULL);

				int i;
				for (i = 0; i < SectionCount; ++i) {
					SendMessage(ComboBox, CB_SETCURSEL, i, NULL);
					SendMessage(INIWnd, WM_COMMAND, MAKEWPARAM(1036, CBN_SELCHANGE), (LPARAM)ComboBox);
					TCHAR* str;
					int strLen = SendMessage(ComboBox, CB_GETLBTEXTLEN, i, NULL) + 1;
					str = new TCHAR[strLen];
					SendMessage(ComboBox, CB_GETLBTEXT, i, (LPARAM)str);
					if (strcmp(str, KeyDictionary[i]) != 0) {
						delete[] str;
						break;
					}
					delete[] str;
				}
				SendMessage(ComboBox, CB_SETCURSEL, i, NULL);
				SendMessage(INIWnd, WM_COMMAND, MAKEWPARAM(1036, CBN_SELCHANGE), (LPARAM)ComboBox);

				for (auto x : KeyDictionary)	delete[] x;

				break;
			}
			//Tag
			case 9970: {//Copy Tag
				HWND TagWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TagWnd.c_str()
				);

				HWND TagCombo = GetDlgItem(TagWnd, 1083);
				if (SendMessage(TagCombo, CB_GETCURSEL, NULL, NULL) == CB_ERR)	break;

				HWND BtnAdd = GetDlgItem(TagWnd, 1154);
				int ID[3] = { 1010,1156,1157 };
				HWND Control[3];
				TCHAR* CopyData[3];
				for (int i = 0; i < 3; ++i) {
					Control[i] = GetDlgItem(TagWnd, ID[i]);
					int strLen = GetWindowTextLength(Control[i]) + 1;
					CopyData[i] = new TCHAR[strLen];
					GetWindowText(Control[i], CopyData[i], strLen);
				}
					
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1154, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1154, NULL);
				
				//SetWindowText(Control[0], strcat(CopyData[0], " Clone"));
				std::string NewName = CopyData[0];
				NewName += " Clone";
				SetWindowText(Control[0], NewName.c_str());

				for (int i = 1; i < 3; ++i) {
					SetWindowText(Control[i], CopyData[i]);
					SendMessage(TagWnd, WM_COMMAND, MAKEWPARAM(ID[i], CBN_EDITCHANGE), (LPARAM)Control[i]);
				}
				for (int i = 0; i < 3; ++i)	delete[] CopyData[i];

				break;
			}
			//Trigger
			case 9987: {
				logger::g_logger.Info("Copy Repeat Type");
				HWND TriggerWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerWnd.c_str()
				);
				HWND TriggerTabWnd = GetDlgItem(FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerWnd.c_str()
				), 1393);
				HWND GlobalWnd = FindWindowEx(
					TriggerTabWnd,
					NULL,
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerGlobalWnd.c_str()
				);
				HWND ComboBox = GetDlgItem(GlobalWnd, 1394);
				HWND BtnCopy = GetDlgItem(TriggerWnd, 1163);
				int CurType;
				TCHAR *str;
				int strLen = GetWindowTextLength(ComboBox) + 1;
				str = new TCHAR[strLen];
				GetWindowText(ComboBox, str, strLen);
				switch (str[0]) {
				case '0':
					CurType = 0;
					break;
				case '1':
					CurType = 1;
					break;
				case '2':
					CurType = 2;
					break;
				default:
					CurType = 0;
					break;
				}
				logger::g_logger.Info("Current trigger repeat type: " + str[0]);
				SendMessage(BtnCopy, WM_LBUTTONDOWN, 1403, NULL);
				SendMessage(BtnCopy, WM_LBUTTONUP, 1403, NULL);
				SendMessage(ComboBox, CB_SETCURSEL, CurType, NULL);
				SendMessage(GlobalWnd, WM_COMMAND, MAKEWPARAM(1394, CBN_SELCHANGE), (LPARAM)ComboBox);
				delete[] str;
				break;
			}
			case 9988: {
				logger::g_logger.Info("Add Event Member");
				HWND TriggerTabWnd = GetDlgItem(
					FindWindow(
						g_FindWindowConfig.DialogClass.c_str(),
						g_FindWindowConfig.TriggerWnd.c_str()
					),
					1393);
				HWND EventWnd = FindWindowEx(
					TriggerTabWnd,
					NULL,
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerEventWnd.c_str()
				);
				HWND CurrEventHandle = GetDlgItem(EventWnd, 1167);
				HWND BtnAdd = GetDlgItem(EventWnd, 1396);
				int CurrEventCount = SendMessage(CurrEventHandle, CB_GETCOUNT, NULL, NULL);
				int CurIndex = SendMessage(CurrEventHandle, CB_GETCURSEL, NULL, NULL);
				int DesIndex;
				if (CurrEventCount <= 9)	DesIndex = CurrEventCount + 1;
				else
				{
					++CurIndex;
					DesIndex = CurrEventCount - 8;
				}
				if (CurrEventCount > 19) {
					logger::g_logger.Warn("Current trigger's event is full");
					MessageBox(
						NULL,
						MessageBoxConfig::Instance.Message.TriggerEventFull.c_str(),
						MessageBoxConfig::Instance.Captain.Warning.c_str(),
						MB_OK
					);
				}
				else {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1403, NULL);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1403, NULL);
					SendMessage(CurrEventHandle, CB_SETCURSEL, DesIndex, NULL);
					SendMessage(EventWnd, WM_COMMAND, MAKEWPARAM(1167, CBN_SELCHANGE), (LPARAM)CurrEventHandle);
				}
				break;
			}
			case 9989: {
				logger::g_logger.Info("Add Action Member");
				HWND TriggerTabWnd = GetDlgItem(
					FindWindow(
						g_FindWindowConfig.DialogClass.c_str(),
						g_FindWindowConfig.TriggerWnd.c_str()
					),
					1393);
				HWND ActionWnd = FindWindowEx(
					TriggerTabWnd, 
					NULL, 
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerActionWnd.c_str()
				);
				HWND CurrActionHandle = GetDlgItem(ActionWnd, 1170);
				HWND BtnAdd = GetDlgItem(ActionWnd, 1403);
				int CurrActionCount = SendMessage(CurrActionHandle, CB_GETCOUNT, NULL, NULL);
				int CurIndex = SendMessage(CurrActionHandle, CB_GETCURSEL, NULL, NULL);
				int DesIndex;
				if (CurrActionCount <= 9)	DesIndex = CurrActionCount + 1;
				else
				{
					++CurIndex;
					DesIndex = CurrActionCount - 8;
				}
				if (CurrActionCount > 19) {
					logger::g_logger.Warn("Current trigger's action is full");
					MessageBox(
						NULL,
						MessageBoxConfig::Instance.Message.TriggerActionFull.c_str(),
						MessageBoxConfig::Instance.Captain.Warning.c_str(),
						MB_OK
					);
				}
				else {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1403, NULL);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1403, NULL);
					SendMessage(CurrActionHandle, CB_SETCURSEL, DesIndex, NULL);
					SendMessage(ActionWnd, WM_COMMAND, MAKEWPARAM(1170, CBN_SELCHANGE), (LPARAM)CurrActionHandle);
				}
				break;
			}
			case 9990: {//Copy Event Member
				logger::g_logger.Info("Copy Event Member");
				HWND TriggerTabWnd = GetDlgItem(
					FindWindow(
						g_FindWindowConfig.DialogClass.c_str(),
						g_FindWindowConfig.TriggerWnd.c_str()
					),
					1393);
				HWND EventWnd = FindWindowEx(
					TriggerTabWnd,
					NULL,
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerEventWnd.c_str()
				);
				HWND CurrEventHandle = GetDlgItem(EventWnd, 1167);
				HWND EventType = GetDlgItem(EventWnd, 1175);
				HWND EventList = GetDlgItem(EventWnd, 1401);
				HWND EventPara = GetDlgItem(EventWnd, 1402);
				int CurrEventCount = SendMessage(CurrEventHandle, CB_GETCOUNT, NULL, NULL);
				if (CurrEventCount > 19) {
					logger::g_logger.Warn("Current trigger's event is full");
					MessageBox(
						NULL,
						MessageBoxConfig::Instance.Message.TriggerEventFull.c_str(),
						MessageBoxConfig::Instance.Captain.Warning.c_str(),
						MB_OK
					);
					break;
				}
				int CurIndex = SendMessage(CurrEventHandle, CB_GETCURSEL, NULL, NULL);
				int DesIndex;
				if (CurrEventCount <= 9)	DesIndex = CurrEventCount + 1;
				else
				{
					++CurIndex;
					DesIndex = CurrEventCount - 8;
				}
				TCHAR *CurType;
				int curLen = GetWindowTextLength(EventType) + 1;
				CurType = new TCHAR[curLen];
				GetWindowText(EventType, CurType, curLen);
				int CurParaCount = SendMessage(EventList, LB_GETCOUNT, NULL, NULL);
				std::vector<TCHAR*> CurPara(CurParaCount);
				for (int i = 0; i < CurParaCount; ++i) {
					SendMessage(EventList, LB_SETCURSEL, i, NULL);
					SendMessage(EventWnd, WM_COMMAND, MAKEWPARAM(1401, LBN_SELCHANGE), (LPARAM)EventList);
					int strLen = GetWindowTextLength(EventPara) + 1;
					CurPara[i] = new TCHAR[strLen];
					GetWindowText(EventPara, CurPara[i], strLen);
				}

				HWND BtnAdd = GetDlgItem(EventWnd, 1396);
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1396, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1396, NULL);
				SendMessage(CurrEventHandle, CB_SETCURSEL, DesIndex, NULL);
				SendMessage(EventWnd, WM_COMMAND, MAKEWPARAM(1167, CBN_SELCHANGE), (LPARAM)CurrEventHandle);

				SetWindowText(EventType, CurType);
				delete[] CurType;
				SendMessage(EventWnd, WM_COMMAND, MAKEWPARAM(1175, CBN_EDITCHANGE), (LPARAM)EventType);

				for (int i = 0; i < CurParaCount; ++i) {
					SendMessage(EventList, LB_SETCURSEL, i, NULL);
					SendMessage(EventWnd, WM_COMMAND, MAKEWPARAM(1401, LBN_SELCHANGE), (LPARAM)EventList);
					SetWindowText(EventPara, CurPara[i]);
					delete[] CurPara[i];
					SendMessage(EventWnd, WM_COMMAND, MAKEWPARAM(1402, CBN_EDITCHANGE), (LPARAM)EventPara);
				}

				break;
			}
			case 9991: {//Copy Action Member
				logger::g_logger.Info("Copy Action Member");
				HWND TriggerTabWnd = GetDlgItem(
					FindWindow(
						g_FindWindowConfig.DialogClass.c_str(),
						g_FindWindowConfig.TriggerWnd.c_str()
					),
					1393);
				HWND ActionWnd = FindWindowEx(
					TriggerTabWnd,
					NULL,
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TriggerActionWnd.c_str()
				);
				HWND CurrActionHandle = GetDlgItem(ActionWnd, 1170);
				HWND ActionType = GetDlgItem(ActionWnd, 1178);
				HWND ActionList = GetDlgItem(ActionWnd, 1401);
				HWND ActionPara = GetDlgItem(ActionWnd, 1402);
				int CurrEventCount = SendMessage(CurrActionHandle, CB_GETCOUNT, NULL, NULL);
				if (CurrEventCount > 19) {
					logger::g_logger.Warn("Current trigger's action is full");
					MessageBox(
						NULL,
						MessageBoxConfig::Instance.Message.TriggerActionFull.c_str(),
						MessageBoxConfig::Instance.Captain.Warning.c_str(),
						MB_OK
					);
					break;
				}
				int CurIndex = SendMessage(CurrActionHandle, CB_GETCURSEL, NULL, NULL);
				int DesIndex;
				if (CurrEventCount <= 9)	DesIndex = CurrEventCount + 1;
				else
				{
					++CurIndex;
					DesIndex = CurrEventCount - 8;
				}
				TCHAR *CurType;
				int curLen = GetWindowTextLength(ActionType) + 1;
				CurType = new TCHAR[curLen];
				GetWindowText(ActionType, CurType, curLen);
				int CurParaCount = SendMessage(ActionList, LB_GETCOUNT, NULL, NULL);
				std::vector<TCHAR*> CurPara(CurParaCount);
				for (int i = 0; i < CurParaCount; ++i) {
					SendMessage(ActionList, LB_SETCURSEL, i, NULL);
					SendMessage(ActionWnd, WM_COMMAND, MAKEWPARAM(1401, LBN_SELCHANGE), (LPARAM)ActionList);
					int strLen = GetWindowTextLength(ActionPara) + 1;
					CurPara[i] = new TCHAR[strLen];
					GetWindowText(ActionPara, CurPara[i], strLen);
				}

				HWND BtnAdd = GetDlgItem(ActionWnd, 1403);
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1403, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1403, NULL);
				SendMessage(CurrActionHandle, CB_SETCURSEL, DesIndex, NULL);
				SendMessage(ActionWnd, WM_COMMAND, MAKEWPARAM(1170, CBN_SELCHANGE), (LPARAM)CurrActionHandle);

				SetWindowText(ActionType, CurType);
				delete[] CurType;
				SendMessage(ActionWnd, WM_COMMAND, MAKEWPARAM(1178, CBN_EDITCHANGE), (LPARAM)ActionType);

				for (int i = 0; i < CurParaCount; ++i) {
					SendMessage(ActionList, LB_SETCURSEL, i, NULL);
					SendMessage(ActionWnd, WM_COMMAND, MAKEWPARAM(1401, LBN_SELCHANGE), (LPARAM)ActionList);
					SetWindowText(ActionPara, CurPara[i]);
					delete[] CurPara[i];
					SendMessage(ActionWnd, WM_COMMAND, MAKEWPARAM(1402, CBN_EDITCHANGE), (LPARAM)ActionPara);
				}
				break;
			}
			//Taskforces
			case 9995: {//Copy Taskforce Member
				logger::g_logger.Info("Copy Taskforce Member");
				HWND TaskforceWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TaskforceWnd.c_str()
				);
				HWND BtnAdd = GetDlgItem(TaskforceWnd, 1146);
				HWND EditNum = GetDlgItem(TaskforceWnd, 1148);
				HWND ComboType = GetDlgItem(TaskforceWnd, 1149);
				HWND ListBox = GetDlgItem(TaskforceWnd, 1145);
				TCHAR CurNum[0x10], CurType[0x20];
				GetWindowText(ComboType, CurType, sizeof(CurType) - 1);
				GetWindowText(EditNum, CurNum, sizeof(CurNum) - 1);
				logger::g_logger.Info("Currect (Type,Number) :[" + (std::string)CurType + ',' + CurNum + "]");
				//safe guard
				if (strlen(CurType)  && strlen(CurType)) {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1146, 0);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1146, 0);
					int Count = SendMessage(ListBox, LB_GETCOUNT, NULL, NULL);
					SendMessage(ListBox, LB_SETCURSEL, Count - 1, NULL);
					SendMessage(TaskforceWnd, WM_COMMAND, MAKEWPARAM(1145, LBN_SELCHANGE), (LPARAM)ListBox);
					SetWindowText(ComboType, CurType);
					SendMessage(TaskforceWnd, WM_COMMAND, MAKEWPARAM(1149, CBN_SELCHANGE), (LPARAM)ComboType);
					SetWindowText(EditNum, CurNum);
				}
				break;
			}
			case 9998: {//Copy Taskforce
				logger::g_logger.Info("Copy Taskforce");
				HWND TaskforceWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TaskforceWnd.c_str()
				);
				TCHAR* TaskforceName, * TaskforceGroup;
				HWND EditName = GetDlgItem(TaskforceWnd, 1010);
				HWND EditGroup = GetDlgItem(TaskforceWnd, 1122);
				HWND EditNum = GetDlgItem(TaskforceWnd, 1148);
				HWND ComboType = GetDlgItem(TaskforceWnd, 1149);
				HWND ListBox = GetDlgItem(TaskforceWnd, 1145);

				int nameLen = GetWindowTextLength(EditName) + 1;
				int groupLen = GetWindowTextLength(EditGroup) + 1;
				TaskforceName = new TCHAR[nameLen];
				TaskforceGroup = new TCHAR[groupLen];

				GetWindowText(EditName, TaskforceName, nameLen);
				GetWindowText(EditGroup, TaskforceGroup, groupLen);
				int ListBoxCount = SendMessage(ListBox, LB_GETCOUNT, 0, 0);
				std::vector<TCHAR*> UnitType(ListBoxCount), UnitNum(ListBoxCount);
				for (int i = 0; i < ListBoxCount; ++i) {
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(TaskforceWnd, WM_COMMAND, MAKEWPARAM(1145, LBN_SELCHANGE), (LPARAM)ListBox);
					int typeLen = GetWindowTextLength(ComboType) + 1;
					int numLen = GetWindowTextLength(EditNum) + 1;
					UnitType[i] = new TCHAR[typeLen];
					UnitNum[i] = new TCHAR[numLen];
					GetWindowText(ComboType, UnitType[i], typeLen);
					GetWindowText(EditNum, UnitNum[i], numLen);
				}
				logger::g_logger.Info(std::to_string(ListBoxCount) + " taskforce members saved");

				//New Taskforce
				HWND BtnNew = GetDlgItem(TaskforceWnd, 1151);
				SendMessage(BtnNew, WM_LBUTTONDOWN, 1151, 0);
				SendMessage(BtnNew, WM_LBUTTONUP, 1151, 0);
				//SetWindowText(EditName, strcat(TaskforceName, " Clone"));
				std::string NewTaskforceName = TaskforceName;
				NewTaskforceName += " Clone";
				SetWindowText(EditName, NewTaskforceName.c_str());
				SetWindowText(EditGroup, TaskforceGroup);
				HWND BtnAdd = GetDlgItem(TaskforceWnd, 1146);
				for (int i = 0; i < ListBoxCount; ++i) {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1146, 0);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1146, 0);
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(TaskforceWnd, WM_COMMAND, MAKEWPARAM(1145, LBN_SELCHANGE), (LPARAM)ListBox);
					SetWindowText(ComboType, UnitType[i]);
					SendMessage(TaskforceWnd, WM_COMMAND, MAKEWPARAM(1149, CBN_SELCHANGE), (LPARAM)ComboType);
					SetWindowText(EditNum, UnitNum[i]);
					delete[] UnitType[i];
					delete[] UnitNum[i];
				}
				delete[] TaskforceGroup;
				delete[] TaskforceName;
				break;
			}
			//Scripts
#if defined(OldScript)
			case 9992: {//Add Script Member (Override 1173)
				logger::g_logger.Info("Add Script Member");
				HWND ScriptWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.ScriptWnd.c_str()
				);
				HWND CheckBox = GetDlgItem(ScriptWnd, 9993);
				HWND BtnAdd = GetDlgItem(ScriptWnd, 1173);
				HWND ListBox = GetDlgItem(ScriptWnd, 1170);
				HWND ComboType = GetDlgItem(ScriptWnd, 1064);
				HWND ComboPara = GetDlgItem(ScriptWnd, 1196);
				int IsChecked = SendMessage(CheckBox, BM_GETCHECK, NULL, NULL);
				int ScriptCount = SendMessage(ListBox, LB_GETCOUNT, 0, 0);
				int CurSelIndex = SendMessage(ListBox, LB_GETCURSEL, 0, 0);
				if (IsChecked != BST_CHECKED) {
					logger::g_logger.Info("Script Member - Insert Mode OFF");
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
					SendMessage(ListBox, LB_SETCURSEL, ScriptCount, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					break;
				}
				if (ScriptCount == 0 || IsChecked != BST_CHECKED) {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
					SendMessage(ListBox, LB_SETCURSEL, 0, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					break;
				}
				logger::g_logger.Info("Script Member - Insert Mode ON");
				std::vector<int> CurType(ScriptCount - CurSelIndex + 1);
				std::vector<TCHAR*> CurPara(ScriptCount - CurSelIndex + 1);
				for (int i = CurSelIndex; i < ScriptCount; ++i) {
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					CurType[i - CurSelIndex] = SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
					int strLen = GetWindowTextLength(ComboPara) + 1;
					CurPara[i - CurSelIndex] = new TCHAR[strLen];
					GetWindowText(ComboPara, CurPara[i - CurSelIndex], strLen);
				}
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
				++ScriptCount;
				for (int i = CurSelIndex + 1; i < ScriptCount; ++i) {
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					SendMessage(ComboType, CB_SETCURSEL, CurType[i - CurSelIndex - 1], NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
					SetWindowText(ComboPara, CurPara[i - CurSelIndex - 1]);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
				}
				SendMessage(ListBox, LB_SETCURSEL, CurSelIndex, NULL);
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);

				SendMessage(ComboType, CB_SETCURSEL, 0, NULL);
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
				SetWindowText(ComboPara, "0");
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
				for (auto x : CurPara)	delete[] x;
				break;
			}
			case 9996: {//Copy Script Member
				logger::g_logger.Info("Copy Script Member");
				HWND ScriptWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.ScriptWnd.c_str()
				);
				HWND CheckBox = GetDlgItem(ScriptWnd, 9993);
				HWND BtnAdd = GetDlgItem(ScriptWnd, 1173);
				HWND ListBox = GetDlgItem(ScriptWnd, 1170);
				HWND ComboType = GetDlgItem(ScriptWnd, 1064);
				HWND ComboPara = GetDlgItem(ScriptWnd, 1196);
				int IsChecked = SendMessage(CheckBox, BM_GETCHECK, NULL, NULL);
				int ScriptCount = SendMessage(ListBox, LB_GETCOUNT, 0, 0);
				int CurSelIndex = SendMessage(ListBox, LB_GETCURSEL, 0, 0);
				if (ScriptCount == 0) {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
					SendMessage(ListBox, LB_SETCURSEL, 0, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					break;
				}
				if (IsChecked != BST_CHECKED) {
					logger::g_logger.Info("Script Member - Insert Mode OFF");
					int t_Type = SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
					TCHAR t_Para[256];
					GetWindowText(ComboPara, t_Para, 256);
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
					SendMessage(ListBox, LB_SETCURSEL, ScriptCount, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					SendMessage(ComboType, CB_SETCURSEL, t_Type, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
					SetWindowText(ComboPara, t_Para);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
					break;
				}
				int CopyType = SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
				TCHAR CopyPara[256];
				GetWindowText(ComboPara, CopyPara, 256);
				logger::g_logger.Info("Script Member - Insert Mode ON");
				std::vector<int> CurType(ScriptCount - CurSelIndex + 1);
				std::vector<TCHAR*> CurPara(ScriptCount - CurSelIndex + 1);
				for (int i = CurSelIndex; i < ScriptCount; ++i) {
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					CurType[i - CurSelIndex] = SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
					int strLen = GetWindowTextLength(ComboPara) + 1;
					CurPara[i - CurSelIndex] = new TCHAR[strLen];
					GetWindowText(ComboPara, CurPara[i - CurSelIndex], strLen);
				}
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1173, NULL);
				++ScriptCount;
				for (int i = CurSelIndex + 1; i < ScriptCount; ++i) {
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					SendMessage(ComboType, CB_SETCURSEL, CurType[i - CurSelIndex - 1], NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
					SetWindowText(ComboPara, CurPara[i - CurSelIndex - 1]);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
				}
				SendMessage(ListBox, LB_SETCURSEL, CurSelIndex, NULL);
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);

				SendMessage(ComboType, CB_SETCURSEL, CopyType, NULL);
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
				SetWindowText(ComboPara, CopyPara);
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
				for (auto x : CurPara)	delete[] x;
				break;
			}
			case 9999: {//Copy Script
				logger::g_logger.Info("Copy Script");
				HWND ScriptWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.ScriptWnd.c_str()
				);
				HWND EditName = GetDlgItem(ScriptWnd, 1010);
				HWND ListBox = GetDlgItem(ScriptWnd, 1170);
				HWND ComboType = GetDlgItem(ScriptWnd, 1064);
				HWND ComboPara = GetDlgItem(ScriptWnd, 1196);
				TCHAR *ScriptName;
				int nameLen = GetWindowTextLength(EditName) + 1;
				ScriptName = new TCHAR[nameLen];
				GetWindowText(EditName, ScriptName, nameLen);
				int ListBoxCount = SendMessage(ListBox, LB_GETCOUNT, 0, 0);
				std::vector<TCHAR*> ScriptPara(ListBoxCount);
				std::vector<int> ScriptType(ListBoxCount);
				for (int i = 0; i < ListBoxCount; ++i) {
					SendMessage(ListBox, LB_SETCURSEL, i, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					ScriptType[i] = SendMessage(ComboType, CB_GETCURSEL, NULL, NULL);
					int strLen = GetWindowTextLength(ComboPara) + 1;
					ScriptPara[i] = new TCHAR[strLen];
					GetWindowText(ComboPara, ScriptPara[i], strLen);
				}

				//Remember All
				HWND AllScriptCombo = GetDlgItem(ScriptWnd, 1193);
				int ScriptCount = SendMessage(AllScriptCombo, CB_GETCOUNT, 0, 0);
				std::vector<TCHAR*> ScriptDictionary(ScriptCount);
				for (int i = 0; i < ScriptCount; ++i) {
					int strLen = SendMessage(AllScriptCombo, CB_GETLBTEXTLEN, i, NULL) + 1;
					ScriptDictionary[i] = new TCHAR[strLen];
					SendMessage(AllScriptCombo, CB_GETLBTEXT, i, (LPARAM)ScriptDictionary[i]);
				}

				logger::g_logger.Info(std::to_string(ScriptCount) + " Script Members Stored");

				//New Script
				HWND BtnNew = GetDlgItem(ScriptWnd, 1154);
				SendMessage(BtnNew, WM_LBUTTONDOWN, 1154, 0);
				SendMessage(BtnNew, WM_LBUTTONUP, 1154, 0);

				//Find The New Script
				int i;
				for (i = 0; i < ScriptCount; ++i) {
					TCHAR* str;
					int strLen = SendMessage(AllScriptCombo, CB_GETLBTEXTLEN, i, NULL) + 1;
					str = new TCHAR[strLen];
					SendMessage(AllScriptCombo, CB_GETLBTEXT, i, (LPARAM)str);
					if (strcmp(str, ScriptDictionary[i]) != 0) {
						delete[] str;
						break;
					}
					delete[] str;
				}
				SendMessage(AllScriptCombo, CB_SETCURSEL, i, NULL);
				SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1193, CBN_SELCHANGE), (LPARAM)AllScriptCombo);

				//Do Copy Works
				//SetWindowText(EditName, strcat(ScriptName, " Clone"));
				std::string NewName = ScriptName;
				NewName += " Clone";
				SetWindowText(EditName, NewName.c_str());

				HWND BtnAdd = GetDlgItem(ScriptWnd, 1173);
				for (int j = 0; j < ListBoxCount; ++j) {
					SendMessage(BtnAdd, WM_LBUTTONDOWN, 1173, 0);
					SendMessage(BtnAdd, WM_LBUTTONUP, 1173, 0);
					SendMessage(ListBox, LB_SETCURSEL, j, NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1170, LBN_SELCHANGE), (LPARAM)ListBox);
					SendMessage(ComboType, CB_SETCURSEL, ScriptType[j], NULL);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1064, CBN_SELCHANGE), (LPARAM)ComboType);
					SetWindowText(ComboPara, ScriptPara[j]);
					SendMessage(ScriptWnd, WM_COMMAND, MAKEWPARAM(1196, CBN_SELCHANGE), (LPARAM)ComboPara);
				}

				for (auto x : ScriptPara)	delete[] x;
				for (auto x : ScriptDictionary) delete[] x;
				delete[] ScriptName;

				break;
			}
#endif
			//Teams
			case 9979: {//Override New Team for Templates
				logger::g_logger.Info("New Team");
				HWND TeamWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TeamWnd.c_str()
				);
				HWND ComboTeamTemplate = GetDlgItem(TeamWnd, 9980);
				HWND EditName = GetDlgItem(TeamWnd, 1010),
					EditPriority = GetDlgItem(TeamWnd, 1011),
					EditMax = GetDlgItem(TeamWnd, 1012);
				HWND ComboGroup = GetDlgItem(TeamWnd, 1122),
					ComboExpLevel = GetDlgItem(TeamWnd, 1143),
					ComboTechLevel = GetDlgItem(TeamWnd, 1103),
					ComboMindControl = GetDlgItem(TeamWnd, 1140);
				int CheckID[19] = { 1113,1114,1115,1116,1117,1128,1129,1130,1131,1132,1119,1120,1127,1133,1134,1137,1136,1138,1139 };
				HWND Check[19];
				for (int i = 0; i < 19; ++i)	Check[i] = GetDlgItem(TeamWnd, CheckID[i]);
				int curTemplateComboCount = SendMessage(ComboTeamTemplate, CB_GETCOUNT, NULL, NULL);
				if (curTemplateComboCount <= 0) {
					HWND BtnLoad = GetDlgItem(TeamWnd, 9981);
					SendMessage(BtnLoad, WM_LBUTTONDOWN, 9981, NULL);
					SendMessage(BtnLoad, WM_LBUTTONUP, 9981, NULL);
				}
				int curTemplateIndex = SendMessage(ComboTeamTemplate, CB_GETCURSEL, NULL, NULL);
				TeamTemplate& curTemplate = g_TeamTemplates[curTemplateIndex];
				logger::g_logger.Info("Now using Team Template " + *curTemplate[0]);
				HWND BtnNew = GetDlgItem(TeamWnd, 1110);
				SendMessage(BtnNew, WM_LBUTTONDOWN, 1151, 0);
				SendMessage(BtnNew, WM_LBUTTONUP, 1151, 0);

				SetWindowText(EditName, curTemplate[1]->c_str());
				SetWindowText(EditPriority, curTemplate[3]->c_str());
				SetWindowText(EditMax, curTemplate[4]->c_str());

				SetWindowText(ComboExpLevel, curTemplate[2]->c_str());
				SetWindowText(ComboGroup, curTemplate[5]->c_str());
				SetWindowText(ComboTechLevel, curTemplate[6]->c_str());
				SetWindowText(ComboMindControl, curTemplate[7]->c_str());
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1143, CBN_SETFOCUS), (LPARAM)ComboExpLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1122, CBN_SETFOCUS), (LPARAM)ComboGroup);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1140, CBN_SETFOCUS), (LPARAM)ComboMindControl);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1103, CBN_SETFOCUS), (LPARAM)ComboTechLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1143, CBN_KILLFOCUS), (LPARAM)ComboExpLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1122, CBN_KILLFOCUS), (LPARAM)ComboGroup);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1140, CBN_KILLFOCUS), (LPARAM)ComboMindControl);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1103, CBN_KILLFOCUS), (LPARAM)ComboTechLevel);

				for (int i = 0; i < 19; ++i) {
					SendMessage(Check[i], BM_SETCHECK, (*curTemplate[i + 8] == "1") ? BST_CHECKED : BST_UNCHECKED, 0);
					SendMessage(Check[i], WM_LBUTTONDOWN, CheckID[i], NULL);
					SendMessage(Check[i], WM_LBUTTONUP, CheckID[i], NULL);
					SendMessage(Check[i], WM_LBUTTONDOWN, CheckID[i], NULL);
					SendMessage(Check[i], WM_LBUTTONUP, CheckID[i], NULL);
				}
				break;
			}
			case 9981: {//Load Templates
				logger::g_logger.Info("Load Team Templates");
				LoadTeamTemplates();
				HWND TeamWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TeamWnd.c_str()
				);
				HWND ComboTeamTemplate = GetDlgItem(TeamWnd, 9980);
				SendMessage(ComboTeamTemplate, CB_RESETCONTENT, NULL, NULL);
				int TeamTemplateCount = g_TeamTemplates.size();
				for (int i = 0; i < TeamTemplateCount; ++i)
					SendMessage(ComboTeamTemplate, CB_ADDSTRING, NULL, (LPARAM)(g_TeamTemplates[i][0]->c_str()));
				SendMessage(ComboTeamTemplate, CB_SETCURSEL, 0, NULL);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(9980, CBN_SELCHANGE), (LPARAM)ComboTeamTemplate);
				break;
			}
			case 9997: {//Copy Team
				logger::g_logger.Info("Copy Team");
				HWND TeamWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.TeamWnd.c_str()
				);
				HWND EditName = GetDlgItem(TeamWnd, 1010),
					EditPriority = GetDlgItem(TeamWnd, 1011),
					EditMax = GetDlgItem(TeamWnd, 1012);
				HWND ComboGroup = GetDlgItem(TeamWnd, 1122),
					ComboExpLevel = GetDlgItem(TeamWnd, 1143),
					ComboWaypoint = GetDlgItem(TeamWnd, 1123),
					ComboTransportWaypoint = GetDlgItem(TeamWnd, 1126),
					ComboTechLevel = GetDlgItem(TeamWnd, 1103),
					ComboMindControl = GetDlgItem(TeamWnd, 1140),
					ComboHouse = GetDlgItem(TeamWnd, 1079),
					ComboScript = GetDlgItem(TeamWnd, 1124),
					ComboTaskforce = GetDlgItem(TeamWnd, 1125),
					ComboTag = GetDlgItem(TeamWnd, 1083);
				int CheckID[20] = { 1113,1114,1115,1116,1117,1128,1129,1130,1131,1132,1119,1120,1127,1133,1134,1135,1137,1136,1138,1139 };
				HWND Check[20];
				for (int i = 0; i < 20; ++i)	Check[i] = GetDlgItem(TeamWnd, CheckID[i]);

				//Save Current Team
				TCHAR* CurrentTeamTextData[13];
				int strLen;
				strLen = GetWindowTextLength(EditName) + 1;
				CurrentTeamTextData[0] = new TCHAR[strLen];
				GetWindowText(EditName, CurrentTeamTextData[0], strLen);
				strLen = GetWindowTextLength(ComboExpLevel) + 1;
				CurrentTeamTextData[1] = new TCHAR[strLen];
				GetWindowText(ComboExpLevel, CurrentTeamTextData[1], 256);
				strLen = GetWindowTextLength(ComboHouse) + 1;
				CurrentTeamTextData[2] = new TCHAR[strLen];
				GetWindowText(ComboHouse, CurrentTeamTextData[2], 256);
				strLen = GetWindowTextLength(EditPriority) + 1;
				CurrentTeamTextData[3] = new TCHAR[strLen];
				GetWindowText(EditPriority, CurrentTeamTextData[3], 256);
				strLen = GetWindowTextLength(EditMax) + 1;
				CurrentTeamTextData[4] = new TCHAR[strLen];
				GetWindowText(EditMax, CurrentTeamTextData[4], 256);
				strLen = GetWindowTextLength(ComboGroup) + 1;
				CurrentTeamTextData[5] = new TCHAR[strLen];
				GetWindowText(ComboGroup, CurrentTeamTextData[5], 256);
				strLen = GetWindowTextLength(ComboWaypoint) + 1;
				CurrentTeamTextData[6] = new TCHAR[strLen];
				GetWindowText(ComboWaypoint, CurrentTeamTextData[6], 256);
				strLen = GetWindowTextLength(ComboTransportWaypoint) + 1;
				CurrentTeamTextData[7] = new TCHAR[strLen];
				GetWindowText(ComboTransportWaypoint, CurrentTeamTextData[7], 256);
				strLen = GetWindowTextLength(ComboTechLevel) + 1;
				CurrentTeamTextData[8] = new TCHAR[strLen];
				GetWindowText(ComboTechLevel, CurrentTeamTextData[8], 256);
				strLen = GetWindowTextLength(ComboMindControl) + 1;
				CurrentTeamTextData[9] = new TCHAR[strLen];
				GetWindowText(ComboMindControl, CurrentTeamTextData[9], 256);
				strLen = GetWindowTextLength(ComboScript) + 1;
				CurrentTeamTextData[10] = new TCHAR[strLen];
				GetWindowText(ComboScript, CurrentTeamTextData[10], 256);
				strLen = GetWindowTextLength(ComboTaskforce) + 1;
				CurrentTeamTextData[11] = new TCHAR[strLen];
				GetWindowText(ComboTaskforce, CurrentTeamTextData[11], 256);
				strLen = GetWindowTextLength(ComboTag) + 1;
				CurrentTeamTextData[12] = new TCHAR[strLen];
				GetWindowText(ComboTag, CurrentTeamTextData[12], 256);
				int IsChecked[20];
				for (int i = 0; i < 20; ++i)	IsChecked[i] = SendMessage(Check[i], BM_GETCHECK, 0, 0);

				//New Team
				HWND BtnNew = GetDlgItem(TeamWnd, 1110);
				SendMessage(BtnNew, WM_LBUTTONDOWN, 1151, 0);
				SendMessage(BtnNew, WM_LBUTTONUP, 1151, 0);
				//SetWindowText(EditName, strcat(CurrentTeamTextData[0], " Clone"));
				std::string NewName = CurrentTeamTextData[0];
				NewName += " Clone";
				SetWindowText(EditName, NewName.c_str());
				SetWindowText(ComboExpLevel, CurrentTeamTextData[1]);
				SetWindowText(ComboHouse, CurrentTeamTextData[2]);
				SetWindowText(EditPriority, CurrentTeamTextData[3]);
				SetWindowText(EditMax, CurrentTeamTextData[4]);
				SetWindowText(ComboGroup, CurrentTeamTextData[5]);
				SetWindowText(ComboWaypoint, CurrentTeamTextData[6]);
				SetWindowText(ComboTransportWaypoint, CurrentTeamTextData[7]);
				SetWindowText(ComboTechLevel, CurrentTeamTextData[8]);
				SetWindowText(ComboMindControl, CurrentTeamTextData[9]);
				SetWindowText(ComboScript, CurrentTeamTextData[10]);
				SetWindowText(ComboTaskforce, CurrentTeamTextData[11]);
				SetWindowText(ComboTag, CurrentTeamTextData[12]);

				//Save ComboBox Changes
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1143, CBN_SETFOCUS), (LPARAM)ComboExpLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1079, CBN_SETFOCUS), (LPARAM)ComboHouse);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1122, CBN_SETFOCUS), (LPARAM)ComboGroup);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1123, CBN_SETFOCUS), (LPARAM)ComboWaypoint);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1126, CBN_SETFOCUS), (LPARAM)ComboWaypoint);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1103, CBN_SETFOCUS), (LPARAM)ComboTechLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1140, CBN_SETFOCUS), (LPARAM)ComboMindControl);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1124, CBN_SETFOCUS), (LPARAM)ComboScript);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1125, CBN_SETFOCUS), (LPARAM)ComboTaskforce);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1083, CBN_SETFOCUS), (LPARAM)ComboTag);

				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1143, CBN_KILLFOCUS), (LPARAM)ComboExpLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1079, CBN_KILLFOCUS), (LPARAM)ComboHouse);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1122, CBN_KILLFOCUS), (LPARAM)ComboGroup);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1123, CBN_KILLFOCUS), (LPARAM)ComboWaypoint);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1126, CBN_KILLFOCUS), (LPARAM)ComboWaypoint);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1103, CBN_KILLFOCUS), (LPARAM)ComboTechLevel);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1140, CBN_KILLFOCUS), (LPARAM)ComboMindControl);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1124, CBN_KILLFOCUS), (LPARAM)ComboScript);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1125, CBN_KILLFOCUS), (LPARAM)ComboTaskforce);
				SendMessage(TeamWnd, WM_COMMAND, MAKEWPARAM(1083, CBN_KILLFOCUS), (LPARAM)ComboTag);

				for (int i = 0; i < 20; ++i) {
					SendMessage(Check[i], BM_SETCHECK, IsChecked[i], 0);
					//Save Checkbox Changes
					SendMessage(Check[i], WM_LBUTTONDOWN, CheckID[i], NULL);
					SendMessage(Check[i], WM_LBUTTONUP, CheckID[i], NULL);
					SendMessage(Check[i], WM_LBUTTONDOWN, CheckID[i], NULL);
					SendMessage(Check[i], WM_LBUTTONUP, CheckID[i], NULL);
				}

				for (int i = 0; i < 13; ++i)	delete[] CurrentTeamTextData[i];

				break;
			}
			//AI Triggers
			case 9994: {
				logger::g_logger.Info("Copy AI Trigger");
				//Initialize Handles
				HWND AITriggerWnd = FindWindow(
					g_FindWindowConfig.DialogClass.c_str(),
					g_FindWindowConfig.AITriggerWnd.c_str()
				);
				HWND BtnAdd = GetDlgItem(AITriggerWnd, 1154);
				int CheckID[6] = { 1218,1424,1425,1426,1452,1453 };
				HWND CheckBox[6];
				for (int i = 0; i < 6; ++i)
					CheckBox[i] = GetDlgItem(AITriggerWnd, CheckID[i]);
				int ComboID[7] = { 1163,1205,1204,1206,1456,1149,1449 };
				HWND ComboBox[7];
				for (int i = 0; i < 7; ++i)
					ComboBox[i] = GetDlgItem(AITriggerWnd, ComboID[i]);
				int EditID[5] = { 1010,1213,1215,1216,1450 };
				HWND Edit[5];
				for (int i = 0; i < 5; ++i)
					Edit[i] = GetDlgItem(AITriggerWnd, EditID[i]);

				//Memorize Things
				int CurCheck[6];
				for (int i = 0; i < 6; ++i)
					CurCheck[i] = SendMessage(CheckBox[i], BM_GETCHECK, 0, 0);
				TCHAR* CurEdit[5];
				for (int i = 0; i < 5; ++i) {
					int strLen = GetWindowTextLength(Edit[i]) + 1;
					CurEdit[i] = new TCHAR[strLen];
					GetWindowText(Edit[i], CurEdit[i], strLen);
				}
					
				int CurTriggerType = SendMessage(ComboBox[0], CB_GETCURSEL, 0, 0);
				int CurTriggerEvent = SendMessage(ComboBox[6], CB_GETCURSEL, 0, 0);
				int CurTriggerSide = SendMessage(ComboBox[4], CB_GETCURSEL, 0, 0);
				TCHAR *CurComboBox[5];
				for (int i = 0; i < 5; ++i) {
					int strLen = GetWindowTextLength(ComboBox[i + 1]) + 1;
					CurComboBox[i] = new TCHAR[strLen];
					GetWindowText(ComboBox[i + 1], CurComboBox[i], strLen);
				}
					

				//New AI Trigger
				SendMessage(BtnAdd, WM_LBUTTONDOWN, 1154, NULL);
				SendMessage(BtnAdd, WM_LBUTTONUP, 1154, NULL);

				for (int i = 0; i < 6; ++i) {
					SendMessage(CheckBox[i], BM_SETCHECK, CurCheck[i], 0);
					SendMessage(CheckBox[i], WM_LBUTTONDOWN, CheckID[i], NULL);
					SendMessage(CheckBox[i], WM_LBUTTONUP, CheckID[i], NULL);
					SendMessage(CheckBox[i], WM_LBUTTONDOWN, CheckID[i], NULL);
					SendMessage(CheckBox[i], WM_LBUTTONUP, CheckID[i], NULL);
				}


				//SetWindowText(Edit[0], strcat(CurEdit[0], " Clone"));
				std::string NewName = CurEdit[0];
				NewName += " Clone";
				SetWindowText(Edit[0], NewName.c_str());
				for (int i = 1; i < 5; ++i)
					SetWindowText(Edit[i], CurEdit[i]);

				SendMessage(ComboBox[0], CB_SETCURSEL, CurTriggerType, 0);
				SendMessage(ComboBox[6], CB_SETCURSEL, CurTriggerEvent, 0);
				SendMessage(ComboBox[4], CB_SETCURSEL, CurTriggerSide, 0);
				for (int i = 0; i < 5; ++i)
					SetWindowText(ComboBox[i + 1], CurComboBox[i]);

				for (int i = 0; i < 7; ++i) {
					SendMessage(AITriggerWnd, WM_COMMAND, MAKEWPARAM(ComboID[i], CBN_SELCHANGE), (LPARAM)ComboBox[i]);
				}

				SendMessage(ComboBox[4], CB_SETCURSEL, CurTriggerSide, 0);
				SendMessage(AITriggerWnd, WM_COMMAND, MAKEWPARAM(ComboID[4], CBN_SELCHANGE), (LPARAM)ComboBox[4]);

				for (int i = 0; i < 5; ++i) {
					delete[] CurEdit[i];
					delete[] CurComboBox[i];
				}
				break;
			}
			}
		}

	}
	return CallNextHookEx(g_CallWndHook, nCode, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (!g_GetMsgHooked) {
		g_GetMsgHooked = TRUE;

		// For global initialization
		g_FA2Wnd = GetWindowHandle();
		g_Path = GetPath();
		//GetTreeViewHwnd();
		LoadINI();
		LoadFA2CopyConfig();

		g_hAccel = LoadAccelerators(g_hModule, MAKEINTRESOURCE(IDR_ACCELERATOR1));
		g_oldProc = (WNDPROC)SetWindowLong(g_FA2Wnd, GWL_WNDPROC, (LONG)HookedWndProc);
	} else {
		TranslateAccelerator(g_FA2Wnd, g_hAccel, (MSG*)lParam);
	}

	//MSG curMsg = *(MSG*)lParam;
	//logger::g_logger.Info("GetMsgProc : message = " + std::to_string(curMsg.message) + ", lParam =  " + std::to_string(curMsg.lParam));
	return CallNextHookEx(g_GetMsgHook, nCode, wParam, lParam);
}

// Replace for Hotkeys
LRESULT CALLBACK HookedWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	return g_oldProc(hWnd, uMsg, wParam, lParam);
}

// Get Window
HWND GetWindowHandle()
{
	DWORD dwCurrentProcessId = GetCurrentProcessId();

	if (!EnumWindows(EnumWindowsProc, (LPARAM)&dwCurrentProcessId))
	{
		return (HWND)dwCurrentProcessId;
	}

	return NULL;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwCurProcessId = *((DWORD*)lParam);
	DWORD dwProcessId = 0;

	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL)
	{
		*((HWND*)lParam) = hwnd;
		return FALSE;
	}

	return TRUE;
}

// Dialog Proc
BOOL CALLBACK HouseDlgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case WM_INITDIALOG:{
			std::unordered_map<std::string, bool>* Houses = 
				((std::pair<std::unordered_map<std::string, bool>*, std::pair<std::string*, std::string*>*>*)lParam)->first;
			std::pair<std::string*, std::string*>* StringParam =
				((std::pair<std::unordered_map<std::string, bool>*, std::pair<std::string*, std::string*>*>*)lParam)->second;
			//I think using a pointer to pass information is much funnier than simply using a global variable XD
			HWND LBA, LBB, EDIT, EDIT2;
			LBA = GetDlgItem(hwnd, IDC_LIST2);//Allies ListBox
			LBB = GetDlgItem(hwnd, IDC_LIST3);//Enemy ListBox
			EDIT = GetDlgItem(hwnd, IDC_EDIT1);//Edit
			EDIT2 = GetDlgItem(hwnd, IDC_EDIT2);//String Address
			for (auto x : (*Houses)) {
				std::string str = x.first;
				if (x.second)
					SendMessage(LBA, LB_ADDSTRING, NULL, (LPARAM)str.c_str());
				else
					SendMessage(LBB, LB_ADDSTRING, NULL, (LPARAM)str.c_str());
			}

			SetWindowText(EDIT2, std::to_string((int)(StringParam->first)).c_str());
			std::string &EditShowStr = *(StringParam->second);
			logger::g_logger.Info("Now processing house " + EditShowStr);
			SetWindowText(EDIT, EditShowStr.c_str());
			logger::g_logger.Info("Successfully done initialization of allie editor dialog box.");
			return TRUE;
		}
		case WM_COMMAND:{
			switch (wParam)
			{
				case IDOK:{
					logger::g_logger.Info("Allie editor confirmed.");
					HWND LBA = GetDlgItem(hwnd, IDC_LIST2);//Allies ListBox
					HWND EDIT = GetDlgItem(hwnd, IDC_EDIT1);
					HWND EDIT2 = GetDlgItem(hwnd, IDC_EDIT2);
					TCHAR str[256];
					GetWindowText(EDIT2, str, 11);
					std::string* retStr = (std::string*)(atoi(str));
					retStr->clear();
					int AllieCount = SendMessage(LBA, LB_GETCOUNT, NULL, NULL);
					for (int i = 0; i < AllieCount; ++i) {
						int TextLen = SendMessage(LBA, LB_GETTEXTLEN, i, NULL);
						if (TextLen == LB_ERR)	break;
						TCHAR* _str = new TCHAR[TextLen + 1];
						SendMessage(LBA, LB_GETTEXT, i, (LPARAM)_str);
						(*retStr) += ((std::string)_str + ',');
					}
					GetWindowText(EDIT, str, 256);
					(*retStr) += (std::string)str;
					EndDialog(hwnd, NULL);
					return TRUE;
				}
				case IDCANCEL: {
					logger::g_logger.Info("Allie editor cancelled.");
					EndDialog(hwnd, NULL);
					return TRUE;
				}
				case IDC_BUTTON1: {//Go Allies
					HWND LBA = GetDlgItem(hwnd, IDC_LIST2);//Allies ListBox
					HWND LBB = GetDlgItem(hwnd, IDC_LIST3);//Enemy ListBox
					int EnemyCount = SendMessage(LBB, LB_GETCOUNT, NULL, NULL);
					if (EnemyCount <= 0)	break;
					int EnemyCurSelIndex = SendMessage(LBB, LB_GETCURSEL, NULL, NULL);
					if (EnemyCurSelIndex < 0 || EnemyCurSelIndex >= EnemyCount)	break;
					int TextLen = SendMessage(LBB, LB_GETTEXTLEN, EnemyCurSelIndex, NULL);
					if (TextLen == LB_ERR)	break;
					TCHAR* str = new TCHAR[TextLen + 1];
					SendMessage(LBB, LB_GETTEXT, EnemyCurSelIndex, (LPARAM)str);
					SendMessage(LBB, LB_DELETESTRING, EnemyCurSelIndex, NULL);
					SendMessage(LBA, LB_ADDSTRING, NULL, (LPARAM)str);
					delete[] str;
					break;
				}
				case IDC_BUTTON2: {//Go Enemies
					HWND LBA = GetDlgItem(hwnd, IDC_LIST2);//Allies ListBox
					HWND LBB = GetDlgItem(hwnd, IDC_LIST3);//Enemy ListBox
					int AllieCount = SendMessage(LBA, LB_GETCOUNT, NULL, NULL);
					if (AllieCount <= 0)	break;
					int AllieCurSelIndex = SendMessage(LBA, LB_GETCURSEL, NULL, NULL);
					if (AllieCurSelIndex < 0 || AllieCurSelIndex >= AllieCount)	break;
					int TextLen = SendMessage(LBA, LB_GETTEXTLEN, AllieCurSelIndex, NULL);
					if (TextLen == LB_ERR)	break;
					TCHAR* str = new TCHAR[TextLen + 1];
					SendMessage(LBA, LB_GETTEXT, AllieCurSelIndex, (LPARAM)str);
					SendMessage(LBA, LB_DELETESTRING, AllieCurSelIndex, NULL);
					SendMessage(LBB, LB_ADDSTRING, NULL, (LPARAM)str);
					delete[] str;
					break;
				}
				default:
					break;
			}
			break;
		}
		case WM_CLOSE:{
			EndDialog(hwnd, NULL);
			return TRUE;
		}
	}
	return FALSE;
}

// Functionality Functions
std::string GetPath() {
	TCHAR* path = NULL;
	path = _getcwd(NULL, 0);
	std::string ret;
	if (path != NULL)
		ret = path;
	delete[] path;
	logger::g_logger.WriteLine("", "Dll Path : %s", ret.c_str());
	return ret;
}

void LoadTeamTemplates() {
	Ini& ini = Ini::ConfigIni;
	g_TeamTemplates.clear();

	//Read Team Templates
	int TeamTemplatesCount = atoi(ini.Read("TeamTemplates", "Counts").c_str());
	if (ini.Exist() == FALSE) {
		MessageBox(
			NULL,
			MessageBoxConfig::Instance.Message.IniNotExist.c_str(),
			MessageBoxConfig::Instance.Captain.Error.c_str(),
			MB_OK
		);
		TeamTemplatesCount = 0;
	}
	if (TeamTemplatesCount < 0)	TeamTemplatesCount = 0;

	logger::g_logger.Info(std::to_string(TeamTemplatesCount) + " Team Templates Loading");

	g_TeamTemplates.resize(TeamTemplatesCount + 1);
	*g_TeamTemplates[0][0] = ini.Read("TeamTemplates", "DefaultName");
	*g_TeamTemplates[0][1] = "New teamtype";
	*g_TeamTemplates[0][2] = "1";
	*g_TeamTemplates[0][3] = "5";
	*g_TeamTemplates[0][4] = "5";
	*g_TeamTemplates[0][5] = "-1";

	for (int i = 1; i <= TeamTemplatesCount; ++i) {
		std::string curstr = ini.Read("TeamTemplates", std::to_string(i));
		TeamTemplate teamTemplate = ini.Split(curstr, ',');
		g_TeamTemplates[i] = teamTemplate;
	}

	return;
}
void GetTreeViewHwnd() {
	logger::g_logger.Info("Tree View Handle has gotten");
	HWND Hwnd1 = FindWindowEx(g_FA2Wnd, NULL, "AfxFrameOrView42s", NULL);
	HWND Hwnd2 = FindWindowEx(Hwnd1, NULL, "AfxMDIFrame42s", NULL);
	g_SysTreeView = FindWindowEx(Hwnd2, NULL, "SysTreeView32", NULL);
	return;
}
void LoadINI() {
	logger::g_logger.Info("INI is loading...");
	std::string FA2CopyDataPath = g_Path;
	FA2CopyDataPath += "\\FA2CopyData.ini";
	Ini::ConfigIni = FA2CopyDataPath;
	if (!Ini::ConfigIni.Exist()) {
		MessageBox(
			NULL,
			MessageBoxConfig::Instance.Message.IniNotExist.c_str(),
			MessageBoxConfig::Instance.Captain.Error.c_str(),
			MB_OK
		);
		SendMessage(g_FA2Wnd, WM_CLOSE, NULL, NULL);
	}
}
void LoadFA2CopyConfig() {
	logger::g_logger.Info("Loading FA2Copy Config");

	//g_FindWindowConfig
	g_FindWindowConfig.AITriggerWnd = Ini::ConfigIni.Read("FindWindowConfig", "AITriggerWnd");
	g_FindWindowConfig.DialogClass = Ini::ConfigIni.Read("FindWindowConfig", "DialogClass");
	g_FindWindowConfig.MapWnd = Ini::ConfigIni.Read("FindWindowConfig", "MapWnd");
	g_FindWindowConfig.IniWnd = Ini::ConfigIni.Read("FindWindowConfig", "IniWnd");
	g_FindWindowConfig.HouseWnd = Ini::ConfigIni.Read("FindWindowConfig", "HouseWnd");
	g_FindWindowConfig.TriggerWnd = Ini::ConfigIni.Read("FindWindowConfig", "TriggerWnd");
	g_FindWindowConfig.TagWnd = Ini::ConfigIni.Read("FindWindowConfig", "TagWnd");
	g_FindWindowConfig.TriggerGlobalWnd = Ini::ConfigIni.Read("FindWindowConfig", "TriggerGlobalWnd");
	g_FindWindowConfig.TriggerEventWnd = Ini::ConfigIni.Read("FindWindowConfig", "TriggerEventWnd");
	g_FindWindowConfig.TriggerActionWnd = Ini::ConfigIni.Read("FindWindowConfig", "TriggerActionWnd");
	g_FindWindowConfig.TaskforceWnd = Ini::ConfigIni.Read("FindWindowConfig", "TaskforceWnd");
	g_FindWindowConfig.ScriptWnd = Ini::ConfigIni.Read("FindWindowConfig", "ScriptWnd");
	g_FindWindowConfig.TeamWnd = Ini::ConfigIni.Read("FindWindowConfig", "TeamWnd");
	g_FindWindowConfig.SaveWnd = Ini::ConfigIni.Read("FindWindowConfig", "SaveWnd");
	g_FindWindowConfig.LoadWnd = Ini::ConfigIni.Read("FindWindowConfig", "LoadWnd");
	g_FindWindowConfig.NewWnd1 = Ini::ConfigIni.Read("FindWindowConfig", "NewWnd1");
	g_FindWindowConfig.NewWnd2 = Ini::ConfigIni.Read("FindWindowConfig", "NewWnd2");
	g_FindWindowConfig.NewWnd3 = Ini::ConfigIni.Read("FindWindowConfig", "NewWnd3");

	// MessageBoxConfig
	MessageBoxConfig::Instance.Captain.Error = Ini::ConfigIni.Read("MessageBoxCaptain", "Error");
	MessageBoxConfig::Instance.Captain.Warning = Ini::ConfigIni.Read("MessageBoxCaptain", "Warning");
	MessageBoxConfig::Instance.Captain.Hint = Ini::ConfigIni.Read("MessageBoxCaptain", "Hint");

	MessageBoxConfig::Instance.Message.HookFailed = Ini::ConfigIni.Read("MessageBoxMessage", "HookFailed");
	MessageBoxConfig::Instance.Message.UnHookFailed = Ini::ConfigIni.Read("MessageBoxMessage", "UnHookFailed");
	MessageBoxConfig::Instance.Message.IniNotExist = Ini::ConfigIni.Read("MessageBoxMessage", "IniNotExist");
	MessageBoxConfig::Instance.Message.TerrainDisabled = Ini::ConfigIni.Read("MessageBoxMessage", "TerrainDisabled");
	MessageBoxConfig::Instance.Message.TerrainMapUnloaded = Ini::ConfigIni.Read("MessageBoxMessage", "TerrainMapUnloaded");
	MessageBoxConfig::Instance.Message.TerrainMapUnknown = Ini::ConfigIni.Read("MessageBoxMessage", "TerrainMapUnknown");
	MessageBoxConfig::Instance.Message.TriggerEventFull = Ini::ConfigIni.Read("MessageBoxMessage", "TriggerEventFull");
	MessageBoxConfig::Instance.Message.TriggerActionFull = Ini::ConfigIni.Read("MessageBoxMessage", "TriggerActionFull");
	MessageBoxConfig::Instance.Message.HouseWndNotFound = Ini::ConfigIni.Read("MessageBoxMessage", "HouseWndNotFound");
	MessageBoxConfig::Instance.Message.INISectionNotFound = Ini::ConfigIni.Read("MessageBoxMessage", "INISectionNotFound");
	
}
#pragma endregion

#pragma region DllMain Function
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:{
		g_hModule = hModule;
		logger::g_logger.WriteLine(
			"[Global]",
			"FA2Copy.dll is attaching...",
			true
		);
		if (!StartHook())
		{
			logger::g_logger.Error("Failed to set hooks!");
			MessageBox(
				NULL,
				MessageBoxConfig::Instance.Message.HookFailed.c_str(),
				MessageBoxConfig::Instance.Captain.Error.c_str(),
				MB_OK
			);
			return FALSE;
		}
		break;
	}
	case DLL_PROCESS_DETACH:{
		logger::g_logger.WriteLine(
			"[Global]",
			"FA2Copy.dll is detaching...",
			true
		);
		if (g_GetMsgHooked) {
			logger::g_logger.Info("Unregistering hot keys...");
		}
		if (EndHook() == FALSE)
		{
			logger::g_logger.Error("Failed to release hooks!");
			MessageBox(
				NULL,
				MessageBoxConfig::Instance.Message.UnHookFailed.c_str(),
				MessageBoxConfig::Instance.Captain.Error.c_str(),
				MB_OK
			);
			return FALSE;
		}
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}
#pragma endregion

#pragma region Export Function
__declspec(dllexport) void FA2CopyImportFunc()
{
	//Do nothing
}
#pragma endregion


#pragma region Inline Hooks

//DEFINE_HOOK(0, Dummy_Hook, 5)
//{
//	UNREFERENCED_PARAMETER(SyringeData::Hooks::_hk__0Dummy_Hook);
//
//	return 0;
//}


DEFINE_HOOK(537129, ExeRun, 9)
{
	FA2Expand::ExeRun();
	return 0;
}

DEFINE_HOOK(537208, ExeTerminate, 9)
{
	FA2Expand::ExeTerminate();
	GET(UINT, result, EAX);
	ExitProcess(result); //teehee
}

DEFINE_HOOK(551B07, CWnd_CreateDlg_FetchResource, 6)
{
	//GET(AFX_MODULE_STATE* const, pAfxModule, EAX);
	//GET(HMODULE, hModule, ESI);
	//GET(LPCTSTR, lpName, EBX);
	GET_STACK(LPCSTR, lpName, STACK_OFFS(0xC, -4));
	//GET(LPCTSTR, lpType, EDX);
	const HMODULE hModule = g_hModule;
	const LPCTSTR lpType = RT_DIALOG;

	if (IS_INTRESOURCE(lpName)) {
		logger::g_logger.Info(__FUNCTION__" lpName ID = " + std::to_string(LOWORD(lpName)));
	}
	else {
		logger::g_logger.Info(__FUNCTION__" lpName = " + std::string(lpName));
	}

	if (HRSRC hResInfo = FindResource(hModule, lpName, lpType)) {
		if (HGLOBAL hResData = LoadResource(hModule, hResInfo)) {
			R->ESI(hModule);
			R->EAX(hResData);
			return 0x551B20; //Resource locked and loaded (omg what a pun), return!
		}
	}
	return 0; //Nothing was found, try the game's own resources.
}

DEFINE_HOOK(551E57, sub_551E20_LoadResource, 5)
{
	const LPCSTR lpName = *(LPCSTR*)(R->ESI() + 0x40);

	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info("lpName = " + std::to_string((DWORD)lpName));

	const HMODULE hModule = g_hModule;
	const LPCTSTR lpType = RT_DIALOG;

	if (HRSRC hResInfo = FindResource(hModule, lpName, lpType)) {
		if (HGLOBAL hResData = LoadResource(hModule, hResInfo)) {
			R->EBX(hModule);
			R->EDI(hResData);
			return 0x551E6D; //Resource locked and loaded (omg what a pun), return!
		}
	}

	return 0;
}

DEFINE_HOOK(552147, sub_55212E, 5)
{
	const LPCSTR lpName = *(LPCSTR*)(R->EDI() + 0x40);

	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info("lpName = " + std::to_string((DWORD)lpName));

	const HMODULE hModule = g_hModule;
	const LPCTSTR lpType = RT_DIALOG;

	if (HRSRC hResInfo = FindResource(hModule, lpName, lpType)) {
		if (HGLOBAL hResData = LoadResource(hModule, hResInfo)) {
			R->EBX(hModule);
			R->EAX(hResData);
			return 0x552153; //Resource locked and loaded (omg what a pun), return!
		}
	}


	return 0;
}

#if 0
DEFINE_HOOK(554C8A, CWnd_ExecuteDlgInit, 9)
{
	GET_STACK(LPCSTR, lpName, STACK_OFFS(0xC, -4));
	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info("lpName = " + std::to_string((DWORD)lpName));

	if (IS_INTRESOURCE(lpName)) {
		if ((DWORD)lpName) {
		}
	}

	return 0;
}
#endif

DEFINE_HOOK(56537B, sub_56536A, 5)
{
	GET_BASE(LPCSTR, lpName, 0x8);
	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info("lpName = " + std::to_string((DWORD)lpName));

	return 0;
}

DEFINE_HOOK(551A82, sub_551A5B, 5)
{
	GET(LPCSTR, lpName, EBX);

	//logger::g_logger.Info(std::string(__FUNCTION__));

	const HMODULE hModule = g_hModule;
	const LPCTSTR lpType = RT_DIALOG;

	if (IS_INTRESOURCE(lpName)) {
		logger::g_logger.Info(__FUNCTION__" lpName ID = " + std::to_string(LOWORD(lpName)));
	} else {
		logger::g_logger.Info(__FUNCTION__" lpName = " + std::string(lpName));
	}

	if (HRSRC hResInfo = FindResource(hModule, lpName, lpType)) {
		if (HGLOBAL hResData = LoadResource(hModule, hResInfo)) {
			R->ESI(hModule);
			R->EAX(hResData);

			return 0x551A97; //Resource locked and loaded (omg what a pun), return!
		}
	}

	return 0;
}

DEFINE_HOOK(426B77, sub_426AC0, 5)
{
	logger::g_logger.Info(std::string(__FUNCTION__));

	return 0;
}

DEFINE_HOOK(49EDC9, LoadMap_Initialize, 9)
{
	//GET(INIClass*, pINI, EBX);
	auto& mapFile =  GlobalVars::INIFiles::CurrentDocument();

	auto const theater = mapFile.GetString("Map", "Theater", "TEMPERATE");

	g_TerrainTheater = theater;
	//logger::g_logger.Warn(__FUNCTION__" Value:" + std::string(static_cast<LPCTSTR>(theater)));

	return 0;
}




#pragma endregion
