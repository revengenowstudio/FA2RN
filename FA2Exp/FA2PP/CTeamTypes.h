#pragma once

#include <FA2PP.h>

class NOVTABLE CTeamTypes : public FA2::CDialog
{
public:
	CTeamTypes() { JMP_THIS(0x4E5860); }

	virtual void __thiscall DoDataExchange(CDataExchange* pDX) override
		{ JMP_STD(0x5971EC); }

	//non-virtual
	void OnNameEditChanged() { JMP_THIS(0x4E9B20); }
	void OnGroupComboBoxChanged() { JMP_THIS(0x4EAF30); }
	void OnWaypointComboBoxChanged() { JMP_THIS(0x4EB190); }
	void OnScriptComboBoxChanged() { JMP_THIS(0x4EB4D0); }
	void OnTaskforceComboBoxChanged() { JMP_THIS(0x4EB760); }
	void OnTagComboBoxChanged() { JMP_THIS(0x4EFE20); }
	void OnTransportWaypointComboBoxChanged() { JMP_THIS(0x4F01B0); }
	void OnMindControlDecisionComboBoxChanged() { JMP_THIS(0x4F0700); }

	void OnNewButtonClicked() { JMP_THIS(0x4EEA50); }
	void OnTeamSelectedCommboBoxChanged() { JMP_THIS(0x4E7900); }

	//member properties
	CStatic		CSTMindControlDecision; // 92
	CComboBox	CCBTeamList; // 152
	BOOL		BOOL_Aggressive; // 212
	BOOL		BOOL_Annoyance; // 216
	BOOL		BOOL_AreTeamMembersRecruitable; // 220
	BOOL		BOOL_Autocreate; // 224
	BOOL		BOOL_AvoidThreats; // 228
	BOOL		BOOL_CargoPlane; // 232
	BOOL		BOOL_Full; // 236
	CString		CString_Group; // 240
	BOOL		BOOL_GuardSlower; // 244
	CString		CString_House; // 248 Country infact
	BOOL		BOOL_IonImmune; // 252 Useless
	BOOL		BOOL_IsBaseDefense; // 256
	BOOL		BOOL_Loadable; // 260
	BOOL		BOOL_LooseRecruit; // 264
	CString		CString_Max; // 268
	CString		CString_Name; // 272
	BOOL		BOOL_OnlyTargetHouseEnemy; // 276
	BOOL		BOOL_OnTransOnly; // 280
	BOOL		BOOL_Prebuild; // 284
	CString		CString_Priority; // 288
	BOOL		BOOL_Recruiter; // 292;
	BOOL		BOOL_Reinforce; // 296
	CString		CString_Script; // 300
	BOOL		BOOL_Suicide; // 304
	CString		CString_Taskforce; // 308
	CString		CString_TechLevel; // 312
	BOOL		BOOL_TransportsReturnOnUnload; // 316
	CString		CString_Waypoint; // 320
	BOOL		BOOL_Whiner; // 324
	CString		CString_VeteranLevel; // 328
	CString		CString_Tag; // 332
	CString		CString_TransportWaypoint; // 336
	CString		CString_MindControlDecision; // 340
};