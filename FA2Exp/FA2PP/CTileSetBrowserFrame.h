#pragma once
#include <CTileSetBrowserView.h>
#include <CTileSetDialogBar.h>

// SIZE : 464
// : CFrameWnd
class NOVTABLE CTileSetBrowserFrame : CFrameWnd
{
public:
	CTileSetDialogBar DialogBar;
	CTileSetBrowserView View;
};