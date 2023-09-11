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
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam) { JMP_THIS(0x553BAC); }

	// get and set window ID, for child windows only
	CWnd* GetDlgItem(int nID) const { JMP_THIS(0x555452); }
	// get immediate child with given ID
	void GetDlgItem(int nID, HWND* phWnd) const { JMP_THIS(0x55547C); }
	void GetWindowText(CString& rString) const { JMP_THIS(0x553134); };
	void SetDlgItemText(int nID, LPCTSTR lpszString) { JMP_THIS(0x5554AA); }
	void SetWindowText(LPCTSTR lpszString) { JMP_THIS(0x5555A2); }

	int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK) { JMP_THIS(0x553E53); }

	//helper function;
	CString GetWindowText() const
	{
		CString ret;
		GetWindowText(ret);
		return ret;
	}

	// Window State Functions
	BOOL IsWindowEnabled() const { JMP_THIS(0x5556B2); }
	BOOL EnableWindow(BOOL bEnable = TRUE) { JMP_THIS(0x5556CD); }

	BOOL ShowWindow(int nCmdShow) { JMP_THIS(0x55568); }

	_AFXWIN_INLINE CWnd* GetWindow(UINT nCmd) const
	{ ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle(::GetWindow(m_hWnd, nCmd)); }

	static CWnd* PASCAL FromHandle(HWND hWnd) { JMP_THIS(0x55273E); }
};

FA2MFC_CLASS(CDialog) : public FA2::CWnd
{
public:
	CDialog() {}
	CDialog(UINT nIDTemplate, CWnd* pParentWnd = nullptr) JMP_THIS(0x551D6B);
	virtual ~CDialog() override JMP_THIS(0x551A1D);

	//CWnd
	virtual BOOL DestroyWindow() override { JMP_THIS(0x552FB5); }

	//CDialog
	virtual BOOL PreTranslateMessage(MSG* pMsg) override { JMP_THIS(0x5518E3); }

	virtual CRuntimeClass* GetRuntimeClass() const override { return reinterpret_cast<CRuntimeClass*>(0x59A528); }

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) override
		JMP_THIS(0x55197C);

	virtual const AFX_MSGMAP* GetMessageMap() const override JMP_THIS(0x5518DD);

	virtual BOOL CheckAutoCenter() override JMP_THIS(0x55212E);

	virtual int DoModal() JMP_THIS(0x551E20);

	virtual BOOL OnInitDialog() JMP_THIS(0x5520A8);

	virtual void OnSetFont(CFont* pFont) RX;

	virtual void OnOK() JMP_THIS(0x55210D);

	virtual void OnCancel() JMP_THIS(0x552126);

	virtual void PreInitDialog() RX;

	// normal functions
	BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = nullptr)
		JMP_THIS(0x551A5B);

	BOOL Create(UINT nIDTemplate, CWnd* pParentWnd)
		{ return Create(MAKEINTRESOURCE(nIDTemplate), pParentWnd); }

	void EndDialog(int nResult) JMP_THIS(0x551F80);

protected:
	UINT m_nIDHelp;                 // Help ID (0 for none, see HID_BASE_RESOURCE)

	// parameters for 'DoModal'
	LPCTSTR m_lpszTemplateName;     // name or MAKEINTRESOURCE
	HGLOBAL m_hDialogTemplate;      // indirect (m_lpDialogTemplate == NULL)
	LPCDLGTEMPLATE m_lpDialogTemplate;  // indirect if (m_lpszTemplateName == NULL)
	void* m_lpDialogInit;           // DLGINIT resource data
	CWnd* m_pParentWnd;             // parent/owner window
	HWND m_hWndTop;                 // top level parent window (may be disabled)
	//MFC macro, but always undefined because FA2 already uses
#if !defined (_AFX_NO_OCC_SUPPORT) || 1
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo) { JMP_THIS(0x551D5E); }
#endif
};
FA2MFC_END

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

class NOVTABLE FA2CView : public CView
{
public:
	// Cwnd
	virtual BOOL PreTransateMessage(MSG* pMsg) { JMP_THIS(0x55304D); }
};

class NOVTABLE FA2CWinApp : public CWinApp
{



};
