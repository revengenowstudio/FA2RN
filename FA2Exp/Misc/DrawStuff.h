#pragma once

#include "../FA2Expand.h"

#include <MFC/FA2CString.h>

class DrawStuff
{
public:
    static void init();
    static void uninit();
    static bool load_vpl(const FA2::CString& name);
    static bool load_vxl(const FA2::CString& name);
    static bool load_hva(const FA2::CString& name);
    static bool get_to_image(unsigned int nFacing, unsigned char*& pBuffer, 
        int& width, int& height, int& x, int& y, const int F = 0, const int L = 0, const int H = 0);
    static bool is_vpl_loaded();
};