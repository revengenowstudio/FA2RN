#include "CFinalSunDlgExt.h"
#include "Helpers.h"
#include "../Utilities/StringHelper.h"
#include "../Enhancement/MultiLanguage.h"

#include <algorithm>
#include <Uxtheme.h>

//#define BrowserRedraw_CleanUp_Separately
#define BrowserRedraw_GuessMode 1

//#include "../../Helpers/Translations.h"
//#include "../../Helpers/STDHelpers.h"
//#include "../../Helpers/MultimapHelper.h"

#include "../Meta/INIMeta.h"
#include <GlobalVars.h>
#include <CSFTable.h>

std::unordered_map<int, HTREEITEM> ObjectBrowserControlExt::ExtNodes;
std::unordered_set<std::string> ObjectBrowserControlExt::IgnoreSet;
std::unordered_set<std::string> ObjectBrowserControlExt::ExtSets[static_cast<int>(TreeViewTechnoType::Count)];
std::unordered_map<std::string, int> ObjectBrowserControlExt::KnownItem;
std::unordered_map<std::string, int> ObjectBrowserControlExt::Owners;

void CFinalSunDlgExt::ProgramStartupInit()
{
	HackHelper::ResetVirtualMemberFunction(VIRTUAL_TABLE_FUNC(0x5937E8), &CFinalSunDlgExt::PreTranslateMessageExt);
}

BOOL CFinalSunDlgExt::PreTranslateMessageExt(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_INITDIALOG:
		;
		///*SetWindowTheme(*this, L"DarkMode_Explorer", NULL);*/
	}
	return FA2CDialog::PreTranslateMessage(pMsg);
}

using vecSideNodeInfo = std::vector<std::pair<FA2::CString, TechnoTypeMask>>;
vecSideNodeInfo getSides()
{
	vecSideNodeInfo ret;
	auto const& fadata = GlobalVars::INIFiles::FAData();
	auto const pSection = fadata.TryGetSection("Sides");

	auto toType = [](const FA2::CString& str) -> TechnoTypeMask {
		return TechnoTypeMask(atoi(str));
	};

	if (!pSection) {
		return ret;
	}
	for (auto const& itemPair : pSection->EntriesDictionary) {
		ret.push_back({ itemPair.second, {} });
	}
	FA2::CString typeStr;
	for (auto& item : ret) {
		auto const commaPos = item.first.Find(',');
		//now parse real type
		if (commaPos >= 0) {
			typeStr = item.first.Mid(commaPos + 1);
			item.first = item.first.Mid(0, commaPos);
			item.second = toType(typeStr);
		}
	}

	return ret;
}

void ObjectBrowserControlExt::insertItemBySides(TreeViewTechnoType type, mpTreeNode& subNodes, HTREEITEM& item)
{
	auto const& sides = getSides();
	int i = 0;
	if (sides.empty()) {
		subNodes[i++] = this->InsertString("Allied", -1, item);
		subNodes[i++] = this->InsertString("Soviet", -1, item);
		subNodes[i++] = this->InsertString("Yuri", -1, item);
	} else {
		for (auto const& side : sides) {
			if (side.second & type) {
				subNodes[i++] = this->InsertString(side.first, -1, item);
			}
		}
	}
	auto const otherStr = Translations::TranslateOrDefault("Other", "Others");
	subNodes[-1] = this->InsertString(otherStr, -1, item);
}


//
void ObjectBrowserControlExt::Redraw()
{
#if !defined(BrowserRedraw_CleanUp_Separately)
	this->SelectItem(NULL);
	this->DeleteAllItems();
#endif
	Redraw_Initialize();
	Redraw_MainList();
	Redraw_Ground();
	Redraw_Owner();
	Redraw_Infantry();
	Redraw_Vehicle();
	Redraw_Aircraft();
	Redraw_Building();
	Redraw_Terrain();
	Redraw_Smudge();
	Redraw_Overlay();
	Redraw_Waypoint();
	Redraw_Celltag();
	Redraw_Basenode();
	Redraw_Tunnel();
	Redraw_PlayerLocation();
}

#if 1
HTREEITEM ObjectBrowserControlExt::InsertString(const char* pString, DWORD dwItemData,
	HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	return this->InsertItem(5, pString, 0, 0, 0, 0, dwItemData, hParent, hInsertAfter);
}

HTREEITEM ObjectBrowserControlExt::InsertTranslatedString(const char* pOriginString, DWORD dwItemData,
	HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	auto const& ret = Translations::Translate(pOriginString);
	return InsertString(ret.GetLength() ? ret : pOriginString, dwItemData, hParent, hInsertAfter);
}

void ObjectBrowserControlExt::Redraw_Initialize()
{
	ExtNodes.clear();
	KnownItem.clear();
	IgnoreSet.clear();
	Owners.clear();
	this->DeleteAllItems();

	auto& fadata = GlobalVars::INIFiles::FAData();
	auto const& mmh = INIMeta::GetRules();

	auto loadSet = [&mmh](const char* pTypeName, TreeViewTechnoType nType)
	{
		ExtSets[static_cast<int>(nType)].clear();
		auto& section = mmh.GetSectionItems(pTypeName);
		for (auto& item : section) {
			ExtSets[static_cast<int>(nType)].insert(std::string(item));
		}
	};

	loadSet("BuildingTypes", TreeViewTechnoType::Building);
	loadSet("InfantryTypes", TreeViewTechnoType::Infantry);
	loadSet("VehicleTypes", TreeViewTechnoType::Vehicle);
	loadSet("AircraftTypes", TreeViewTechnoType::Aircraft);

	auto loadOwner = [&mmh]()
	{
		auto& sides = mmh.GetIndices("Sides", true);
		for (size_t i = 0, sz = sides.size(); i < sz; ++i)
			for (auto& owner : utilities::split_string(sides[i]))
				Owners[(std::string)owner] = i;
	};
	loadOwner();

	if (auto const pSection = fadata.TryGetSection("IgnoreRA2")) {
		auto& ignores = pSection->EntriesDictionary;
		for (auto& item : ignores) {
			IgnoreSet.insert(std::string(item.second));
		}
	}
}

void ObjectBrowserControlExt::Redraw_MainList()
{
	ExtNodes[Root_Nothing] = this->InsertTranslatedString("NothingObList", -2);
	ExtNodes[Root_Ground] = this->InsertTranslatedString("GroundObList", 13);
	ExtNodes[Root_Owner] = this->InsertTranslatedString("ChangeOwnerObList");
	ExtNodes[Root_Infantry] = this->InsertTranslatedString("InfantryObList", 0);
	ExtNodes[Root_Vehicle] = this->InsertTranslatedString("VehiclesObList", 1);
	ExtNodes[Root_Aircraft] = this->InsertTranslatedString("AircraftObList", 2);
	ExtNodes[Root_Building] = this->InsertTranslatedString("StructuresObList", 3);
	ExtNodes[Root_Terrain] = this->InsertTranslatedString("TerrainObList", 4);
	ExtNodes[Root_Smudge] = this->InsertTranslatedString("SmudgesObList", 10);
	ExtNodes[Root_Overlay] = this->InsertTranslatedString("OverlayObList", 5);
	ExtNodes[Root_Waypoint] = this->InsertTranslatedString("WaypointsObList", 6);
	ExtNodes[Root_Celltag] = this->InsertTranslatedString("CelltagsObList", 7);
	ExtNodes[Root_Basenode] = this->InsertTranslatedString("BaseNodesObList", 8);
	ExtNodes[Root_Tunnel] = this->InsertTranslatedString("TunnelObList", 9);
	ExtNodes[Root_PlayerLocation] = this->InsertTranslatedString("StartpointsObList", 12);
	ExtNodes[Root_Delete] = this->InsertTranslatedString("DelObjObList", 10);
}

void ObjectBrowserControlExt::Redraw_Ground()
{
	HTREEITEM& hGround = ExtNodes[Root_Ground];
	if (hGround == NULL)    return;

	auto& doc = GlobalVars::INIFiles::CurrentDocument();
	FA2::CString theater = doc.GetString("Map", "Theater");
	if (theater == "NEWURBAN") {
		theater = "UBN";
	}

	FA2::CString suffix;
	if (theater.GetLength() > 0) {
		suffix = theater.Mid(0, 3);
	}

	this->InsertTranslatedString("GroundClearObList" + suffix, 61, hGround);
	this->InsertTranslatedString("GroundSandObList" + suffix, 62, hGround);
	this->InsertTranslatedString("GroundRoughObList" + suffix, 63, hGround);
	this->InsertTranslatedString("GroundGreenObList" + suffix, 65, hGround);
	this->InsertTranslatedString("GroundPaveObList" + suffix, 66, hGround);
	this->InsertTranslatedString("GroundWaterObList", 64, hGround);

	if (theater == "UBN") {
		this->InsertTranslatedString("GroundPave2ObListUBN", 67, hGround);
	}
}

//#define HintNonMultiplayerOwner
void ObjectBrowserControlExt::Redraw_Owner()
{
	HTREEITEM& hOwner = ExtNodes[Root_Owner];
	if (hOwner == NULL) { 
		return;
	}

	auto& doc = GlobalVars::INIFiles::CurrentDocument();

	bool bMultiplayer = doc.GetBool("Basic", "MultiplayerOnly");
	auto const& orderedSections = INIMeta::GetSectionItems(doc, "Houses");

#if defined(HintNonMultiplayerOwner)
	bool &bLoadOnlySpecial = bMultiplayer;
	if (bMultiplayer && orderedSections.size() > 2) {
		bLoadOnlySpecial =
		::MessageBox(
			NULL,
			"This map has MultiplayerOnly=yes but has more"
			"Houses than two (Neutral and Special), do you"
			"still want to load these houses?",
			"WARNNING",
			MB_YESNO
		) != IDYES;

	}
#endif
	for (size_t i = 0;  i < orderedSections.size(); ++i) {
		auto houseName = orderedSections[i];
#if defined(HintNonMultiplayerOwner)
		if (bMultiplayer
			&& section != "Neutral House" 
			&& section != "Special House") {
			continue;
		}
#endif
		if (houseName == "GDI" || houseName == "Nod") {
			continue;
		}
		//remove ' House' suffix
		auto const spacePos = houseName.Find(' ');
		if (spacePos >= 0) {
			houseName = houseName.Mid(0, spacePos);
		}
		houseName = CSFTable::GetUIName(houseName);
		this->InsertString(houseName, Const_House + i, hOwner);
	}
}

void ObjectBrowserControlExt::Redraw_Infantry()
{
	HTREEITEM& hInfantry = ExtNodes[Root_Infantry];
	if (hInfantry == NULL)   return;

	std::unordered_map<int, HTREEITEM> subNodes;

	this->insertItemBySides(TreeViewTechnoType::Infantry, subNodes, hInfantry);

	auto const& mmh = INIMeta::GetRules();
	auto& infantries = mmh.GetSectionItems("InfantryTypes");
	auto index = 0;
	for (auto& inf : infantries) {
		index++;
		if (IgnoreSet.find(std::string(inf)) != IgnoreSet.end()) {
			continue;
		}
		int side = GuessSide(inf, TreeViewTechnoType::Infantry);
		if (subNodes.find(side) == subNodes.end()) {
			side = -1;
		}
		this->InsertString(
			CSFTable::GetUIName(inf),
			Const_Infantry + index - 1,
			subNodes[side]
		);
	}

	// Clear up
#if defined(BrowserRedraw_CleanUp_Separately)
	for (auto& subnode : subNodes) {
		if (!this->ItemHasChildren(subnode.second)) {
			this->DeleteItem(subnode.second);
		}
	}
#endif
}

void ObjectBrowserControlExt::Redraw_Vehicle()
{
	HTREEITEM& hVehicle = ExtNodes[Root_Vehicle];

	if (hVehicle == NULL) {
		return;
	}

	std::unordered_map<int, HTREEITEM> subNodes;

	this->insertItemBySides(TreeViewTechnoType::Vehicle, subNodes, hVehicle);

	auto const& mmh = INIMeta::GetRules();
	auto& vehicles = mmh.GetSectionItems("VehicleTypes");
	auto index = 0;
	for (auto& veh : vehicles) {
		index++;
		if (IgnoreSet.find(std::string(veh)) != IgnoreSet.end()) {
			continue;
		}
		int side = GuessSide(veh, TreeViewTechnoType::Vehicle);
		if (subNodes.find(side) == subNodes.end()) {
			side = -1;
		}
		this->InsertString(
			CSFTable::GetUIName(veh),
			Const_Vehicle + index - 1,
			subNodes[side]
		);
	}

	// Clear up
#if defined(BrowserRedraw_CleanUp_Separately)
	for (auto& subnode : subNodes) {
		if (!this->ItemHasChildren(subnode.second)) {
			this->DeleteItem(subnode.second);
		}
	}
#endif
}

void ObjectBrowserControlExt::Redraw_Aircraft()
{
	HTREEITEM& hAircraft = ExtNodes[Root_Aircraft];
	if (hAircraft == NULL)   return;

	std::unordered_map<int, HTREEITEM> subNodes;

	auto& rules = GlobalVars::INIFiles::Rules();

	this->insertItemBySides(TreeViewTechnoType::Aircraft, subNodes, hAircraft);

	auto const& mmh = INIMeta::GetRules();
	auto& aircrafts = mmh.GetSectionItems("AircraftTypes");
	auto index = 0;
	for (auto& air : aircrafts) {
		index++;
		if (IgnoreSet.find(std::string(air)) != IgnoreSet.end()) {
			continue;
		}
		int side = GuessSide(air, TreeViewTechnoType::Aircraft);
		if (subNodes.find(side) == subNodes.end()) {
			side = -1;
		}
		this->InsertString(
			CSFTable::GetUIName(air),
			Const_Aircraft + index - 1,
			subNodes[side]
		);
	}

	// Clear up
#if defined(BrowserRedraw_CleanUp_Separately)
	for (auto& subnode : subNodes) {
		if (!this->ItemHasChildren(subnode.second)) {
			this->DeleteItem(subnode.second);
		}
	}
#endif
}

void ObjectBrowserControlExt::Redraw_Building()
{
	HTREEITEM& hBuilding = ExtNodes[Root_Building];
	if (hBuilding == NULL) {
		return;
	}
	std::unordered_map<int, HTREEITEM> subNodes;

	this->insertItemBySides(TreeViewTechnoType::Building, subNodes, hBuilding);

	auto const& mmh = INIMeta::GetRules();
	auto& buildings = mmh.GetSectionItems("BuildingTypes");
	auto index = 0;
	for (auto& bud : buildings)
	{
		index++;
		if (IgnoreSet.find(std::string(bud)) != IgnoreSet.end()) {
			continue;
		}
		int side = GuessSide(bud, TreeViewTechnoType::Building);
		if (subNodes.find(side) == subNodes.end()) {
			side = -1;
		}
		this->InsertString(
			CSFTable::GetUIName(bud),
			Const_Building + index - 1,
			subNodes[side]
		);
	}

	// Clear up
#if defined(BrowserRedraw_CleanUp_Separately)
	for (auto& subnode : subNodes) {
		if (!this->ItemHasChildren(subnode.second)) {
			this->DeleteItem(subnode.second);
		}
	}
#endif
}

void ObjectBrowserControlExt::Redraw_Terrain()
{
	HTREEITEM& hTerrain = ExtNodes[Root_Terrain];
	if (hTerrain == NULL) {
		return;
	}

	this->InsertTranslatedString("RndTreeObList", 50999, hTerrain);

	auto& rules = GlobalVars::INIFiles::Rules();
	auto const& indices = INIMeta::GetSectionItems(rules, "TerrainTypes");

	for (auto i = 0; i < indices.size(); ++i) {
		auto const& item = indices[i];
		if (IgnoreSet.find(item.operator LPCTSTR()) == IgnoreSet.end()) {
			FA2::CString buffer = CSFTable::GetUIName(item);
			this->InsertString(buffer, Const_Terrain + i, hTerrain);
		}
	}
}

void ObjectBrowserControlExt::Redraw_Smudge()
{
	HTREEITEM& hSmudge = ExtNodes[Root_Smudge];
	if (hSmudge == NULL) { 
		return; 
	}

	auto& rules = GlobalVars::INIFiles::Rules();
	auto const& indices = INIMeta::GetSectionItems(rules, "SmudgeTypes");

	for (size_t i = 0;i < indices.size(); ++i) {
		auto const& item = indices[i];
		if (IgnoreSet.find(item.operator LPCTSTR()) == IgnoreSet.end()) {
			this->InsertString(item, Const_Smudge + i, hSmudge);
		}
	}
}

void ObjectBrowserControlExt::Redraw_Overlay()
{
	HTREEITEM& hOverlay = ExtNodes[Root_Overlay];
	if (hOverlay == NULL) {
		return;
	}

	auto& rules = GlobalVars::INIFiles::Rules();

	HTREEITEM hTemp;
	hTemp = this->InsertTranslatedString("DelOvrlObList", -1, hOverlay);
	this->InsertTranslatedString("DelOvrl0ObList", 60100, hTemp);
	this->InsertTranslatedString("DelOvrl1ObList", 60101, hTemp);
	this->InsertTranslatedString("DelOvrl2ObList", 60102, hTemp);
	this->InsertTranslatedString("DelOvrl3ObList", 60103, hTemp);

	hTemp = this->InsertTranslatedString("GrTibObList", -1, hOverlay);
	this->InsertTranslatedString("DrawTibObList", 60210, hTemp);
	this->InsertTranslatedString("DrawTib2ObList", 60310, hTemp);

	hTemp = this->InsertTranslatedString("BridgesObList", -1, hOverlay);
	this->InsertTranslatedString("BigBridgeObList", 60500, hTemp);
	this->InsertTranslatedString("SmallBridgeObList", 60501, hTemp);
	this->InsertTranslatedString("BigTrackBridgeObList", 60502, hTemp);
	this->InsertTranslatedString("SmallConcreteBridgeObList", 60503, hTemp);

	// Walls
	HTREEITEM hWalls = this->InsertTranslatedString("OthObList", -1, hOverlay);

	hTemp = this->InsertTranslatedString("AllObList", -1, hOverlay);

	this->InsertTranslatedString("OvrlManuallyObList", 60001, hOverlay);
	this->InsertTranslatedString("OvrlDataManuallyObList", 60002, hOverlay);

	if (!rules.SectionExists("OverlayTypes")) {
		return;
	}

	// a rough support for tracks
	this->InsertTranslatedString("Tracks", Const_Overlay + 39, hOverlay);

	auto const& indices = INIMeta::GetSectionItems(rules, "OverlayTypes");
	auto const indiceSize = std::min<unsigned int>(indices.size(), 255);
	for (size_t i = 0;i < indiceSize; ++i) {
		auto const& item = indices[i];
		auto const& name = CSFTable::GetUIName(item);
		if (rules.GetBool(item, "Wall") && rules.GetBool(item, "Wall.HasConnection", true)) {
			this->InsertString( name, Const_Overlay + i, hWalls);
		}
		if (IgnoreSet.find(item.operator LPCTSTR()) == IgnoreSet.end()) {
			this->InsertString(name, Const_Overlay + i, hTemp);
		}
	}
}

void ObjectBrowserControlExt::Redraw_Waypoint()
{
	HTREEITEM& hWaypoint = ExtNodes[Root_Waypoint];
	if (hWaypoint == NULL)   return;

	this->InsertTranslatedString("CreateWaypObList", 20, hWaypoint);
	this->InsertTranslatedString("DelWaypObList", 21, hWaypoint);
	this->InsertTranslatedString("CreateSpecWaypObList", 22, hWaypoint);
}

void ObjectBrowserControlExt::Redraw_Celltag()
{
	HTREEITEM& hCellTag = ExtNodes[Root_Celltag];
	if (hCellTag == NULL)   return;

	this->InsertTranslatedString("CreateCelltagObList", 36, hCellTag);
	this->InsertTranslatedString("DelCelltagObList", 37, hCellTag);
	this->InsertTranslatedString("CelltagPropObList", 38, hCellTag);
}

void ObjectBrowserControlExt::Redraw_Basenode()
{
	HTREEITEM& hBasenode = ExtNodes[Root_Basenode];
	if (hBasenode == NULL)   return;

	this->InsertTranslatedString("CreateNodeNoDelObList", 40, hBasenode);
	this->InsertTranslatedString("CreateNodeDelObList", 41, hBasenode);
	this->InsertTranslatedString("DelNodeObList", 42, hBasenode);
}

void ObjectBrowserControlExt::Redraw_Tunnel()
{
	HTREEITEM& hTunnel = ExtNodes[Root_Tunnel];
	if (hTunnel == NULL)   return;

	if (GlobalVars::INIFiles::FAData().GetBool("Debug", "AllowTunnels"))
	{
		this->InsertTranslatedString("NewTunnelObList", 50, hTunnel);
		this->InsertTranslatedString("DelTunnelObList", 51, hTunnel);
	}
}

void ObjectBrowserControlExt::Redraw_PlayerLocation()
{
	HTREEITEM& hPlayerLocation = ExtNodes[Root_PlayerLocation];
	if (hPlayerLocation == NULL)   return;

	this->InsertTranslatedString("StartpointsDelete", 21, hPlayerLocation);
}

TreeViewTechnoType ObjectBrowserControlExt::GuessType(const char* pRegName)
{
	if (ExtSets[static_cast<int>(TreeViewTechnoType::Building)].find(pRegName) != ExtSets[static_cast<int>(TreeViewTechnoType::Building)].end())
		return  TreeViewTechnoType::Building;
	if (ExtSets[static_cast<int>(TreeViewTechnoType::Infantry)].find(pRegName) != ExtSets[static_cast<int>(TreeViewTechnoType::Infantry)].end())
		return  TreeViewTechnoType::Infantry;
	if (ExtSets[static_cast<int>(TreeViewTechnoType::Vehicle)].find(pRegName) != ExtSets[static_cast<int>(TreeViewTechnoType::Vehicle)].end())
		return  TreeViewTechnoType::Vehicle;
	if (ExtSets[static_cast<int>(TreeViewTechnoType::Aircraft)].find(pRegName) != ExtSets[static_cast<int>(TreeViewTechnoType::Aircraft)].end())
		return  TreeViewTechnoType::Aircraft;
	return  TreeViewTechnoType::Set_None;
}

int ObjectBrowserControlExt::GuessSide(const char* pRegName, TreeViewTechnoType nType)
{
	auto& knownIterator = KnownItem.find(pRegName);
	if (knownIterator != KnownItem.end())
		return knownIterator->second;

	int result = -1;
	switch (nType)
	{
	case  TreeViewTechnoType::Set_None:
	default:
		break;
	case  TreeViewTechnoType::Building:
		result = GuessBuildingSide(pRegName);
		break;
	case  TreeViewTechnoType::Infantry:
		result = GuessGenericSide(pRegName, TreeViewTechnoType::Infantry);
		break;
	case  TreeViewTechnoType::Vehicle:
		result = GuessGenericSide(pRegName, TreeViewTechnoType::Vehicle);
		break;
	case  TreeViewTechnoType::Aircraft:
		result = GuessGenericSide(pRegName, TreeViewTechnoType::Aircraft);
		break;
	}
	KnownItem[pRegName] = result;
	return result;
}

int ObjectBrowserControlExt::GuessBuildingSide(const char* pRegName)
{
	auto& rules = GlobalVars::INIFiles::Rules();

	int planning;
	planning = rules.GetInteger(pRegName, "AIBasePlanningSide", -1);
	if (planning >= rules.GetKeyCount("Sides")) {
		return -1;
	}
	if (planning >= 0) {
		return planning;
	}
	auto& cons = utilities::split_string(rules.GetString("AI", "BuildConst"));
	int i;
	for (i = 0; i < cons.size(); ++i)
	{
		if (cons[i] == pRegName)
			return i;
	}
	if (i >= rules.GetKeyCount("Sides"))
		return -1;
	return GuessGenericSide(pRegName, TreeViewTechnoType::Building);
}

int ObjectBrowserControlExt::GuessGenericSide(const char* pRegName, TreeViewTechnoType nType)
{
	auto const& mmh = INIMeta::GetRules(); \
		auto const& set = ExtSets[static_cast<int>(nType)];

	if (set.find(pRegName) == set.end()) {
		return -1;
	}

#if BrowserRedraw_GuessMode == 0
	for (auto& prep : utilities::split_string(mmh.GetString(pRegName, "Prerequisite")))
	{
		int guess = -1;
		for (auto& subprep : utilities::split_string(mmh.GetString("GenericPrerequisites", prep.c_str())))
		{
			guess = GuessSide(subprep.c_str(), GuessType(subprep.c_str()));
			if (guess != -1)
				return guess;
		}
		guess = GuessSide(prep.c_str(), GuessType(prep.c_str()));
		if (guess != -1)
			return guess;
	}
	return -1;
#elif BrowserRedraw_GuessMode == 1
	auto& owners = utilities::split_string(mmh.GetString(pRegName, "Owner"));
	if (owners.size() <= 0) {
		return -1;
	}
	auto& itr = Owners.find((std::string)owners[0]);
	if (itr == Owners.end()) {
		return -1;
	}
	return itr->second;
#endif
}

// ObjectBrowserControlExt::OnSelectChanged

int ObjectBrowserControlExt::UpdateEngine(int nData)
{
	if (nData < 10000)
		switch (nData)
		{
		case 114514:
			return 0x51BB28;
		default:
			return 0x51AFBE;
		} else
			return 0x51BB28;
}

#endif

#define TakeOwer_Redraw
DEFINE_HOOK(51CD20, ObjectBrowserControl_Redraw, 7)
{
#if defined(TakeOwer_Redraw)
	GET(ObjectBrowserControlExt*, pThis, ECX);
	pThis->Redraw();
	return 0x523173;
#else
	return 0;
#endif
}

//DEFINE_HOOK(51AFB8, ObjectBrowserControl_OnSelectedChanged, 6)
//{
//    GET_STACK(ObjectBrowserControlExt*, pThis, 0x10);
//    GET(int, nData, ECX);
//    return pThis->UpdateEngine(nData);
//}

//void PrintNode(CTreeCtrl* pTree, HTREEITEM hNode)
//{
//    static int depth = 0;
//    if (pTree->ItemHasChildren(hNode))
//    {
//        HTREEITEM hNextItem;
//        HTREEITEM hChildItem = pTree->GetChildItem(hNode);
//
//        while (hChildItem != NULL)
//        {
//            CString spaces;
//            for (int i = 0; i < depth; ++i)
//                spaces += "    ";
//            Logger::Debug("%s%s %d\n", spaces, pTree->GetItemText(hChildItem), pTree->GetItemData(hChildItem));
//            if (pTree->ItemHasChildren(hChildItem))
//            {
//                ++depth;
//                PrintNode(pTree, hChildItem);
//                --depth;
//            }
//            hNextItem = pTree->GetNextItem(hChildItem, TVGN_NEXT);
//            hChildItem = hNextItem;
//        }
//    }
//}
//
//DEFINE_HOOK(523139, ObjectBrowserControl_Redraw_End, 5)
//{
//    GET(CTreeCtrl*, pTree, ESI);
//    auto hRoot = pTree->GetSelectedItem();
//    
//    PrintNode(pTree, hRoot);
//    
//    return 0;
//}