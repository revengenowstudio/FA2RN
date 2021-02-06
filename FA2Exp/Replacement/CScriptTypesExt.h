#pragma once

#include <CScriptTypes.h>

class CScriptTypesExt : public CScriptTypes
{
public:
	static void ProgramStartupInit();
	BOOL PreTranslateMessageHook(MSG * pMsg);

};