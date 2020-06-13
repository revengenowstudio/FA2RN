#pragma once

#include <AFXWIN.H>
#include <FA2PPCore.h>

class NOVTABLE FA2CWnd : public CWnd
{
public:
};

class NOVTABLE FA2CDialog : public CDialog
{
public:
	//CWnd
	virtual BOOL DestroyWindow() override
		{ JMP_THIS(0x552FB5); }

	//CDialog
	virtual BOOL OnInitDialog() override
		{ JMP_THIS(0x5520A8); }
	 
	virtual BOOL PreTranslateMessage(MSG* pMsg) override
		{ JMP_THIS(0x5518E3); }

};

class NOVTABLE FA2CWinApp : public CWinApp
{



};
