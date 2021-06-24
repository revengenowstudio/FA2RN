#pragma once

#include <GlobalDefinitions.h>
#include "../FA2Expand.h"

//addr must be a AFX_MSG

class HackHelper
{
public:
	static inline void ResetMessageType(DWORD addr, int msg)
	{
		RunTime::ResetMemoryContentAt(addr + ::AFXMessageTypeSize, &msg, ::AFXMessageTypeSize);
	}

	//template<class MemFn, class D = MemFn>
	//struct FuncTraits
	//{
	//};
	///* Non-const member function pointers. */

	//template<class T, class R, class... ParamList>
	//struct FuncTraits<R(T::*)(ParamList...)>
	//{
	//	static bool const isMemberFunction = true;
	//	static bool const isConstMemberFunction = false;
	//	using DeclType = R(T::*)(ParamList...);
	//	using ClassType = T;
	//	using ReturnType = R;
	//	using Params = typename MakeTypeList<ParamList...>::Result;

	//	static R call(ClassType* obj, const DeclType& fp, TypeListValues<Params>& tvl)
	//	{
	//		return doCall<R>(obj, fp, tvl);
	//	}
	//};

	///* Const member function pointers. */

	//template<class T, class R, class... ParamList>
	//struct FuncTraits<R(T::*)(ParamList...) const>
	//{
	//	static bool const isMemberFunction = true;
	//	static bool const isConstMemberFunction = true;
	//	using DeclType = R(T::*)(ParamList...) const;
	//	using ClassType = T;
	//	using ReturnType = R;
	//	using Params = typename MakeTypeList<ParamList...>::Result;

	//	static R call(const ClassType* obj, const DeclType& fp, TypeListValues<Params>& tvl)
	//	{
	//		return doCall<R>(obj, fp, tvl);
	//	}
	//};

	template<typename Func>
	static inline void ResetMessageMemberFunction(DWORD addr, Func func)
	{
		RunTime::ResetMemoryContentAt(addr + sizeof(AFX_MSGMAP_ENTRY) - AFXMessageFuncPtrSize, &func, sizeof(Func));
	}

	template<typename Func>
	static inline void ResetVirtualMemberFunction(DWORD addr, Func func)
	{
		RunTime::ResetMemoryContentAt(addr, &func, sizeof(Func));
	}



};

