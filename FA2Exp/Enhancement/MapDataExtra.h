#pragma once
#include <CMapData.h>
#include <memory>
#include <unordered_map>

struct BuildingDataExt
{
    using FoundationStorage = std::vector<MapCoord>;
    using FoundationLineStorage = std::vector<std::pair<MapCoord, MapCoord>>;

    bool IsCustomFoundation() const
    {
        return Foundations != nullptr;
    }

    int Width{ 1 };
    int Height{ 1 };
    std::unique_ptr<FoundationStorage> Foundations{};
    std::unique_ptr<FoundationLineStorage> LinesToDraw{};
};

class CMapDataExt
{
    using BuildingDataMap = std::unordered_map<int, BuildingDataExt>;

public:
    void InitializeBuildingTypesExt(const char* ID);

    static BuildingDataMap BuildingDataExts;//building extra data valt
    static int BuildingIndex;
};