#include "CScriptTypesExt.h"
#include "Helpers.h"

void CScriptTypesExt::ProgramStartupInit()
{
	HackHelper::ResetMessageType(0x595FC8, EN_KILLFOCUS); // name update
	HackHelper::ResetMessageType(0x596010, CBN_KILLFOCUS); // param update

	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596148), &CScriptTypesExt::PreTranslateMessageHook);
}

BOOL CScriptTypesExt::PreTranslateMessageHook(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		{
			switch (::GetDlgCtrlID(pMsg->hwnd)) {
			case DLG_ScriptTypes_Edit_Name: this->OnNameEditChanged();
			default:
				break;
			}

		}

		//do not exit dialog when enter key pressed
		return TRUE;
		default:
			break;
		}
	}

	return this->FA2CDialog::PreTranslateMessage(pMsg);
}