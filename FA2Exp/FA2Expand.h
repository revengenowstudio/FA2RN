#pragma once
#include "ini.h"
#include "logger.h"
#include "RunTime.h"
#include "Macro.h"
#include "UI/Resource.h"
#include <MFC/FA2CString.h>

#define VERSION_STRVER "1.0.0"

class FA2Expand
{
public:
	static void __stdcall ExeRun();
	static void __stdcall ExeTerminate();

	FA2Expand();
	~FA2Expand();
};

