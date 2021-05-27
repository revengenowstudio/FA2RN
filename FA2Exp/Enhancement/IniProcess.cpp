#include "../FA2Expand.h"
#include <INI.h>

DEFINE_HOOK(4530F7, CLoading_ParseINI_PlusSupport, 8)
{
	// length [0x1000]
	GET_STACK(INIClass*, pINI, STACK_OFFS(0x22FC, 0x22D0));
	LEA_STACK(char*, lpKey, STACK_OFFS(0x22FC, 0x200C));
	LEA_STACK(const char*, lpSection, STACK_OFFS(0x22FC, 0x210C));

	if (strcmp(lpKey, "+") == 0) {
		static unsigned inc = 0;
		sprintf_s(lpKey, 0x1000, "auto%u", inc++);
	}
	return 0;
}