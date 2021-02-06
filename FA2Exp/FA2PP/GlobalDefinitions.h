#pragma once

using DWORD = unsigned long;

static constexpr int AFXMessageTypeSize = sizeof DWORD;

#define AFX_MSG_STRUCT(x) x
#define VIRTUAL_TABLE_FUNC(x) x