/*
	isaac-loader
	description: Inject it into isaac-ng.exe
*/

#define PROCESS_NAME "isaac-ng.exe"
#define DEFAULT_DLLNAME "isaac-helper.dll"

#include "common.h"

int _tmain(int argc, _TCHAR* argv[]) {

	setlocale(LC_ALL, "");

	tstring CurrentDirectory(260, 0);
	unsigned long dwLen = GetCurrentDirectory(CurrentDirectory.size(), (TCHAR*)CurrentDirectory.data());
	CurrentDirectory.resize(dwLen);

	tstring dllPath = ( argc == 1 ) ? _T(DEFAULT_DLLNAME) : argv[1];
	tstring dllFullPath = findDllFullPath(dllPath);

	_tprintf(_T("[info] dll: %Ts\n"), dllPath.c_str());
	_tprintf(_T("[info] dllFullPath: %Ts\n"), dllFullPath.c_str());

	if (!dllFullPath.size()) {
		_tprintf(_T("dll fullpath is not found\n"));
		return -1;
	}

	// find process
	unsigned long processID = getProcess(_T(PROCESS_NAME));
	_tprintf(_T("[info] isaac-ng.exe Process id %d\n"), processID);
	if (processID == -1) {
		_tprintf(_T("[error] %Ts not found\n"), _T(PROCESS_NAME));
		return -1;
	}

	DWORD error = injection(processID, dllFullPath);
	if (error == ERROR_SUCCESS) {
		_tprintf(_T("[info] Injection successful\n"));
		return ERROR_SUCCESS;
	}

	_tprintf(_T("[error] Message: %Ts\n"), getErrorString(error).c_str());
	return error;
}