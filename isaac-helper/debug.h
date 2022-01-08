#pragma once
#include <windows.h>
#include <stdio.h>

#include <string>

#if defined(DEBUG) || defined(_DEBUG)
#define DEBUG_MODE
#endif

class CLog {
public:
	enum class Log {
		LOG_DEBUG = 1,
		LOG_INFO = 2,
		LOG_WARN = 3,
		LOG_ERROR = 4,
		LOG_FATAL = 5,
	};

	void SetLogFile(std::string logFile);
	void Write(const char* str, ...);
private:
	std::string _logFile;
};

void myOutputDebugString(LPCSTR pszStr, ...);
void ConsoleInstall();
void ConsoleUnInstall();
