#include "common.h"
#include "debug.h"
#include "hook_control.h"
#include "hook_function.h"
#include "iniparser.hpp"
#include "isaac_data.h"




DWORD WINAPI ThreadFunc(PVOID hModule) {
	CApp* App = CApp::getInstance();

#ifdef DEBUG_MODE
	ConsoleInstall();
	SetConsoleOutputCP(65001); // UTF-8
#endif



	{
		char DllPath[260] = { 0, };
		DWORD dwLen = GetModuleFileName((HINSTANCE)hModule, DllPath, _countof(DllPath));
		App->DllPath = DllPath;
		App->DllDirectory = App->DllPath.substr(0, App->DllPath.find_last_of('\\') + 1);
		App->logPath = App->DllDirectory + "dll.log";
		App->iniPath = App->DllDirectory + "Setting.ini";
	}


	App->Log.SetLogFile(App->logPath);
	App->Log.Write("[INFO] Start!!!");
	App->Log.Write("[INFO] DllDirectory: %s", App->DllDirectory);
	App->Log.Write("[INFO] logPath: %s", App->logPath);
	App->Log.Write("[INFO] iniPath: %s", App->iniPath);
	std::string lang;

	// hook test
	// 83 B9 18 1B 00 00 00 89 4C 24 1C 89 74 24 2C C7 44 24 18 00 00 00 00
	uint8_t check1[] = { 0x83, 0xB9, 0x18, 0x1B, 0x00, 0x00, 0x00, 0x89, 0x4C, 0x24, 0x1C, 0x89, 0x74, 0x24, 0x2C, 0xC7, 0x44, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00 };
	if (memcmp((LPVOID)( App->dwBaseAddress + 0x2560F5 ), check1, _countof(check1)) != 0) {
		App->Log.Write("[FATAL] This version of isaac-ng.exe is not supported. type1");
		Unload();
		return -1;
	}

	// 53 8B 5D 08 57 8B 7D 0C 8D A4 24 00 00 00 00 8B 46 50 8B 8E 8C 00 00
	uint8_t check2[] = { 0x53, 0x8B, 0x5D, 0x08, 0x57, 0x8B, 0x7D, 0x0C, 0x8D, 0xA4, 0x24, 0x00, 0x00, 0x00, 0x00, 0x8B, 0x46, 0x50, 0x8B, 0x8E, 0x8C, 0x00, 0x00 };
	if (memcmp((LPVOID)( App->dwBaseAddress + 0x143F1 ), check2, _countof(check2)) != 0) {
		App->Log.Write("[FATAL] This version of isaac-ng.exe is not supported. type2");
		Unload();
		return -1;
	}


	INI::File ft;
	if (ft.Load(App->iniPath.c_str())) {
		std::string langValue = ft.GetValue("lang").AsString();
		if (langValue.empty()) {
			App->Log.Write("[WARN] Setting.ini 'lang' is not found");
			App->Log.Write("[WARN] No 'lang' found. Replaced by ko.");
			lang = "ko";
		}
		else {
			lang = langValue;
		}

		std::string lang_path = path_join(App->DllDirectory, "data\\text\\ko");
		std::string csv_path;
		if (file_exist(lang_path)) {
			LoadData(lang_path);
		}
		else {
			App->Log.Write("[WARN] lang directory not found %s", lang_path.c_str());
			App->Log.Write("[FATAL] lang is empty");
			lang = "";
		}
	}
	else {
		App->Log.Write("[WARN] Setting.ini is not found");
	}

	std::string image_path = path_join(App->DllDirectory, "data\\image");
	if (file_exist(image_path)) {
		GAME::imagePath = image_path;
	}
	else {
		App->Log.Write("[WARN] image not found %s", image_path.c_str());
	}

	HMODULE hOpengl32 = GetModuleHandle("opengl32.dll");
	if (hOpengl32 == 0) {
		hOpengl32 = LoadLibrary("opengl32.dll");
	}
	if (hOpengl32 == 0) {
		App->Log.Write("[FATAL] opengl32.dll is not found");
		Unload();
		return -1;
	}

	App->Log.Write("[INFO] hook");

	App->hook.addHot(App->hook.GetAddress("opengl32.dll", "wglSwapBuffers"),
		R"(
			pushad
			pushfd
			push dword ptr ds:[esp + 0x28]
			call {0}
			popfd
			popad
			@jmp
		)",
		1, glSwapBuffers_hook
	);


	App->hook.addJmp((LPVOID)( App->dwBaseAddress + 0x2560F5 ),
		R"(
			pushad
			pushfd
			push ecx
			call {0}
			popfd
			popad
			cmp dword ptr ds:[ecx+0x1B18], 0x0
			@jmp
		)",
		7,
		1, UserDataCallback
	);

	App->hook.addJmp((LPVOID)( App->dwBaseAddress + 0x143F1 ),
		R"(
			pushad
			pushfd
			push dword ptr ds:[esi+0x58]
			push dword ptr ds:[esi+0x50]
			call {0}
			popfd
			popad
			push ebx
			mov ebx, dword ptr ds:[ebp+0x8]
			push edi
			@jmp
		)",
		5,
		1, ItemDataCallback
	);

	App->hook.hookAll();

	return 0;
}




BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		{
			CApp* App = CApp::getInstance();
			App->dwBaseAddress = (DWORD)GetModuleHandle(nullptr);
			App->g_hModule = hModule;
			CloseHandle(MyCreateThread(ThreadFunc, hModule));
			break;
		}
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

