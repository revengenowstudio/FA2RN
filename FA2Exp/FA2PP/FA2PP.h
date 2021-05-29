#pragma once

#include <AFXWIN.H>
#include <AFXCMN.H>
#include <FA2PPCore.h>
#include <MFC/FA2MFC.h>
#include <MFC/FA2CString.h>

FA2MFC_BEGIN


FA2MFC_CLASS(CWnd) : public ::CWnd
{
public:
	// get and set window ID, for child windows only
	CWnd* GetDlgItem(int nID) const { JMP_THIS(0x555452); }
	// get immediate child with given ID
	void GetDlgItem(int nID, HWND* phWnd) const { JMP_THIS(0x55547C); }
	void GetWindowText(CString& rString) const { JMP_THIS(0x553134); };
	void SetDlgItemText(int nID, LPCTSTR lpszString) { JMP_THIS(0x5554AA); }
	void SetWindowText(LPCTSTR lpszString) { JMP_THIS(0x5555A2); }

	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK) { JMP_THIS(0x553E53); }

	// Window State Functions
	BOOL IsWindowEnabled() const { JMP_THIS(0x5556B2); }
	BOOL EnableWindow(BOOL bEnable = TRUE) { JMP_THIS(0x5556CD); }

	BOOL ShowWindow(int nCmdShow) { JMP_THIS(0x55568); }

	_AFXWIN_INLINE CWnd* GetWindow(UINT nCmd) const
	{ ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle(::GetWindow(m_hWnd, nCmd)); }

	static CWnd* PASCAL FromHandle(HWND hWnd) { JMP_THIS(0x55273E); }
};

FA2MFC_END

class NOVTABLE FA2CDialog : public FA2::CWnd /*CDialog*/
{
public:
	//CWnd
	virtual BOOL DestroyWindow() override { JMP_THIS(0x552FB5); }

	//CDialog
	virtual BOOL OnInitDialog() { JMP_THIS(0x5520A8); }
	 
	virtual BOOL PreTranslateMessage(MSG* pMsg) override { JMP_THIS(0x5518E3); }

protected:
	UINT m_nIDHelp;                 // Help ID (0 for none, see HID_BASE_RESOURCE)

	// parameters for 'DoModal'
	LPCTSTR m_lpszTemplateName;     // name or MAKEINTRESOURCE
	HGLOBAL m_hDialogTemplate;      // indirect (m_lpDialogTemplate == NULL)
	LPCDLGTEMPLATE m_lpDialogTemplate;  // indirect if (m_lpszTemplateName == NULL)
	void* m_lpDialogInit;           // DLGINIT resource data
	CWnd* m_pParentWnd;             // parent/owner window
	HWND m_hWndTop;                 // top level parent window (may be disabled)

#ifndef _AFX_NO_OCC_SUPPORT
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo) { JMP_THIS(0x551D5E); }
#endif
	virtual void PreInitDialog();

};

class NOVTABLE CTreeView : public CCtrlView
{
public:
	afx_msg void OnDestroy() { JMP_THIS(0x563E04); }


};

class NOVTABLE CViewObjects : public CCtrlView
{
public:


	DWORD unk1;
	DWORD unk2;
};

class NOVTABLE FA2CWinApp : public CWinApp
{



};
