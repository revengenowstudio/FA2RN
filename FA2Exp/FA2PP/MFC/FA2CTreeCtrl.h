#pragma once

#include <FA2PP.h>

class NOVTABLE FA2CTreeCtrl : public CTreeCtrl
{
public:
	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
		HTREEITEM hParent, HTREEITEM hInsertAfter) 
		FA2MFC_THISCALL(0x5503F7);
};

