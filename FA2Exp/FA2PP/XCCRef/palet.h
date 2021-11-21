#if !defined(AFX_PALET_H__2BA979E0_160D_11D4_B605_0000B4936994__INCLUDED_)
#define AFX_PALET_H__2BA979E0_160D_11D4_B605_0000B4936994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vartypes.h"

#pragma pack(push, 1)

struct t_palet_entry
{
	byte r, g, b;
};

struct t_palet24rgb_entry
{
	byte r, g, b;
};

struct t_palet24bgr_entry
{
	byte b, g, r;
};

union t_palet32entry
{
	struct
	{
		byte r, g, b, a;
	};
	unsigned __int32 v;
};

union t_palet32bgr_entry
{
	struct
	{
		byte b, g, r, a;
	};
	unsigned __int32 v;
};

typedef t_palet_entry t_paletentry;
typedef t_palet_entry t_palet[256];

void apply_rp(byte* d, int cb_d, const byte* rp);
void convert_image_8_to_24(const byte* s, byte* d, int cx, int cy, const t_palet palet);
void convert_image_24_to_8(const byte* s, byte* d, int cx, int cy, const byte* rp);
void convert_image_24_to_8(const byte* s, byte* d, int cx, int cy, const t_palet palet);
//void convert_palet_18_to_24(const t_palet s, t_palet d) { JMP_STD(0x52D370) }
//void convert_palet_18_to_24(t_palet palet) { JMP_STD(0x52D400) }
void convert_palet_24_to_18(const t_palet s, t_palet d);
void convert_palet_24_to_18(t_palet palet);
void create_downsample_table(const t_palet palet, byte* rp);
void create_rp(const t_palet s1, const t_palet s2, byte* d);
int find_color(int r, int g, int b, const t_palet p);
int find_color_t(int r, int g, int b, const t_palet p);

#pragma pack(pop)

#endif // !defined(AFX_PALET_H__2BA979E0_160D_11D4_B605_0000B4936994__INCLUDED_)