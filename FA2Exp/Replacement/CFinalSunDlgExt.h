#pragma once

#include "../FA2Expand.h"
#include <CFinalSunDlg.h>

#include <unordered_set>
#include <unordered_map>

class NOVTABLE CFinalSunDlgExt : CFinalSunDlg
{
public:
    static void ProgramStartupInit();

    BOOL PreTranslateMessageExt(MSG* pMsg);
};

#define MAKE_MASK(refVal) 1 << static_cast<int>(refVal) 

enum class TreeViewTechnoType {
	Set_None = -1,
	Building = 0,
	Infantry,
	Vehicle,
	Aircraft,
	_Last,
	_First = 0,
	Count = _Last,
};

enum class TechnoTypeMask
{
	ForBuilding = MAKE_MASK(TreeViewTechnoType::Building),
	ForInfantry = MAKE_MASK(TreeViewTechnoType::Infantry),
	ForVehicle = MAKE_MASK(TreeViewTechnoType::Vehicle),
	ForAircraft = MAKE_MASK(TreeViewTechnoType::Aircraft),
};

inline bool operator&(TechnoTypeMask lhs, TechnoTypeMask rhs)
{
	return static_cast<int>(lhs) & static_cast<int>(rhs);
}
inline bool operator&(TechnoTypeMask lhs, TreeViewTechnoType rhs)
{
	return lhs & static_cast<TechnoTypeMask>(MAKE_MASK(rhs));
}
inline bool operator&(TreeViewTechnoType lhs, TechnoTypeMask rhs)
{
	return rhs & static_cast<TechnoTypeMask>(MAKE_MASK(lhs));
}

class ObjectBrowserControlExt : public ObjectBrowserControl
{
    enum {
        Root_Nothing = 0, Root_Ground, Root_Owner, Root_Infantry, Root_Vehicle,
        Root_Aircraft, Root_Building, Root_Terrain, Root_Smudge, Root_Overlay,
        Root_Waypoint, Root_Celltag, Root_Basenode, Root_Tunnel, Root_PlayerLocation,
        Root_Delete
    };

    enum
    {
        Const_Infantry = 10000, Const_Building = 20000, Const_Aircraft = 30000,
        Const_Vehicle = 40000, Const_Terrain = 50000, Const_Overlay = 63000,
        Const_House = 70000, Const_Smudge = 80000
    };

	using mpTreeNode = std::unordered_map<int, HTREEITEM>;

    static mpTreeNode ExtNodes;
    static std::unordered_set<std::string> IgnoreSet;
    static std::unordered_set<std::string> ExtSets[static_cast<int>(TreeViewTechnoType::Count)];
    static std::unordered_map<std::string, int> KnownItem;
    static std::unordered_map<std::string, int> Owners;
    HTREEITEM InsertString(const char* pString, DWORD dwItemData = 0, 
        HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
    HTREEITEM InsertTranslatedString(const char* pOriginString, DWORD dwItemData = 0,
        HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
    void Redraw_Initialize();
    void Redraw_MainList();
    void Redraw_Ground();
    void Redraw_Owner();
    void Redraw_Infantry();
    void Redraw_Vehicle();
    void Redraw_Aircraft();
    void Redraw_Building();
    void Redraw_Terrain();
    void Redraw_Smudge();
    void Redraw_Overlay();
    void Redraw_Waypoint();
    void Redraw_Celltag();
    void Redraw_Basenode();
    void Redraw_Tunnel();
    void Redraw_PlayerLocation();
public:
	void Redraw();
    int UpdateEngine(int nData);
private:
	void insertItemBySides(TreeViewTechnoType type, mpTreeNode& node, HTREEITEM& item);

public:
    /// <summary>
    /// Guess which type does the item belongs to.
    /// </summary>
    /// <param name="pRegName">Reg name of the given item.</param>
    /// <returns>
    /// The index of type guessed. -1 if cannot be guessed.
    /// 0 = Building, 1 = Infantry, 2 = Vehicle, 3 = Aircraft
    /// </returns>
    static TreeViewTechnoType GuessType(const char* pRegName);
    /// <summary>
    /// Guess which side does the item belongs to.
    /// </summary>
    /// <param name="pRegName">Reg name of the given item.</param>
    /// <param name="nType">
    /// Which type does this item belongs to.
    /// 0 = Building, 1 = Infantry, 2 = Vehicle, 3 = Aircraft
    /// </param>
    /// <returns>The index of side guessed. -1 if cannot be guessed.</returns>
    static int GuessSide(const char* pRegName, TreeViewTechnoType nType);
    /// <summary>
    /// Guess which side does the item belongs to.
    /// </summary>
    /// <param name="pRegName">Reg name of the given item.</param>
    /// <returns>The index of side guessed. -1 if cannot be guessed.</returns>
    static int GuessBuildingSide(const char* pRegName);
    /// <summary>
    /// Guess which side does the item belongs to.
    /// </summary>
    /// <param name="pRegName">Reg name of the given item.</param>
    /// <param name="nType">
    /// Which type does this item belongs to.
    /// 0 = Building, 1 = Infantry, 2 = Vehicle, 3 = Aircraft
    /// </param>
    /// <returns>The index of side guessed. -1 if cannot be guessed.</returns>
    static int GuessGenericSide(const char* pRegName, TreeViewTechnoType nType);
};
