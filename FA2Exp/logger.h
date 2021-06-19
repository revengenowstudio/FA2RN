#ifndef __FA2COPY_LOGGER__
#define __FA2COPY_LOGGER__

#include <string>

#define LogDebug(Format, ...) \
	logger::g_logger.WriteLine("[Debug] ", Format, __VA_ARGS__);

class logger
{
public:
	static void Init(const std::string& path = "FA2Ext.log");

	static inline void debug(const std::string& message) { return g_logger.Debug(message); }
	static inline void info(const std::string& message) { return g_logger.Info(message); }
	static inline void warn(const std::string& message) { return g_logger.Warn(message); }
	static inline void error(const std::string& message) { return g_logger.Error(message); }

	static logger g_logger;

	logger() = default;
	logger(const std::string& Name);
	~logger();
	void Debug(const std::string& Reason);
	void Warn(const std::string& Reason);
	void Info(const std::string& Reason);
	void Error(const std::string& Reason);
	void WriteLine(const char* Prefix, const char* Format, ...);

private:
	void reopen();
	static std::string currentTime();

	void writeLine(const char* Prefix, const char* Format, const va_list Args);
	void output(const char* pMessage, size_t Length);

	static constexpr auto defaultBufferLength = 0x800;

	FILE* pFile{ nullptr };
	std::string FileName{};
	char Buffer[defaultBufferLength];
};

#endif