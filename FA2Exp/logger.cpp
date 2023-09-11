#include "logger.h"
#include <FA2PP.h>
#include <mutex>
#include <sstream>
#include <iomanip>

logger logger::g_logger;//default initialization, do nothing, because syringe would also create such item
FILE* pFile;
std::mutex gMutex;

void logger::Init(const std::string& path)
{
	g_logger = logger(path);//create real meaningful instance when really loaded in host process rather than syringe
}

logger::logger(const std::string& FileName) :
	FileName(FileName)
{
	if (this->FileName.empty()) {
		this->FileName = "FA2Ext.log";
	}
}

logger::~logger()
{
	if (pFile) {
		fclose(pFile);
		pFile = nullptr;
	}
}

void logger::reopen()
{
	//std::lock_guard<decltype(gMutex)> locker(gMutex);
	if (!pFile) {
		pFile = _fsopen(this->FileName.c_str(), "w", _SH_DENYWR);
	}
}

void logger::output(const char* pMessage, size_t Length)
{
	reopen();
	if (pFile) {
		fwrite(pMessage, Length, 1, pFile);
		fflush(pFile);
	}
}

void logger::WriteLine(const char* Prefix, const char* Format, ...)
{
	va_list args;
	va_start(args, Format);
	writeLine(Prefix, Format, args);
	va_end(args);
}

void logger::writeLine(const char* Prefix, const char* Format, const va_list Args)
{
	char Buffer[defaultBufferLength];
	auto const& prefixInf = prefixInfo();
	auto constexpr extraLen = 1;//2;//1 for 0 end, 1 for \n
	const size_t prefixCategoryLen = strlen(Prefix);
	const size_t prefixTotalLen = prefixInf.size() + prefixCategoryLen;
	const size_t contentLen = vsnprintf(nullptr, 0, Format, Args) + prefixTotalLen;
	const size_t bufferWantedLen = contentLen + extraLen;
	bool dynamicBufferRequired = bufferWantedLen > defaultBufferLength;
	char* pBuffer = Buffer;
	if (dynamicBufferRequired) {
		pBuffer = new char[bufferWantedLen];
	}
	//insert prefix first
	memcpy(pBuffer,					prefixInf.c_str(),	prefixInf.size());
	memcpy(pBuffer + prefixInf.size(),	Prefix,			prefixCategoryLen);
	//real write
	vsnprintf(pBuffer + prefixTotalLen, bufferWantedLen, Format, Args);
	pBuffer[contentLen] = '\n';
	pBuffer[contentLen + 1] = '\0';

	output(pBuffer, bufferWantedLen);

	//deallocate resources
	if (pBuffer != Buffer) {
		delete[] pBuffer;
	}
}

void currentTime(std::stringstream& stream)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	using std::setw;
	using std::setfill;
	//auto const savedFmt(stream.flags());
	stream << sys.wYear << '-' << setw(2) << setfill('0') << sys.wMonth << "-" << setw(2) << setfill('0') << sys.wDay << ' '
		<< setw(2) << setfill('0') << sys.wHour << ':' << setw(2) << setfill('0') << sys.wMinute << ':' << setw(2) << setfill('0') << sys.wSecond << '.' << setw(3) << setfill('0') << sys.wMilliseconds
		;
	//stream.flags(savedFmt);
}

std::string logger::prefixInfo()
{
	std::stringstream ss;
	currentTime(ss);
	ss << ' ';
	ss << '(';
	ss << std::this_thread::get_id();
	ss << ')';
	return ss.str();
}

