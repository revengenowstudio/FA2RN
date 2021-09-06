#pragma once

#include "../FA2Expand.h"

#include <CPalette.h>

#include <map>

class Palettes
{
public:
    static void Init();
    static Palette* LoadPalette(const FA2::CString& palName);
    static Palette* GetRemap(const FA2::CString& palName, BGRStruct color);
    static Palette* GetRemap(Palette* pal, BGRStruct color);
    static void Clear();

private:
    static std::map<FA2::CString, Palette*> LoadedPalettes;
    static std::map<Palette*, std::map<BGRStruct, Palette>> RemappedPalettes;
};