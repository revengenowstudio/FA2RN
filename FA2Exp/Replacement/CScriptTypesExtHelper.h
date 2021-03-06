#pragma once

#pragma once

#include "CScriptTypesExt.h"

#include <GlobalVars.h>

#include <unordered_set>

#include "../Meta/INIMeta.h"
#include "../Utilities/StringHelper.h"
#include "../Meta/ControlMeta.h"

class CScriptTypesFunctions
{
public:

	// 1
	static void LoadParams_Target(FA2::CComboBox& comboBox)
	{
		struct ParamaPair
		{
			const char* Str;
			int Idx;
		};
		static ParamaPair params[] = {
			{"0 - Not specified", 0},
			{"1 - Anything (uses auto-targeting)", 1},
			{"2 - Buildings", 2},
			{"3 - Harvesters", 3},
			{"4 - Infantry", 4},
			{"5 - Vehicles", 5},
			{"6 - Factories", 6},
			{"7 - Base defenses", 7},
			{"9 - Power plants", 9},
			{"10 - Occupiables", 10},
			{"11 - Tech Buildings", 11},
		};

		while (comboBox.DeleteString(0) != -1);

		for (auto const& param : params) {
			comboBox.SetItemData(comboBox.AddString(param.Str), param.Idx);
		}
	}

	// 2
	static void LoadParams_Waypoint(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& doc = GlobalVars::INIFiles::CurrentDocument();
		int waypoints[702];
		memset(waypoints, -1, sizeof waypoints);
		if (auto const entries = doc.TryGetSection("Waypoints")) {
			for (auto& x : entries->EntriesDictionary) {
				if (x.first != "Name" && !INIMeta::IsNullOrEmpty(x.second)) {
					int l = atoi(x.first);
					if (l <= 701 && l >= 0) {
						waypoints[l] = atoi(x.second);
					}
				}
			}
		}
		char buffer[0x20];
		for (int i = 0; i < 702; ++i) {
			if (waypoints[i] >= 0)
			{
				sprintf_s(buffer, "%u - (%u, %u)",
					i, waypoints[i] % 1000, waypoints[i] / 1000);
				int idx = comboBox.AddString(buffer);
				comboBox.SetItemData(idx, i);
			}
		}
	}

	// 3
	static void LoadParams_ScriptLine(FA2::CComboBox& comboBox, FA2::CComboBox& currentScript, FA2::CListBox& listBox)
	{
		int cnt = listBox.GetCount();
		// up to 50
		if (cnt > 50)
			cnt = 50;

		while (comboBox.DeleteString(0) != -1);

		auto& doc = GlobalVars::INIFiles::CurrentDocument();

		FA2::CString buffer, scriptName, parambuf;
		currentScript.GetLBText(currentScript.GetCurSel(), scriptName);
		utilities::trim_index(scriptName);

		for (int i = 0; i < cnt; ++i)
		{
			buffer.Format("%d", i);
			buffer = doc.GetString(scriptName, buffer, "0,0");
			int actionIndex = buffer.Find(',');
			if (actionIndex == CB_ERR)
				actionIndex = -1;
			else
				actionIndex = atoi(buffer.Mid(0, actionIndex));
			buffer.Format("%d - %s", i + 1, CScriptTypesExt::ExtActions[actionIndex].Name_);
			int idx = comboBox.AddString(buffer);
			comboBox.SetItemData(idx, i);
		}
	}

	// 4
	static void LoadParams_SplitGroup(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		comboBox.SetItemData(comboBox.AddString("0 - Keep Transports, Keep Units"), 0);
		comboBox.SetItemData(comboBox.AddString("1 - Keep Transports, Lose Units"), 1);
		comboBox.SetItemData(comboBox.AddString("2 - Lose Transports, Keep Units"), 2);
		comboBox.SetItemData(comboBox.AddString("3 - Lose Transports, Lose Units"), 3);
	}

	// 5
	static void LoadParams_GlobalVariables(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& rules = GlobalVars::INIFiles::Rules();
		if (auto const entities = rules.TryGetSection("VariableNames")) {
			FA2::CString text;
			for (auto& x : entities->EntriesDictionary) {
				if (x.first != "Name" && !INIMeta::IsNullOrEmpty(x.first)) {
					int l = atoi(x.first);
					text.Format("%d - %s", l, x.second);
					comboBox.SetItemData(comboBox.AddString(text), l);
				}
			}
		}
	}

	// 6
	static void LoadParams_ScriptTypes(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& doc = GlobalVars::INIFiles::CurrentDocument();
		if (auto const entries = doc.TryGetSection("ScriptTypes")) {
			FA2::CString text;
			for (auto& ent : entries->EntriesDictionary) {
				if (doc.SectionExists(ent.second) && !INIMeta::IsNullOrEmpty(ent.second)) {
					int id = atoi(ent.first);
					text = doc.GetString(ent.second, "Name");
					text.Format("%d - %s - %s", id, ent.second, text);
					comboBox.SetItemData(comboBox.AddString(text), id);
				}
			}
		}

	}

	// 7
	static void LoadParams_TeamTypes(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& doc = GlobalVars::INIFiles::CurrentDocument();
		if (auto const entries = doc.TryGetSection("TeamTypes")) {
			FA2::CString text;
			for (auto& ent : entries->EntriesDictionary) {
				if (doc.SectionExists(ent.second) && !INIMeta::IsNullOrEmpty(ent.second)) {
					int id = atoi(ent.first);
					text = doc.GetString(ent.second, "Name");
					text.Format("%d - %s - %s", id, ent.second, text);
					comboBox.SetItemData(comboBox.AddString(text), id);
				}
			}
		}

	}

	// 8
	static void LoadParams_Houses(FA2::CComboBox& comboBox)
	{
		ControlMeta::ComboBox::LoadHouses(comboBox, true);
	}

	// 9
	static void LoadParams_Speechs(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& eva = GlobalVars::INIFiles::Eva();
		if (auto const entries = eva.TryGetSection("DialogList"))
		{
			FA2::CString text;
			for (auto& ent : entries->EntriesDictionary)
			{
				if (eva.SectionExists(ent.second))
				{
					int id = atoi(ent.first);
					text.Format("%d - %s - %s", id, ent.second, eva.GetString(ent.second, "Text"));
					comboBox.SetItemData(comboBox.AddString(text), id);
				}
			}
		}
	}

	// 10
	static void LoadParams_Sounds(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& sound = GlobalVars::INIFiles::Sound();
		if (auto const entries = sound.TryGetSection("SoundList")) {
			FA2::CString text;
			for (auto& ent : entries->EntriesDictionary) {
				if (sound.SectionExists(ent.second) && !INIMeta::IsNullOrEmpty(ent.second)) {
					int id = atoi(ent.first);
					text.Format("%d - %s", id, ent.second);
					comboBox.SetItemData(comboBox.AddString(text), id);
				}
			}
		}
	}

	// 11
	static void LoadParams_Movies(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& art = GlobalVars::INIFiles::Art();
		if (auto const entries = art.TryGetSection("Movies")) {
			FA2::CString text;
			for (auto& ent : entries->EntriesDictionary) {
				if (ent.first != "Name") {
					int id = atoi(ent.first);
					text.Format("%d - %s", id, ent.second);
					comboBox.SetItemData(comboBox.AddString(text), id);
				}
			}
		}
	}

	// 12
	static void LoadParams_Themes(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& theme = GlobalVars::INIFiles::Theme();
		if (auto const entries = theme.TryGetSection("Themes")) {
			FA2::CString text;
			for (auto& ent : entries->EntriesDictionary) {
				if (theme.SectionExists(ent.second) && !INIMeta::IsNullOrEmpty(ent.second)) {
					int id = atoi(ent.first);
					text.Format("%d - %s", id, ent.second);
					comboBox.SetItemData(comboBox.AddString(text), id);
				}
			}
		}
	}

	// 13
	static void LoadParams_Countries(FA2::CComboBox& comboBox)
	{
		ControlMeta::ComboBox::LoadCountries(comboBox, true);
	}

	// 14
	static void LoadParams_LocalVariables(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		auto& doc = GlobalVars::INIFiles::CurrentDocument();
		if (auto const entities = doc.TryGetSection("VariableNames")) {
			FA2::CString text;
			for (auto& x : entities->EntriesDictionary) {
				if (INIMeta::IsNullOrEmpty(x.first) || x.first == "Name")
					continue;
				int l = atoi(x.first);
				text.Format("%d - %s", l, x.second);
				comboBox.SetItemData(comboBox.AddString(text), l);
			}
		}
	}

	// 15
	static void LoadParams_Facing(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		comboBox.SetItemData(comboBox.AddString("0 - NE"), 0);
		comboBox.SetItemData(comboBox.AddString("1 - E"), 1);
		comboBox.SetItemData(comboBox.AddString("2 - SE"), 2);
		comboBox.SetItemData(comboBox.AddString("3 - S"), 3);
		comboBox.SetItemData(comboBox.AddString("4 - SW"), 4);
		comboBox.SetItemData(comboBox.AddString("5 - W"), 5);
		comboBox.SetItemData(comboBox.AddString("6 - NW"), 6);
		comboBox.SetItemData(comboBox.AddString("7 - N"), 7);
	}

	// 16
	static void LoadParams_BuildingTypes(FA2::CComboBox& comboBox)
	{
		ControlMeta::ComboBox::LoadGenericList(comboBox, "BuildingTypes", true, true);
	}

	// 17
	static void LoadParams_Animations(FA2::CComboBox& comboBox)
	{
		ControlMeta::ComboBox::LoadGenericList(comboBox, "Animations", true, true);
	}

	// 18
	static void LoadParams_TalkBubble(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		comboBox.SetItemData(comboBox.AddString("0 - None"), 0);
		comboBox.SetItemData(comboBox.AddString("1 - Asterisk(*)"), 1);
		comboBox.SetItemData(comboBox.AddString("2 - Question mark(?)"), 2);
		comboBox.SetItemData(comboBox.AddString("3 - Exclamation mark(!)"), 3);
	}

	// 19
	static void LoadParams_Mission(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		const char* pStatus[] =
		{
			"Sleep",
			"Attack nearest enemy",
			"Move",
			"QMove",
			"Retreat home for R&R",
			"Guard",
			"Sticky (never recruit)",
			"Enter object",
			"Capture object",
			"Move into & get eaten",
			"Harvest",
			"Area Guard",
			"Return (to refinery)",
			"Stop",
			"Ambush (wait until discovered)",
			"Hunt",
			"Unload",
			"Sabotage (move in & destroy)",
			"Construction",
			"Deconstruction",
			"Repair",
			"Rescue",
			"Missile",
			"Harmless",
			"Open",
			"Patrol",
			"Paradrop approach drop zone",
			"Paradrop overlay drop zone",
			"Wait",
			"Attack again",
			"Spyplane approach",
			"Spyplane overfly"
		};

		for (int i = 0; i < 32; ++i) {
			char buffer[0x40];
			sprintf_s(buffer, "%u - %s", i, pStatus[i]);
			int idx = comboBox.AddString(buffer);
			if (idx >= 0) {
				comboBox.SetItemData(idx, i);
			}
		}
	}

	// 20
	static void LoadParams_Boolean(FA2::CComboBox& comboBox)
	{
		while (comboBox.DeleteString(0) != -1);

		comboBox.SetItemData(comboBox.AddString("0 - FALSE"), 0);
		comboBox.SetItemData(comboBox.AddString("1 - TRUE"), 1);
	}

};