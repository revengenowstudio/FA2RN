#include "CTaskForceExt.h"
#include "Helpers.h"
#include <functional>
CTaskForce* CTaskForceExt::Instance = nullptr;

void CTaskForceExt::ProgramStarupInit()
{
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596B50), CBN_KILLFOCUS);// member type update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596B38), EN_KILLFOCUS); // name update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596B20), EN_KILLFOCUS); // member count update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596BC8), EN_KILLFOCUS); // group update

	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x596C88), &CTaskForceExt::PreTranslateMessageHook);
}

BOOL CTaskForceExt::PreTranslateMessageHook(MSG * pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
#if 0
		logger::g_logger.Info(__FUNCTION__" WM_KEYDOWN received, value = " + std::to_string(pMsg->wParam));
		logger::g_logger.Info(__FUNCTION__" pMsg->hwnd = " + std::to_string((DWORD)pMsg->hwnd));
		logger::g_logger.Info(__FUNCTION__" child hwnd = " + std::to_string((DWORD)this->CCBMemberType.GetWindow(GW_CHILD)->m_hWnd));
		logger::g_logger.Info(__FUNCTION__" this->CCBMemberType.m_hWnd= " + std::to_string((DWORD)this->CCBMemberType.m_hWnd));
#endif
		switch (pMsg->wParam)
		{
			case VK_RETURN:
			{
				auto pEdit = this->CCBMemberType.GetWindow(GW_CHILD);
				if (pMsg->hwnd == pEdit->m_hWnd) {
					this->OnMemberEditChanged();
				}

				switch (::GetDlgCtrlID(pMsg->hwnd)) {
					case DLG_TaskForce_Edit_Name: this->OnTaskForceNameChanged();
					case DLG_TaskForce_Edit_MemberCount: this->OnMemberCountChanged();
					case DLG_TaskForce_Edit_GroupID: this->OnGroupIDChanged();
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

//inline hook methods:
//i changed the notify time of this function , so only when focus lost on this control
//will go to this place
DEFINE_HOOK(4E41A0, CTaskForce_OnMemberEditChanged, 6)
{
	GET(CTaskForce* const, pThis, ECX);

	CString windowText;
	pThis->CCBMemberType.GetWindowTextA(windowText);

	return windowText.GetLength() ? 0 : 0x4E4539;
}

#if 0
bool IsMemberTypeSelectedFromBox = false;
//now this one is used as "focus loss" handler
DEFINE_HOOK(4E44C7, CTaskForce_OnMemberEditChanged, 7)
{
	GET(CTaskForce* const, pThis, ESI);
	GET_STACK(const char*, pStr, 0x28);

	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info(pStr);

	pThis->UpdateMemberType();
	pThis->CCBMemberType.SetWindowTextA(pStr);

	return 0x4E44DA;
}
#endif

#if 0
DEFINE_HOOK(4E4540, CTaskForce_OnMemberSelectChanged, 7)
{
	GET(CTaskForce* const, pThis, ECX);

	logger::g_logger.Info(std::string(__FUNCTION__));
	pThis->OnMemberEditChanged();

	return 0x4E489A;
}

DEFINE_HOOK(4E4846, CTaskForce_OnMemberSelectChanged_Update, 7)
{
	GET(CTaskForce* const, pThis, ESI);
	GET_STACK(const char*, pStr, 0x20);

	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info(pStr);

	pThis->UpdateMemberType();
	//pThis->CCBMemberType.SetWindowTextA(pStr);
	IsMemberTypeSelectedFromBox = true;

	return 0x4E484D;
}

DEFINE_HOOK(4E358D, CTaskForce_OnItemListSelect_MemberType, C)
{
	GET(const char*, pStr, EAX);

	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info(pStr);

	return 0;
}

DEFINE_HOOK(4E1F7A, CTaskForce_UpdateMemberType, 8)
{
	GET_STACK(DWORD, caller, 0x128);
	GET(const char*, pStr, EDX);
	char buffer[0x20];
	sprintf_s(buffer, "%08X", caller);
	logger::g_logger.Info(__FUNCTION__" caller = " + std::string(buffer));
	logger::g_logger.Info(pStr);

	return 0;
}
#endif
