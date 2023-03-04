#pragma once
#include <CMapData.h>
#include <vector>
#include <memory>

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
public:
    void InitializeBuildingTypesExt(const char* ID);

    static std::vector<BuildingDataExt> BuildingDataExts;//building extra data valt
    static int BuildingIndex;
};