#include "CScriptTypesExt.h"
#include "Helpers.h"

void CScriptTypesExt::ProgramStartupInit()
{
	HackHelper::ResetMessageType(0x595FC8, EN_KILLFOCUS); // name update
	HackHelper::ResetMessageType(0x596010, CBN_KILLFOCUS); // param update
}
