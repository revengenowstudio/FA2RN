#pragma once

#include <CommCtrl.h>

#include <FA2PPCore.h>

#ifndef FA2MFC_NAMESPACE
#define FA2MFC_NAMESPACE FA2
#endif // FA2MFC_NAMESPACE

#ifndef FA2MFC_ASSERT
#ifdef ASSERT
#define FA2MFC_ASSERT ASSERT
#else
#define FA2MFC_ASSERT(x) void(0)
#endif
#endif

#ifndef FA2MFC_CDECL
#define FA2MFC_CDECL __cdecl
#endif

#ifndef FA2MFC_BEGIN
#define FA2MFC_BEGIN namespace FA2MFC_NAMESPACE {
#endif // _FA2MFC_BEGIN

#ifndef FA2MFC_END
#define FA2MFC_END }
#endif // _FA2MFC_END

#ifndef FA2MFC_CLASS
#define FA2MFC_CLASS(clsname) class NOVTABLE clsname
#endif // FA2MFC_CLASS

#ifndef FA2MFC_STRUCT
#define FA2MFC_STRUCT(clsname) struct clsname
#endif // FA2MFC_STRUCT

#ifndef _PPMFC_INHERIT
#define _PPMFC_INHERIT(clsname) : public clsname
#endif // _PPMFC_INHERIT

#ifndef FA2MFC_THISCALL
#define FA2MFC_THISCALL(addr) { JMP_THIS(addr); }
#endif // FA2MFC_THISCALL

#ifndef FA2MFC_STDCALL
#define FA2MFC_STDCALL(addr) { JMP_STD(addr); }
#endif // FA2MFC_STDCALL

#ifndef FA2MFC_API
#define FA2MFC_API __stdcall
#endif // FA2MFC_API

FA2MFC_BEGIN

FA2MFC_CLASS(CObject)
{
private:
public:
	virtual ~CObject() RX;
};

FA2MFC_CLASS(CPoint) _PPMFC_INHERIT(tagPOINT)
{
public:
    CPoint(DWORD dwPoint) FA2MFC_THISCALL(0x5338A9);
};

FA2MFC_STRUCT(XDispatch)
{
	virtual ~XDispatch() RX;
};

FA2MFC_STRUCT(XConnPtContainer)
{
	virtual ~XConnPtContainer() RX;
};

FA2MFC_END