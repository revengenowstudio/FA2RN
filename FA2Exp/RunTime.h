#pragma once

class RunTime
{
public:
	using ptr_type = unsigned long;

#pragma pack(push, 1)
	struct JumpOperation
	{
		JumpOperation(ptr_type from, ptr_type to)
			: OpCode{ 0xE9 }
			, Offset(to - from - 5)
		{}
		unsigned char OpCode;
		ptr_type Offset;
	};
#pragma pack(pop)

	static void ResetMemoryContentAt(ptr_type addr, const void* content, size_t size, size_t offset = 0);

	template<typename Func>
	static void ReplaceFunction(ptr_type targetAddr, Func funcAddr)
	{
		ResetMemoryContentAt(targetAddr, &funcAddr, sizeof(Func));
	}

	RunTime() = delete;
};

