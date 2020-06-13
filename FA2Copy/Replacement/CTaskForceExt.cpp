#include "CTaskForceExt.h"

CTaskForce* CTaskForceExt::Instance = nullptr;

void CTaskForceExt::ProgramStarupInit()
{
	auto constexpr taskforceCBMemberTypeMsg = CBN_KILLFOCUS;
	auto taskforcePreTranslateAddr = &CTaskForceExt::PreTranslateMessageHook;

	RunTime::ResetMemoryContentAt(0x596B50 + sizeof(taskforceCBMemberTypeMsg), &taskforceCBMemberTypeMsg, sizeof(taskforceCBMemberTypeMsg));
	RunTime::ResetMemoryContentAt(0x596C88, &taskforcePreTranslateAddr, sizeof(taskforcePreTranslateAddr));
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
		case   VK_RETURN:
		{
			auto pEdit = this->CCBMemberType.GetWindow(GW_CHILD);
			if (pMsg->hwnd == pEdit->m_hWnd) {
				this->OnMemberTypeChanged();
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
