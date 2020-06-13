#include "../FA2Expand.h"
#include <CTaskForce.h>

//fix FA2 would automatically convert file saving prefix
DEFINE_HOOK(42703A, FA2Main_SaveMap_Extension, 9)
{
	return 0x42708D;
}

//i changed the notify time of this function , so only when focus lost on this control
//will go to this place
#if 0
bool IsEditingMember = false;

DEFINE_HOOK(4E44C7, CTaskForce_OnMemberTypeChanged, 7)
{
	GET(CTaskForce* const, pThis, ESI);
	GET_STACK(const char*, pStr, 0x28);

	logger::g_logger.Info(std::string(__FUNCTION__));
	logger::g_logger.Info(pStr);

	IsEditingMember = true;
	pThis->UpdateMemberTypeComboBox();

	pThis->CCBMemberType.SetWindowTextA(pStr);

	return 0x4E44DA;
}
#endif