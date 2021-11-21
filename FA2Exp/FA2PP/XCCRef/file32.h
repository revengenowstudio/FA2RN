// file32.h: interface for the Cfile32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILE32_H__DB898AD3_D338_11D3_867E_0000F81AF605__INCLUDED_)
#define AFX_FILE32_H__DB898AD3_D338_11D3_867E_0000F81AF605__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>
#include <windows.h>

#include "vartypes.h"

class Cfile32
{
public:
    HANDLE handle();
    void attach(HANDLE handle);
    void detach();
    int open(const std::string& name, dword access);
    int open(const std::string& name, dword access, dword creation, dword share);
    int open_read(const std::string& name);
    int open_edit(const std::string& name);
    int open_write(const std::string& name);
    int get_size() const;
    FILETIME get_creation_time() const;
    FILETIME get_last_access_time() const;
    FILETIME get_last_write_time() const;
    int read(void* data, int size);
    int read_line(std::string& s);
    int write(const void* data, int size);
    int write(int v);
    int write(const std::string& s);
    int set_eof();
    void close();
    Cfile32();
    virtual ~Cfile32();

    bool eof() const
    {
        return m_p >= get_size();
    }

    bool is_open() const
    {
        return m_is_open;
    }

    int get_p() const
    {
        return m_p;
    }

    void seek(int p)
    {
        m_p = p;
    }

    void skip(int p)
    {
        m_p += p;
    }
private:
    HANDLE m_handle;
    bool m_is_open;
    int m_p;
};

int file32_write(const std::string& name, const void* s, int cb_s);

#endif // !defined(AFX_FILE32_H__DB898AD3_D338_11D3_867E_0000F81AF605__INCLUDED_)