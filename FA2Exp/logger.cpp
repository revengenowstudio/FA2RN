#include "logger.h"
#include <FA2PP.h>
#include <mutex>

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

void logger::Debug(const std::string& Reason)
{
	WriteLine("[Debug] ", Reason.c_str());
}

void logger::Warn(const std::string& Reason)
{
	WriteLine("[Warn] ", Reason.c_str());
}

void logger::Info(const std::string& Reason)
{
	WriteLine("[Info] ", Reason.c_str());
}

void logger::Error(const std::string& Reason)
{
	WriteLine("[Error] ", Reason.c_str());
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
	auto const& time = currentTime();
	auto constexpr extraLen = 1;//2;//1 for 0 end, 1 for \n
	const size_t prefixCategoryLen = strlen(Prefix);
	const size_t prefixTotalLen = time.size() + prefixCategoryLen;
	const size_t contentLen = vsnprintf(nullptr, 0, Format, Args) + prefixTotalLen;
	const size_t bufferWantedLen = contentLen + extraLen;
	bool dynamicBufferRequired = bufferWantedLen > defaultBufferLength;
	char* pBuffer = this->Buffer;
	if (dynamicBufferRequired) {
		pBuffer = new char[bufferWantedLen];
	}
	//insert prefix first
	memcpy(pBuffer,					time.c_str(),	time.size());
	memcpy(pBuffer + time.size(),	Prefix,			prefixCategoryLen);
	//real write
	vsnprintf(pBuffer + prefixTotalLen, bufferWantedLen, Format, Args);
	pBuffer[contentLen] = '\n';
	//pBuffer[contentLen + 1] = '\0';

	output(pBuffer, bufferWantedLen);

	//deallocate resources
	if (dynamicBufferRequired && pBuffer) {
		delete[] pBuffer;
	}
}

std::string logger::currentTime() {
	SYSTEMTIME Sys;
	GetLocalTime(&Sys);
	std::string ret;
	ret = (std::to_string(Sys.wYear) + '/' + std::to_string(Sys.wMonth) + '/'
		+ std::to_string(Sys.wDay) + ' ' + std::to_string(Sys.wHour) + ':'
		+ std::to_string(Sys.wMinute) + ':' + std::to_string(Sys.wSecond)) + '.'
		+ std::to_string(Sys.wMilliseconds);
	return ret;
}