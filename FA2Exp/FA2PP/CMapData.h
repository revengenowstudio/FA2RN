#pragma once

#include <INI.h>
#include <GlobalVars.h>
#include <CObjectData.h>
#include <CPalette.h>

#include <Structure/FAVector.h>

class Direction
{
public:
    typedef unsigned int Value;
    enum
    {
        N = 0x0,
        North = 0x0,
        NE = 0x1,
        NorthEast = 0x1,
        E = 0x2,
        East = 0x2,
        SE = 0x3,
        SouthEast = 0x3,
        S = 0x4,
        South = 0x4,
        SW = 0x5,
        SouthWest = 0x5,
        W = 0x6,
        West = 0x6,
        NW = 0x7,
        NorthWest = 0x7,
        Count,
    };
};

struct CellData_BaseNodeData
{
    int BuildingID;
    int BasenodeID;
    FA2::CString House;
};

struct CellData
{
    short Unit;
    short Infantry[3];
    short Aircraft;
    short Structure;
    int TypeListIndex; // for BuildingType, it's -65536+{idx in list}, for TerrainType, it's some fucking number, or it's -1
    int TerrainType;
    short Smudge;
    int SmudgeType;
    short Waypoint;
    CellData_BaseNodeData BaseNode;
    unsigned char Overlay;
    unsigned char OverlayData; // [0, 59]
    short TileIndex;
    short Short_30;
    unsigned char TileSubIndex;
    unsigned char Height;
    unsigned char IceGrowth;
    short CellTag;
    short Tube;
    unsigned char TubeDataIndex;
    unsigned char StatusFlag;
    unsigned char LAT; // uses high 4 bit, see https://modenc.renegadeprojects.com/images/ConnectingLATSetSubTileSelection.png
};

struct MapCoord
{
    static const MapCoord Facings[Direction::Count];

    int X{};
    int Y{};
};

#pragma pack(push, 1)
struct TileStruct
{
    short X;
    short Y;
    int TileIndex;
    unsigned char TileSubIndex;
    unsigned char Level;
    unsigned char IceGrowth; // Unused in RA2/YR
};
#pragma pack(pop)

struct StructureData
{
    ColorStruct HouseColor;
    FA2::CString ID;
    short Y;
    short X;
    short Facing;
    unsigned char Strength;
    unsigned char PowerUpCount;
    FA2::CString PowerUp1;
    FA2::CString PowerUp2;
    FA2::CString PowerUp3;
};

struct TubeData
{
    __int16 Unknown_0;
    __int16 DestY;
    __int16 DestX;
    __int16 TubeDir;
    __int16 Unknown_8; // X related?
    char Data[102];
};

class NOVTABLE CMapData
{
public:
    static constexpr reference<CMapData, 0x72CBF8> const Instance{};

    void UpdateMapFieldData(bool bFlag) { JMP_THIS(0x49C280); }
    INIClass* UpdateCurrentDocument() { JMP_THIS(0x49C260); }
    static INIClass* GetMapDocument(bool bUpdateMapField = false)
    {
        if (bUpdateMapField) {
            GlobalVars::CMapData().UpdateMapFieldData(1);
        }
        return &GlobalVars::CMapData().INI;
    }

    const wchar_t* QueryUIName(const char* pRegName) { JMP_THIS(0x4B2610); }
    static FA2::CString GetUIName(const char* pRegName) { return FA2::CString(GlobalVars::CMapData->QueryUIName(pRegName)); }

    void LoadMap(const char* pMapPath) { JMP_THIS(0x49D2C0); }
    void UnpackData() { JMP_THIS(0x49EE50); } // called in LoadMap

    void InitializeBuildingTypes(const char* ID) { JMP_THIS(0x4B5460); } // use nullptr to reload all
    void UpdateTypeData() { JMP_THIS(0x4AD930); }
    void UpdateMapPreviewAt(int X, int Y) { JMP_THIS(0x4A23A0); }

    int GetFixedObjectTypeIndex(const char* ID) { JMP_THIS(0x4AE910); }// was GetBuildingTypeID
    inline CellData* GetCellAt(int nIndex) { return &this->CellData[nIndex]; }
    inline CellData* GetCellAt(int X, int Y) { return this->GetCellAt(this->GetCoordIndex(X, Y)); }

    // FA2 magics
    int GetCoordIndex(int X, int Y) { return Y + X * MapWidthPlusHeight; }  
    int GetXFromCoordIndex(int CoordIndex) { return CoordIndex / MapWidthPlusHeight; }
    int GetYFromCoordIndex(int CoordIndex) { return CoordIndex % MapWidthPlusHeight; }

    void GetStructureData(int structureID, StructureData* pRet) { JMP_THIS(0x4C3C20); }
    TubeData* GetTubeData(int tubeID) { JMP_THIS(0x4753C0); }

    FA2::CString StringBuffer;
    BOOL Initialized; // Maybe? It's data related, if this is false, UnitData, StructureData and so on will be called for loading?
    int MapWidthPlusHeight;
    CellData TempCellData;
    BOOL FieldDataAllocated;
    std::FAMap<FA2::CString, int, 0x5D8CD0, 0x5D8CCC> BuildingTypes;
    std::FAMap<FA2::CString, int, 0x5D8CD0, 0x5D8CCC> TerrainTypes;
    std::FAMap<FA2::CString, int, 0x5D8CD0, 0x5D8CCC> SmudgeTypes;
    unsigned char Overlay[0x40000];
    unsigned char OverlayData[0x40000];
    TileStruct* IsoPackData;
    int IsoPackDataCount;
    INIClass INI;
    RECT Size;
    RECT LocalSize;
    CellData* CellData; // see 4BB920 validate the map, dtor at 416FC0
    int CellDataCount; // see 4BB920 validate the map
    void* UndoRedoData;
    int UndoRedoDataCount; // undo redo count related
    int UndoRedoCurrentDataIndex; // undo redo count related, UndoRedoDataCount - 1
    int MoneyCount;
    FAVector<StructureData> StructureData; // being used in 4C3C20
    FAVector<TubeData> TubeData; // see 4753C0, might be TUBE related
    FAVector<int> vector_801F8;
    FAVector<CTerrainData> TerrainData;
    FAVector<CInfantryData> InfantryData;
    FAVector<CUnitData> UnitDatas; // Seems never used except DTOR
    FAVector<int> vector_80238;    // Seems never used except DTOR
    unsigned char MapPreview[0x40000];
    BITMAPINFO MapPreviewInfo;
    int nSomeMapPreviewData_C0274;
    INIClass SomeTheaterINI; //maybe?
    int Unknown_C0380;
    int Unknown_C0384;
    int Unknown_C0388;
    int Unknown_C038C;
};
