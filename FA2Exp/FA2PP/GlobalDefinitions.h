#pragma once

using DWORD = unsigned long;

static constexpr int AFXMessageTypeSize = sizeof DWORD;
static constexpr int AFXMessageParamSize = sizeof DWORD;
static constexpr int AFXMessageFuncPtrSize = sizeof DWORD;

#define AFX_MSG_STRUCT(x) x
#define VIRTUAL_TABLE_FUNC(x) x
#define AFX_MSG_FUNC(x) x