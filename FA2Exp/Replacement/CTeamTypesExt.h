#pragma once
#include <CTeamTypes.h>

class CTeamTypesExt : public CTeamTypes
{
public:
	static void ProgramStartupInit();

	//member function replacements
	BOOL PreTranslateMessageHook(MSG * pMsg);


};