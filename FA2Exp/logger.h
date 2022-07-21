#ifndef __FA2COPY_LOGGER__
#define __FA2COPY_LOGGER__

#include <string>

#define LogWithLevel(level, format, ...) logger::g_logger.WriteLine(#level ## " (" __FUNCTION__ ") ", format, __VA_ARGS__);

#define LogDebug(Format, ...) LogWithLevel([Debug],  Format, __VA_ARGS__);
#define LogInfo(Format, ...) LogWithLevel([Info],  Format, __VA_ARGS__);
#define LogWarn(Format, ...) LogWithLevel([Warn],  Format, __VA_ARGS__);
#define LogError(Format, ...) LogWithLevel([Error],  Format, __VA_ARGS__);

class logger
{
public:
	static void Init(const std::string& path = "FA2Ext.log");

	static logger g_logger;

	logger() = default;
	logger(const std::string& Name);
	~logger();

	void WriteLine(const char* Prefix, const char* Format, ...);

private:
	void reopen();
	static std::string currentTime();
	static std::string prefixInfo();

	void writeLine(const char* Prefix, const char* Format, const va_list Args);
	void output(const char* pMessage, size_t Length);

	static constexpr auto defaultBufferLength = 0x800;

	FILE* pFile{ nullptr };
	std::string FileName{};
};

#endif