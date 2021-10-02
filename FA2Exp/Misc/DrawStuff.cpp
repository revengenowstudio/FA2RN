#include "DrawStuff.h"

#include <GlobalVars.h>
#include <CLoading.h>

#include "../RSRef/vxl_drawing_lib.h"

void DrawStuff::init()
{
    vxl_drawing_lib::init();
}

void DrawStuff::uninit()
{
    vxl_drawing_lib::deinit();
}

bool DrawStuff::load_vpl(FA2::CString name)
{
    bool result = false;
    DWORD dwSize;
    if (auto pBuffer = (unsigned char*)GlobalVars::Dialog::CLoading->ReadWholeFile(name, &dwSize))
    {
        result = vxl_drawing_lib::load_vpl(pBuffer);
        FADeleteArray(pBuffer, dwSize);
    }
    return result;
}

bool DrawStuff::load_vxl(FA2::CString name)
{
    bool result = false;
    DWORD dwSize;
    if (auto pBuffer = (unsigned char*)GlobalVars::Dialog::CLoading->ReadWholeFile(name, &dwSize))
    {
        if (vxl_drawing_lib::is_loaded())
            vxl_drawing_lib::clear();
        result = vxl_drawing_lib::load_vxl(pBuffer);
        FADeleteArray(pBuffer, dwSize);
    }
    return result;
}

bool DrawStuff::load_hva(FA2::CString name)
{
    bool result = false;
    DWORD dwSize;
    if (auto pBuffer = (unsigned char*)GlobalVars::Dialog::CLoading->ReadWholeFile(name, &dwSize))
    {
        result = vxl_drawing_lib::load_hva(pBuffer);
        FADeleteArray(pBuffer, dwSize);
    }
    return result;
}

bool DrawStuff::get_to_image(unsigned int nFacing, unsigned char*& pBuffer, int& width,
    int& height, int& x, int& y, const int F, const int L, const int H)
{
    unsigned int nIndex = 4 * nFacing;
    vxl_drawing_lib::prepare_cache(nIndex, F, L, H);
    vxl_drawing_lib::get_image_frame(nIndex, width, height, x, y);
    if (width < 0 || height < 0)
        return false;
    return vxl_drawing_lib::get_to_image(nIndex, pBuffer);
}

bool DrawStuff::is_vpl_loaded()
{
    return vxl_drawing_lib::is_vpl_loaded();
}
