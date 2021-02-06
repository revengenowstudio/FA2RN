#pragma once

#include <GlobalDefinitions.h>
#include "../FA2Expand.h"

//addr must be a AFX_MSG

class HackHelper
{
public:
	static void ResetMessageType(DWORD addr, int msg)
	{
		RunTime::ResetMemoryContentAt(addr + ::AFXMessageTypeSize, &msg, 4);
	}










};

