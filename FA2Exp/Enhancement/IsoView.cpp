#include "../FA2Expand.h"
#include "MapDataExtra.h"
#include <CIsoView.h>
#include <CMapData.h>
#include <Helper/Template.h>

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

class CIsoViewExt
{
public:

	static void DrawLockedLines(
        CIsoView* pThis,
        const std::vector<std::pair<MapCoord, MapCoord>>& lines, 
        int X, int Y, 
        COLORREF color, 
        bool bUseDot, 
        bool bUsePrimary, 
        LPDDSURFACEDESC2 lpDesc
    );
    static void DrawLockedCellOutline(
        CIsoView* pThis,
        int X,
        int Y,
        int W,
        int H,
        COLORREF color,
        bool bUseDot,
        bool bUsePrimary,
        LPDDSURFACEDESC2 lpDesc
    );
};

void CIsoViewExt::DrawLockedCellOutline(
    CIsoView* pThis,
    int X,
    int Y, 
    int W, 
    int H, 
    COLORREF color, 
    bool bUseDot, 
    bool bUsePrimary, 
    LPDDSURFACEDESC2 lpDesc
)
{
    if (lpDesc->lpSurface == nullptr)
        return;

    RECT rect;
    pThis->GetWindowRect(&rect);

    auto lPitch = lpDesc->lPitch;
    auto nBytesPerPixel = *(int*)0x72A8C0;

    auto pRGB = (ColorStruct*)&color;
    BGRStruct ddColor;
    ddColor.R = pRGB->red;
    ddColor.G = pRGB->green;
    ddColor.B = pRGB->blue;

    auto DrawLine = [lPitch, nBytesPerPixel, ddColor, lpDesc, &rect](int X1, int Y1, int X2, int Y2) {
        int color = *(int*)&ddColor;

        if (X1 > X2) {
            std::swap(X1, X2);
            std::swap(Y1, Y2);
        }

        int dx = X2 - X1;
        int dy = Y2 - Y1;

        auto ptr = (unsigned char*)lpDesc->lpSurface + lPitch * Y1 + X1 * nBytesPerPixel;

        if (dy == 0) {
            for (int i = 0; i <= dx; ++i) {
                memcpy(ptr, &ddColor, nBytesPerPixel);
                ptr += nBytesPerPixel;
            }
        } else if (dx == 0) {
            int pitch = lPitch;
            if (dy < 0) {
                pitch = -pitch;
                dy = -dy;
            }

            for (int i = 0; i <= dy; ++i) {
                memcpy(ptr, &ddColor, nBytesPerPixel);
                ptr += pitch;
            }
        } else {
            int pitch = lPitch;
            if (dy < 0) {
                pitch = -pitch;
                dy = -dy;
            }

            int dx2 = 2 * dx;
            int dy2 = 2 * dy;

            if (dx > dy) {
                int delta = dy2 - dx;
                for (int i = 0; i < dx; ++i) {
                    memcpy(ptr + i * nBytesPerPixel, &ddColor, nBytesPerPixel);
                    if (delta > 0) {
                        ptr += pitch;
                        delta -= dx2;
                    }
                    delta += dy2;
                }
            } else {
                int delta = dx2 - dy;
                int k = 0;

                for (int i = 0; i < dy; ++i) {
                    memcpy(ptr + k * nBytesPerPixel, &ddColor, nBytesPerPixel);
                    if (delta > 0) {
                        ++k;
                        delta -= dy2;
                    }
                    delta += dx2;
                    ptr += pitch;
                }
            }
        }
    };
    auto ClipAndDrawLine = [&rect, DrawLine](int X1, int Y1, int X2, int Y2) {
        auto encode = [&rect](int x, int y) {
            int c = 0;
            if (x < rect.left) c = c | 0x1;
            else if (x > rect.right) c = c | 0x2;
            if (y > rect.bottom) c = c | 0x4;
            else if (y < rect.top) c = c | 0x8;
            return c;
        };
        auto clip = [&rect, encode](int& X1, int& Y1, int& X2, int& Y2) -> bool {
            int code1, code2, code;
            int x = 0, y = 0;
            code1 = encode(X1, Y1);
            code2 = encode(X2, Y2);
            while (code1 != 0 || code2 != 0) {
                if ((code1 & code2) != 0) return false;
                code = code1;
                if (code == 0) code = code2;
                if ((0b1 & code) != 0) {
                    x = rect.left;
                    y = Y1 + (Y2 - Y1) * (rect.left - X1) / (X2 - X1);
                } else if ((0b10 & code) != 0) {
                    x = rect.right;
                    y = Y1 + (Y2 - Y1) * (rect.right - X1) / (X2 - X1);
                } else if ((0b100 & code) != 0) {
                    y = rect.bottom;
                    x = X1 + (X2 - X1) * (rect.bottom - Y1) / (Y2 - Y1);
                } else if ((0b1000 & code) != 0) {
                    y = rect.top;
                    x = X1 + (X2 - X1) * (rect.top - Y1) / (Y2 - Y1);
                }
                if (code == code1) {
                    X1 = x;
                    Y1 = y;
                    code1 = encode(x, y);
                } else {
                    X2 = x;
                    Y2 = y;
                    code2 = encode(x, y);
                }
            }
            return true;
        };
        if (clip(X1, Y1, X2, Y2))
            DrawLine(X1, Y1, X2, Y2);
    };

    int halfCellWidth = 30 * W;
    int quaterCellWidth = 15 * W;
    int fullCellHeight = 30 * H;
    int halfCellHeight = 15 * H;

    int y1 = Y - 30;
    int x1 = X + 30;

    int x2 = halfCellWidth + X + 30;
    int y2 = quaterCellWidth + y1;

    int x3 = halfCellWidth - fullCellHeight + X + 29;
    int y3 = halfCellHeight + quaterCellWidth + y1 - 1;

    int x4 = X - fullCellHeight + 29;
    int y4 = halfCellHeight + y1 - 1;

    //   1
    //  # #
    // 4   2
    //  # #
    //   3

    ClipAndDrawLine(x1, y1, x2, y2);
    ClipAndDrawLine(x2, y2, x3, y3);
    ClipAndDrawLine(x3, y3, x4, y4);
    ClipAndDrawLine(x4, y4, x1, y1);

    // thicker
    if (!bUseDot) {
        ClipAndDrawLine(x1 + 1, y1, x2 + 1, y2);
        ClipAndDrawLine(x1 - 1, y1, x2 - 1, y2);
        ClipAndDrawLine(x1 + 2, y1, x2 + 2, y2);
        ClipAndDrawLine(x1 - 2, y1, x2 - 2, y2);

        ClipAndDrawLine(x2 + 1, y2, x3 + 1, y3);
        ClipAndDrawLine(x2 - 1, y2, x3 - 1, y3);
        ClipAndDrawLine(x2 + 2, y2, x3 + 2, y3);
        ClipAndDrawLine(x2 - 2, y2, x3 - 2, y3);

        ClipAndDrawLine(x3 + 1, y3, x4 + 1, y4);
        ClipAndDrawLine(x3 - 1, y3, x4 - 1, y4);
        ClipAndDrawLine(x3 + 2, y3, x4 + 2, y4);
        ClipAndDrawLine(x3 - 2, y3, x4 - 2, y4);

        ClipAndDrawLine(x4 + 1, y4, x1 + 1, y1);
        ClipAndDrawLine(x4 - 1, y4, x1 - 1, y1);
        ClipAndDrawLine(x4 + 2, y4, x1 + 2, y1);
        ClipAndDrawLine(x4 - 2, y4, x1 - 2, y1);
    }

}

void CIsoViewExt::DrawLockedLines(
    CIsoView* pThis,
    const std::vector<std::pair<MapCoord, MapCoord>>& lines,
    int X, 
    int Y, 
    COLORREF color, 
    bool bUseDot, 
    bool bUsePrimary, 
    LPDDSURFACEDESC2 lpDesc
)
{
    if (lpDesc->lpSurface == nullptr)
        return;

    RECT rect;
    pThis->GetWindowRect(&rect);

    auto lPitch = lpDesc->lPitch;
    auto nBytesPerPixel = *(int*)0x72A8C0;

    auto pRGB = (ColorStruct*)&color;
    BGRStruct ddColor;
    ddColor.R = pRGB->red;
    ddColor.G = pRGB->green;
    ddColor.B = pRGB->blue;

    auto DrawLine = [lPitch, nBytesPerPixel, ddColor, lpDesc, &rect](int X1, int Y1, int X2, int Y2) {
        int color = *(int*)&ddColor;

        if (X1 > X2) {
            std::swap(X1, X2);
            std::swap(Y1, Y2);
        }

        int dx = X2 - X1;
        int dy = Y2 - Y1;

        auto ptr = (unsigned char*)lpDesc->lpSurface + lPitch * Y1 + X1 * nBytesPerPixel;

        if (dy == 0) {
            for (int i = 0; i <= dx; ++i) {
                memcpy(ptr, &ddColor, nBytesPerPixel);
                ptr += nBytesPerPixel;
            }
        } else if (dx == 0) {
            int pitch = lPitch;
            if (dy < 0) {
                pitch = -pitch;
                dy = -dy;
            }

            for (int i = 0; i <= dy; ++i) {
                memcpy(ptr, &ddColor, nBytesPerPixel);
                ptr += pitch;
            }
        } else {
            int pitch = lPitch;
            if (dy < 0) {
                pitch = -pitch;
                dy = -dy;
            }

            int dx2 = 2 * dx;
            int dy2 = 2 * dy;

            if (dx > dy) {
                int delta = dy2 - dx;
                for (int i = 0; i < dx; ++i) {
                    memcpy(ptr + i * nBytesPerPixel, &ddColor, nBytesPerPixel);
                    if (delta > 0) {
                        ptr += pitch;
                        delta -= dx2;
                    }
                    delta += dy2;
                }
            } else {
                int delta = dx2 - dy;
                int k = 0;

                for (int i = 0; i < dy; ++i) {
                    memcpy(ptr + k * nBytesPerPixel, &ddColor, nBytesPerPixel);
                    if (delta > 0) {
                        ++k;
                        delta -= dy2;
                    }
                    delta += dx2;
                    ptr += pitch;
                }
            }
        }
    };
    auto ClipAndDrawLine = [&rect, DrawLine](int X1, int Y1, int X2, int Y2) {
        auto encode = [&rect](int x, int y) {
            int c = 0;
            if (x < rect.left) c = c | 0x1;
            else if (x > rect.right) c = c | 0x2;
            if (y > rect.bottom) c = c | 0x4;
            else if (y < rect.top) c = c | 0x8;
            return c;
        };
        auto clip = [&rect, encode](int& X1, int& Y1, int& X2, int& Y2) -> bool {
            int code1, code2, code;
            int x = 0, y = 0;
            code1 = encode(X1, Y1);
            code2 = encode(X2, Y2);
            while (code1 != 0 || code2 != 0) {
                if ((code1 & code2) != 0) return false;
                code = code1;
                if (code == 0) code = code2;
                if ((0b1 & code) != 0) {
                    x = rect.left;
                    y = Y1 + (Y2 - Y1) * (rect.left - X1) / (X2 - X1);
                } else if ((0b10 & code) != 0) {
                    x = rect.right;
                    y = Y1 + (Y2 - Y1) * (rect.right - X1) / (X2 - X1);
                } else if ((0b100 & code) != 0) {
                    y = rect.bottom;
                    x = X1 + (X2 - X1) * (rect.bottom - Y1) / (Y2 - Y1);
                } else if ((0b1000 & code) != 0) {
                    y = rect.top;
                    x = X1 + (X2 - X1) * (rect.top - Y1) / (Y2 - Y1);
                }
                if (code == code1) {
                    X1 = x;
                    Y1 = y;
                    code1 = encode(x, y);
                } else {
                    X2 = x;
                    Y2 = y;
                    code2 = encode(x, y);
                }
            }
            return true;
        };
        if (clip(X1, Y1, X2, Y2))
            DrawLine(X1, Y1, X2, Y2);
    };

    Y -= 30;
    X += 30;
    for (const auto& line : lines) {
        int x1 = X + line.first.X;
        int y1 = Y + line.first.Y;
        int x2 = X + line.second.X;
        int y2 = Y + line.second.Y;
        ClipAndDrawLine(x1, y1, x2, y2);
        ClipAndDrawLine(x1, y1, x2, y2);
        if (!bUseDot) {
            ClipAndDrawLine(x1 + 1, y1, x2 + 1, y2);
            ClipAndDrawLine(x1 - 1, y1, x2 - 1, y2);
            ClipAndDrawLine(x1 + 2, y1, x2 + 2, y2);
            ClipAndDrawLine(x1 - 2, y1, x2 - 2, y2);
        }
    }
}


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

DEFINE_HOOK(4709EE, CIsoView_Draw_ShowBuildingOutline, 6)
{
	GET(CIsoView*, pThis, EDI);
	GET(int, X, EBX);
	GET(int, Y, EBP);
	GET_STACK(int, W, STACK_OFFS(0xD18, 0xCFC));
	GET_STACK(int, H, STACK_OFFS(0xD18, 0xD00));
	GET_STACK(COLORREF, dwColor, STACK_OFFS(0xD18, 0xD04));
	LEA_STACK(LPDDSURFACEDESC2, lpDesc, STACK_OFFS(0xD18, 0x92C));

    auto constexpr backAddr = 0x470A38u;

	auto const it = CMapDataExt::BuildingDataExts.find(CMapDataExt::BuildingIndex);
    if (it == CMapDataExt::BuildingDataExts.end()) {
        return backAddr;
    }
    auto const& bldData = it->second;
    if (bldData.IsCustomFoundation()) {
        CIsoViewExt::DrawLockedLines(pThis, *bldData.LinesToDraw, X, Y, dwColor, false, false, lpDesc);
        return backAddr;
    }
	
    CIsoViewExt::DrawLockedCellOutline(pThis, X, Y, W, H, dwColor, false, false, lpDesc);
	return backAddr;
}

DEFINE_HOOK(4727B2, CIsoView_Draw_BasenodeOutline_CustomFoundation, B)
{
    GET_STACK(CIsoView*, pThis, STACK_OFFS(0xD18, 0xCD4));
    GET(int, X, EBX);
    GET(int, Y, EBP);
    GET_STACK(int, W, STACK_OFFS(0xD18, 0xCFC));
    GET_STACK(int, H, STACK_OFFS(0xD18, 0xD00));
    GET_STACK(COLORREF, dwColor, STACK_OFFS(0xD18, 0xB94));
    LEA_STACK(LPDDSURFACEDESC2, lpDesc, STACK_OFFS(0xD18, 0x92C));

    const auto& DataExt = CMapDataExt::BuildingDataExts[CMapDataExt::BuildingIndex];
    if (DataExt.IsCustomFoundation()) {
        CIsoViewExt::DrawLockedLines(pThis, *DataExt.LinesToDraw, X, Y, dwColor, true, false, lpDesc);
        CIsoViewExt::DrawLockedLines(pThis, *DataExt.LinesToDraw, X + 1, Y, dwColor, true, false, lpDesc);
    } else {
        CIsoViewExt::DrawLockedCellOutline(pThis, X, Y, W, H, dwColor, true, false, lpDesc);
        CIsoViewExt::DrawLockedCellOutline(pThis, X + 1, Y, W, H, dwColor, true, false, lpDesc);
    }
    return 0x472884;
}
