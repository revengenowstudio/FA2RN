#pragma once

#include <FA2PP.h>
#include <Helper/CompileTime.h>

struct BGRStruct 
{ 
    unsigned char B, G, R, Zero;
    bool operator< (const BGRStruct& rhs) const { return *(int*)this < *(int*)&rhs; }
    bool operator==(const BGRStruct& rhs) const { return *(int*)this == *(int*)&rhs; }
};

struct ColorStruct
{
    unsigned char red, green, blue;
};

class BytePalette
{
public:
    ColorStruct Data[256];

    ColorStruct& operator[](int index) { return Data[index]; }
};

class Palette
{
public:
    static constexpr constant_ptr<Palette, 0x72B4C4> PALETTE_UNIT{};
    static constexpr constant_ptr<Palette, 0x72B8C4> PALETTE_ISO{};
    static constexpr constant_ptr<Palette, 0x72ACC4> PALETTE_THEATER{};
    static constexpr constant_ptr<Palette, 0x72A8C4> PALETTE_LIB{};

    BGRStruct Data[256];

    BGRStruct& operator[](int index) { return Data[index]; }
    ColorStruct GetByteColor(int index) { // It's BGR color
        ColorStruct ret;
        BGRStruct& tmp = *(BGRStruct*)&Data[index];
        ret.red = tmp.R;
        ret.green = tmp.G;
        ret.blue = tmp.B;
        return ret;
    }
};