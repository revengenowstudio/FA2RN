#include "../FA2Expand.h"
#include <CIsoView.h>

class ConfigurationItems
{
public:
	int CopySelectionBound_Color{ 0x0000FF };
	int CursorSelectionBound_Color{ 0x3CA03C };
	int CursorSelectionBound_HeightColor{ 0x3C3C3C };
	int Waypoint_Color{ 0xFF0000 };
	bool Waypoint_Background{ true };
	int Waypoint_Background_Color{ 0x777777 };
} Configuration;

DEFINE_HOOK(459F4F, CIsoView_Draw_CopySelectionBoundColor, 6)
{
	R->Stack<COLORREF>(0x0, Configuration.CopySelectionBound_Color);
	return 0;
}

DEFINE_HOOK(45AD81, CIsoView_Draw_CursorSelectionBoundColor, 5)
{
	R->Stack<COLORREF>(0x0, Configuration.CursorSelectionBound_Color);
	return 0;
}

DEFINE_HOOK(45ADD0, CIsoView_Draw_CursorSelectionBoundHeightColor, 6)
{
	R->Stack<COLORREF>(0x8, Configuration.CursorSelectionBound_HeightColor);
	return 0;
}

DEFINE_HOOK(474A49, CIsoView_Draw_WaypointColor, 5)
{
	GET(CIsoView*, pThis, EBP);
	GET(int, X, ESI);
	GET(int, Y, EDI);
	GET_STACK(FA2::CString, TextItem, STACK_OFFS(0xD18, 0xCE4));

	pThis->DrawTextA(X + 15, Y + 7, TextItem, Configuration.Waypoint_Color);

	return 0x474A67;
}

DEFINE_HOOK(4685EA, CIsoView_DrawText, 9)
{
	if (Configuration.Waypoint_Background) {
		GET(HDC, hdc, EDX);

		SetBkColor(hdc, Configuration.Waypoint_Background_Color);
		SetBkMode(hdc, OPAQUE);

		return 0x4685F3;
	}
	return 0;
}