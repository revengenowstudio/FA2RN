#include "../FA2Expand.h"
#include "../UI/resource.h"
#include <GlobalVars.h>
#include <FA2PP.h>

int MinPlayer = 2;

DEFINE_HOOK(4D57D2, SaveMapOption_OnInitDialog, 5)
{
	GET(const CDialog*, pThis, ESI);

	auto& mapFile = GlobalVars::INIFiles::CurrentDocument();

	auto const MinPlayer = mapFile.GetInteger("Basic", "MinPlayer", 2);
	auto const hwnd = GetDlgItem(pThis->m_hWnd, WND_SaveOption::Edit_MinPlayer);
	char szMinPlayer[4]{};
	_itoa_s(MinPlayer, szMinPlayer, 10);
	SendMessageA(hwnd, WM_SETTEXT, sizeof(szMinPlayer), (LPARAM)szMinPlayer);

	//LogWarn(" minPlayer:" + std::string(szMinPlayer));
	return 0;
}

DEFINE_HOOK(4D56E0, SaveMapOption_DoDataExchange, 8)
{
	GET(const CDialog*, pThis, ECX);

	auto const hwnd = GetDlgItem(pThis->m_hWnd, WND_SaveOption::Edit_MinPlayer);
	char szMinPlayer[4]{};
	SendMessageA(hwnd, WM_GETTEXT, 4, (LPARAM)szMinPlayer);
	//LogWarn(" " + std::string(szMinPlayer));
	MinPlayer = atoi(szMinPlayer);
	MinPlayer > 0 ? MinPlayer : 2;

	return 0;
}

DEFINE_HOOK(428229, SaveMap_MinPlayer, 8)
{
	using ItemIterator = std::FAMap<FA2::CString, FA2::CString>::iterator;
	GET(ItemIterator, pEntry, ESI);

	auto const minPlayerStr = std::to_string(MinPlayer);
	pEntry->second = minPlayerStr.c_str();

	//LogWarn(" " + std::string(pEntry->Value.c_str()));

	return 0x428236;
}