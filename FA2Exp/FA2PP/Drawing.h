#pragma once

#include <FA2PP.h>
#include <Structure/FAMap.h>
#include <Helper/CompileTime.h>

#include <ddraw.h>

class Palette;
class ImageCacheFlag;

enum class ImageDataFlag : unsigned int
{
    SurfaceData = 0,
    SHP = 1,
    VXL = 2
};

class NOVTABLE ImageDataClass
{
public:
    ImageDataClass() { JMP_THIS(0x475380) }; 
    
    void Initialize() { JMP_THIS(0x475380); } // probably ctor
    void Set(ImageDataClass* another) { JMP_THIS(0x4DE4F0); }
    void Clear() { JMP_THIS(0x4DE510); }

    union
    {
        unsigned char* pImageBuffer; // draw from here, size = FullWidth*FullHeight
        LPDIRECTDRAWSURFACE7 lpSurface; // Only available for flag = 0, if this is used, only ValidWidth and ValidHeight are set
    };

    struct ValidRangeData
    {
        short First;
        short Last;
    } *pPixelValidRanges;
    // size = FullHeight, stores the valid pixel from where to begin and end for each row
    // If it's an invalid row, then first = FullWidth - 1, second = 0 

    Palette* pPalette;
    short ValidX; // negative value for vxl, dunno why
    short ValidY; // negative value for vxl, dunno why
    short ValidWidth; // same as full for vxl, dunno why
    short ValidHeight; // same as full for vxl, dunno why
    short FullWidth;
    short FullHeight;
    ImageDataFlag Flag;
    bool IsOverlay; // Only OVRLXX_XX will set this true
};

struct BuildingData
{
    static constexpr reference<BuildingData, 0x627930, 3840> const Array{};

    ImageDataFlag Facings[8];
    int FacingCount;
    unsigned __int8 Foundation_Width;
    unsigned __int8 Foundation_Height;
    int Unknown_108;
    int Unknown_10C;
    int Unknown_110;
};

using ImageDataMap = std::FAMap<FA2::CString, ImageDataClass, 0x5E7C18, 0>; // DrawDataMap& tmp = *reinterpret_cast<DrawDataMap*>(0x72CBC8);

class ImageDataMapHelper
{
private:
    ImageDataClass* GetImageDataFromMap_(const FA2::CString* ppName) { JMP_THIS(0x4768D0); }
public:
    static bool IsImageLoaded(const FA2::CString& name)
    {
        ImageDataMap& imageData = *reinterpret_cast<ImageDataMap*>(0x72CBC8);
        auto itr = imageData.find(name);
        if (itr == imageData.end()) {
            return false;
        }
        return itr->second.pImageBuffer != nullptr;
    }

    static ImageDataClass* GetImageDataFromMap(const FA2::CString& name)
    {
        ImageDataMap& imageData = *reinterpret_cast<ImageDataMap*>(0x72CBC8);
        return ((ImageDataMapHelper*)&imageData)->GetImageDataFromMap_(&name);
    }
};

class NOVTABLE ImageCacheFlag : public std::FAMap<FA2::CString, bool, 0x5D8CD0, 0>
{
public:
    static ImageCacheFlag& Global()
    {
        return *reinterpret_cast<ImageCacheFlag*>(0x72A870);
    }

    iterator find(const FA2::CString& name) { JMP_THIS(0x4767B0); }

    _Pairib insert(const FA2::CString& key, const bool& value)
    {
        _Pairib ret;
        insert(&ret, value_type{ key, value });
        return ret;
    }

    _Pairib* insert(_Pairib* ret, const value_type& value) { JMP_THIS(0x476BE0); }

    //static void SetSomeData(FA2::CString name, bool value)
    //{
    //    CStringBoolMap& someData = *reinterpret_cast<CStringBoolMap*>(0x72A870);
    //    *((SomeDataMapHelper*)&someData)[name] = value;
    //}
};

class HSVClass;
class RGBClass
{
public:
	unsigned char R, G, B;

	operator int()
	{
		return RGB(R, G, B);
	}

    operator HSVClass() const;
};

class HSVClass
{
public:
    unsigned char H, S, V;

    operator RGBClass() const;
};