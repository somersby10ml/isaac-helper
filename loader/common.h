#pragma once
#include <conio.h>

#include <string>
#include <iostream>
#include <locale.h>

#include <Windows.h>
#include <TlHelp32.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <tchar.h>
typedef std::basic_string<TCHAR> tstring;
struct handle_deleter {
	void operator()(HANDLE handle) {
		::CloseHandle(handle);
	}
	typedef HANDLE pointer;
};


unsigned long getProcess(tstring processName);
tstring findDllFullPath(tstring name);
tstring getErrorString(unsigned long error_code);
unsigned long injection(unsigned long pid, tstring dllPath);
