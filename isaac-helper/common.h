#pragma once
#pragma execution_character_set( "utf-8" )

#ifndef __MY_COMMON__
#define __MY_COMMON__

#include "debug.h"
#include "util.h"
#include "hook_control.h"

#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <string>

class CApp {
private:
	static CApp* instance;

public:
	static CApp* getInstance();

	DWORD dwBaseAddress;
	HMODULE g_hModule;
	hookControl hook;

	std::string DllPath;
	std::string DllDirectory;
	std::string logPath;
	std::string iniPath;
	CLog Log;
};

void WINAPI Unload();

#endif // __MY_COMMON__