#include "debug.h"
#include <cstdarg>
FILE* fp_in, * fp_out, * fp_err;
void ConsoleInstall() {
	AllocConsole();
	freopen_s(&fp_in, "CONIN$", "r", stdin);
	freopen_s(&fp_out, "CONOUT$", "w", stdout);
	freopen_s(&fp_err, "CONOUT$", "w", stdout);
}
void ConsoleUnInstall() {
	fclose(fp_err);
	fclose(fp_out);
	fclose(fp_in);
	FreeConsole();
}

void myOutputDebugString(LPCSTR pszStr, ...) {
	char szMsg[256];
	va_list args;
	va_start(args, pszStr);
	vsprintf_s(szMsg, 256, pszStr, args);
	OutputDebugString(szMsg);
}



void CLog::SetLogFile(std::string logFile) {
	_logFile = logFile;

	FILE* fLog = nullptr;
	fopen_s(&fLog, _logFile.c_str(), "w");
	if (fLog) {
		fclose(fLog);
	}
}


void CLog::Write(const char* str, ...) {
	char szMsg[500] = { 0, };
	va_list argptr;
	va_start(argptr, str);
	vsprintf_s(szMsg, _countof(szMsg), str, argptr);
	va_end(argptr);

#ifdef DEBUG_MODE
	printf(szMsg);
	printf("\n");
#endif

	FILE* logFile = nullptr;
	fopen_s(&logFile, _logFile.c_str(), "a");
	if (logFile) {
		fprintf(logFile, szMsg);
		fprintf(logFile, "\n");
		fclose(logFile);
	}

}
