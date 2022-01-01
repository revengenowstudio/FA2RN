#pragma once

#include "../Syringe.h"

// here be dragons(plenty)


// return functors

// set EAX to smth, return to smth
template<typename T>
class retfunc {
protected:
	REGISTERS *R;
	DWORD retAddr;
public:
	retfunc(REGISTERS *r, DWORD addr) : R(r), retAddr(addr) {};
	DWORD operator()(T Result) {
		R->EAX(Result);
		return retAddr;
	}
};

// set EAX to smth, return to fixed
template<typename T>
class retfunc_fixed : public retfunc<T> {
protected:
	T Result;
public:
	retfunc_fixed(REGISTERS *r, DWORD addr, T res) : retfunc<T>(r, addr), Result(res) {};
	DWORD operator()() {
		this->R->EAX(Result);
		return this->retAddr;
	}
};

// return to one of two fixed
class retfunc_bool : public retfunc<int> {
protected:
	DWORD negAddr;
public:
	retfunc_bool(REGISTERS *r, DWORD yAddr, DWORD nAddr) : retfunc<int>(r, yAddr), negAddr(nAddr) {};
	DWORD operator()(bool choose) {
		return choose ? retAddr : negAddr;
	}
};
