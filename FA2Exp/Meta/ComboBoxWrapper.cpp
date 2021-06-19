#include "ControlMeta.h"
#include "../logger.h"

namespace ControlMeta
{
	void ComboBoxWrapper::EnableWindow(bool enable)
	{
		::EnableWindow(m_hwnd, enable);
	}

	void ComboBoxWrapper::Clear()
	{
		while ((int)::SendMessage(m_hwnd, CB_DELETESTRING, 0, 0) != -1);
	}

	void ComboBoxWrapper::SetWindowText(LPCSTR lpsz)
	{
		::SetWindowTextA(m_hwnd, lpsz);
	}













}