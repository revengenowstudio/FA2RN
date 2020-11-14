#pragma once
#include <CTaskForce.h>

class NOVTABLE CTaskForceExt : public CTaskForce
{
public:

	static CTaskForce* Instance;

	//hook function to replace in virtual function map
	BOOL PreTranslateMessageHook(MSG* pMsg);


	static void ProgramStarupInit();

	CTaskForceExt() {};
	~CTaskForceExt() {};

private:

};