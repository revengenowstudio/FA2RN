#pragma once

#include <ASMMacros.h>

class FAStringAImpl;

class FAStringPtr {
public:
	FAStringPtr() = default;
	FAStringPtr(char* ptr) : ptr(ptr) {}

	operator FAStringAImpl* () {
		return ptr ? reinterpret_cast<FAStringAImpl*>(ptr - 12) : nullptr;
	}

	operator const char* const () {
		return ptr;
	}

	char* ptr{ nullptr };
};

class FAStringAImpl
{
public:
	FAStringAImpl() = default;

	operator FAStringPtr () {
		return FAStringPtr(&Text);
	}

	int RefCount{ 1 };
	int Size{ 1 };
	int MaxSize{ 1 };
	char Text{ '\0' };
};
//old CString
class FAString
{
public:
	FAString() : pImpl(*(char**)0x5D4498) {}

	FAString(const char* text) {
		JMP_THIS(0x555F7D);
	}

	FAString(const wchar_t* text) {
		JMP_THIS(0x555FCF); 
	}

	FAString(const FAString& other) {
		*this = other;
	}

	FAString& operator=(const FAString& other) {
		JMP_THIS(0x555C84);
	}

	FAString& operator=(const char* other) {
		JMP_THIS(0x556098);
	}

	~FAString() {
		JMP_THIS(0x555F0F);
	}

	char const* c_str() const noexcept {
		return pImpl.ptr;
	}

	operator char const* const& () const noexcept {
		return pImpl.ptr;
	}

//private:
	FAStringPtr pImpl;
};
