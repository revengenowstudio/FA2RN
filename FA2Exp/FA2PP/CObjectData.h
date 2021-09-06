#pragma once

#include <MFC/FA2CString.h>

enum class ObjectType
{
    Infantry = 0,
    Structure = 1,
    Aircraft = 2,
    Unit = 3,
    Overlay = 4,
    Celltag = 5,
    Waypoints = 6
};

class CInfantryData
{
public:
    FA2::CString House;
    FA2::CString TypeID;
    FA2::CString Health;
    FA2::CString X;
    FA2::CString Y;
    FA2::CString SubCell;
    FA2::CString Status;
    FA2::CString Facing;
    FA2::CString Tag;
    FA2::CString VeterancyPercentage;
    FA2::CString Group;
    FA2::CString IsAboveGround;
    FA2::CString AutoNORecruitType;
    FA2::CString AutoYESRecruitType;
    int Flag;
};

class CStructureData
{
    FA2::CString House;
    FA2::CString TypeID;
    FA2::CString Health;
    FA2::CString X;
    FA2::CString Y;
    FA2::CString Facing;
    FA2::CString Tag;
    FA2::CString AISellable;
    FA2::CString AIRebuildable;
    FA2::CString PoweredOn;
    FA2::CString Upgrades;
    FA2::CString SpotLight;
    FA2::CString Upgrade1;
    FA2::CString Upgrade2;
    FA2::CString Upgrade3;
    FA2::CString AIRepairable;
    FA2::CString Nominal;
    int Flag;
};

class CUnitData
{
public:
    FA2::CString House;
    FA2::CString TypeID;
    FA2::CString Health;
    FA2::CString X;
    FA2::CString Y;
    FA2::CString Facing;
    FA2::CString Status;
    FA2::CString Tag;
    FA2::CString VeterancyPercentage;
    FA2::CString Group;
    FA2::CString IsAboveGround;
    FA2::CString FollowsIndex;
    FA2::CString AutoNORecruitType;
    FA2::CString AutoYESRecruitType;
    int Flag;
};

class CAircraftData
{
public:
    FA2::CString House;
    FA2::CString TypeID;
    FA2::CString Health;
    FA2::CString X;
    FA2::CString Y;
    FA2::CString Facing;
    FA2::CString Status;
    FA2::CString Tag;
    FA2::CString VeterancyPercentage;
    FA2::CString Group;
    FA2::CString AutoNORecruitType;
    FA2::CString AutoYESRecruitType;
    int Flag;
};

class CTerrainData
{
public:
    FA2::CString TypeID;
    FA2::CString X; // maybe
    FA2::CString Y; // maybe
    int Flag;
};