#include "CTeamTypesExt.h"
#include "Helpers.h"

void CTeamTypesExt::ProgramStartupInit()
{
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596D00), EN_KILLFOCUS); // name update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596D30), CBN_KILLFOCUS); // veteran update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596D48), CBN_KILLFOCUS); // house update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596D60), EN_KILLFOCUS); // priority update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596D78), EN_KILLFOCUS); // max update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596D90), CBN_KILLFOCUS); // techlevel update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596DA8), CBN_KILLFOCUS); // group update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596DC0), CBN_KILLFOCUS); // waypoint update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596DD8), CBN_KILLFOCUS); // script update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x596DF0), CBN_KILLFOCUS); // taskforce update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x5970C0), CBN_KILLFOCUS); // tag update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x5970F0), CBN_KILLFOCUS); // transport waypoint update
	HackHelper::ResetMessageType(AFX_MSG_STRUCT(0x597120), CBN_KILLFOCUS); // mind control decision update
}
