#include "CTileSetDialogBarExt.h"
#include "../Config.h"
#include <algorithm>
#include "../FA2Expand.h"

CTileSetDialogBar* CTileSetDialogBarExt::Instance = nullptr;
std::vector<TerrainSort> CTileSetDialogBarExt::TerrainSorts;


void CTileSetDialogBarExt::ProgramStarupInit()
{

}

#pragma region TerrainSort
//Class TerrainSort
TerrainSort::TerrainSort() {
	Data.clear();
}

TerrainSort::TerrainSort(std::vector<std::string> init) {
	Name = init[0];
	int count = atoi(init[1].c_str());
	Data.resize(count);
	for (int i = 0; i < count; ++i) {
		Data[i].first = init[i * 2 + 2];
		Data[i].second = init[i * 2 + 3];
	}
}

int TerrainSort::Count() {
	return Data.size();
}

void TerrainSort::Resize(int size) {
	Data.resize(size);
	return;
}

std::pair<std::string, std::string>* TerrainSort::operator[] (int index) {
	return &Data[index];
}

std::string TerrainSort::GetName() {
	return Name;
}
#pragma endregion

// Templates
void CTileSetDialogBarExt::LoadTerrainGroups(std::string Theater)
{
	using std::string;
	auto itr = Theater.begin();	++itr;
	transform(itr, Theater.end(), itr, tolower);
	string IniSectionName = "Terrain" + Theater;
	TerrainSorts.clear();
	Ini& ini = Ini::ConfigIni;
	if (!ini.Exist()) {
		::MessageBoxA(
			NULL,
			MessageBoxConfig::Instance.Message.IniNotExist.c_str(),
			MessageBoxConfig::Instance.Captain.Error.c_str(),
			MB_OK
		);
		return;
	}
	string Enable = ini.Read(IniSectionName, "Enable");
	if (Enable != "yes") {
		::MessageBoxA(
			NULL,
			MessageBoxConfig::Instance.Message.TerrainDisabled.c_str(),
			MessageBoxConfig::Instance.Captain.Hint.c_str(),
			MB_OK
		);
		return;
	}
	int Count = atoi(ini.Read(IniSectionName, "Counts").c_str());
	if (Count <= 0) {
		return;
	}
	LogInfo("%d Terrain Groups Loading", Count);
	TerrainSorts.reserve(Count);
	for (int i = 0; i < Count; ++i) {
		string str = ini.Read(IniSectionName, std::to_string(i + 1));
		TerrainSort terrainsort = ini.Split(str, ',');
		TerrainSorts.push_back(terrainsort);
	}

	HWND TerrainWnd = CTileSetDialogBarExt::Instance->GetSafeHwnd();
	HWND ComboMain = ::GetDlgItem(TerrainWnd, IDC_TerrainListWindow::ComboBox_Main);

	::SendMessageA(ComboMain, CB_RESETCONTENT, NULL, NULL);
	for (int idx = 0; idx < Count; ++idx) {
		::SendMessageA(ComboMain, CB_ADDSTRING, NULL, (LPARAM)(TerrainSorts[idx].GetName().c_str()));
	}

	::SendMessageA(ComboMain, CB_SETCURSEL, 0, NULL);
	::SendMessageA(TerrainWnd, WM_COMMAND, MAKEWPARAM(IDC_TerrainListWindow::ComboBox_Main, CBN_SELCHANGE), (LPARAM)ComboMain);
	return;
}

void CTileSetDialogBarExt::ReloadTileSets(const std::string& Theater)
{
	LogInfo("Reload Terrain Group");

	HWND TerrainWnd = CTileSetDialogBarExt::Instance->GetSafeHwnd();

	HWND ComboMain = ::GetDlgItem(TerrainWnd, 9983);
	HWND ComboSub = ::GetDlgItem(TerrainWnd, IDC_TerrainListWindow::ComboBox_Sub);

	::SendMessageA(ComboMain, CB_RESETCONTENT, NULL, NULL);
	::SendMessageA(ComboSub, CB_RESETCONTENT, NULL, NULL);
	::SendMessageA(ComboMain, CB_SETCURSEL, -1, NULL);
	::SendMessageA(ComboSub, CB_SETCURSEL, -1, NULL);

	const char* StandardTheater[] = { "TEMPERATE","SNOW","URBAN","NEWURBAN","LUNAR","DESERT" };
	if (Theater.empty()) {
		LogInfo("No map has been loaded while reading");
		::MessageBoxA(
			NULL,
			MessageBoxConfig::Instance.Message.TerrainMapUnloaded.c_str(),
			MessageBoxConfig::Instance.Captain.Hint.c_str(),
			MB_OK
		);
		return;
	}

	bool Flag = false;

	auto it = std::find_if(std::begin(StandardTheater), std::end(StandardTheater), [Theater](const char* item) {
		return Theater == item;
	});

	if (it == std::end(StandardTheater)) {
		LogError("Loaded unknown terrain threater");
		::MessageBoxA(
			NULL,
			MessageBoxConfig::Instance.Message.TerrainMapUnknown.c_str(),
			MessageBoxConfig::Instance.Captain.Error.c_str(),
			MB_OK
		);
	}

	LogInfo("Start to load terrain groups: %s", Theater.c_str());
	CTileSetDialogBarExt::LoadTerrainGroups(Theater);
}

void CTileSetDialogBarExt::OnGroupComboboxSelectionChanged()
{
	//LogInfo("Terrain Main SELCHANGE");
	//HWND TerrainWnd1 = FindWindowEx(g_FA2Wnd, NULL, "AfxFrameOrView42s", NULL);
	//HWND TerrainWnd2 = FindWindowEx(TerrainWnd1, NULL, "AfxMDIFrame42s", NULL);
	//EnumChildWindows(TerrainWnd2, EnumChildWindowsProc, NULL);
	HWND TerrainWnd = CTileSetDialogBarExt::Instance->GetSafeHwnd();
	HWND ComboMain = ::GetDlgItem(TerrainWnd, IDC_TerrainListWindow::ComboBox_Main);
	HWND ComboSub = ::GetDlgItem(TerrainWnd, IDC_TerrainListWindow::ComboBox_Sub);

	int MainCount = ::SendMessageA(ComboMain, CB_GETCOUNT, NULL, NULL);
	if (MainCount <= 0) {
		return;
	}
	int MainIndex = ::SendMessageA(ComboMain, CB_GETCURSEL, NULL, NULL);
	if (MainIndex < 0) {
		return;
	}
	int SubCount = TerrainSorts[MainIndex].Count();
	::SendMessageA(ComboSub, CB_RESETCONTENT, NULL, NULL);
	::SendMessageA(ComboSub, CB_SETCURSEL, -1, NULL);
	if (SubCount <= 0) {
		return;
	}
	for (int i = 0; i < SubCount; ++i) {
		::SendMessageA(ComboSub, CB_ADDSTRING, NULL, (LPARAM)TerrainSorts[MainIndex][i]->first.c_str());
	}
	::SendMessageA(ComboSub, CB_SETCURSEL, 0, NULL);
	::SendMessageA(TerrainWnd, WM_COMMAND, MAKEWPARAM(IDC_TerrainListWindow::ComboBox_Sub, CBN_SELCHANGE), (LPARAM)ComboSub);

}

void CTileSetDialogBarExt::OnSubTypeComboboxSelectionChanged()
{
	//LogInfo("Terrain Sub SELCHANGE");
	HWND TerrainWnd = CTileSetDialogBarExt::Instance->GetSafeHwnd();
	HWND ComboMain = ::GetDlgItem(TerrainWnd, 9983);
	HWND ComboSub = ::GetDlgItem(TerrainWnd, IDC_TerrainListWindow::ComboBox_Sub);
	HWND ComboReal = ::GetDlgItem(TerrainWnd, 1366);
	int MainCount = ::SendMessageA(ComboMain, CB_GETCOUNT, NULL, NULL);
	if (MainCount <= 0) {
		return;
	}
	int MainIndex = ::SendMessageA(ComboMain, CB_GETCURSEL, NULL, NULL);
	if (MainIndex < 0) {
		return;
	}
	int SubCount = TerrainSorts[MainIndex].Count();
	if (SubCount <= 0) {
		return;
	}
	int SubIndex = ::SendMessageA(ComboSub, CB_GETCURSEL, NULL, NULL);
	if (SubIndex < 0) {
		return;
	}
	std::string TerrainId = TerrainSorts[MainIndex][SubIndex]->second;
	int FindIndex = ::SendMessageA(ComboReal, CB_FINDSTRING, -1, (LPARAM)TerrainId.c_str());
	if (FindIndex == CB_ERR) {
		FindIndex = 0;
	}
	::SendMessageA(ComboReal, CB_SETCURSEL, FindIndex, NULL);
	::SendMessageA(TerrainWnd, WM_COMMAND, MAKEWPARAM(1366, CBN_SELCHANGE), (LPARAM)ComboReal);
}


DEFINE_HOOK(4F09EE, CTileSetBrowserFrame_CTOR, 6)
{
	GET(CTileSetDialogBar* const, pThis, ESI);

	CTileSetDialogBarExt::Instance = pThis;
	return 0;
}

DEFINE_HOOK(4F5DD8, CTileSetBrowserFrame_DTOR, 6)
{
	CTileSetDialogBarExt::Instance = nullptr;
	return 0;
}