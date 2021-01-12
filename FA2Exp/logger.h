#ifndef __FA2COPY_LOGGER__
#define __FA2COPY_LOGGER__

#include <fstream>
#include <string>

class logger
{
public:
	
	static inline void info(const std::string& message) { return g_logger.Info(message); }
	static inline void warn(const std::string& message) { return g_logger.Warn(message); }
	static inline void error(const std::string& message) { return g_logger.Error(message); }

	static logger g_logger;

	logger();
	logger(const std::string& Name);
	void Warn(const std::string& Reason);
	void Info(const std::string& Reason);
	void Error(const std::string& Reason);
	void Custom(const std::string& Prefix, const std::string& Reason, bool ShowTime);

private:
	std::string CurrentTime();
	std::ofstream fout;
	std::string DebugName;
};

#endif