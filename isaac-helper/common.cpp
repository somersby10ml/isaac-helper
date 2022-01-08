#include "common.h"
#include "screen.h"

CApp* CApp::instance = 0;
CApp* CApp::getInstance() {

	if (!instance) {
		instance = new CApp();
	}
	return instance;

}


DWORD WINAPI UnloadThread(PVOID pvParam) {

	CApp* App = CApp::getInstance();
	App->hook.unhookAll();
	ScreenUnload();

	HMODULE hMod = App->g_hModule;
	delete App;

#ifdef DEBUG_MODE
	ConsoleUnInstall();
#endif

	FreeLibraryAndExitThread(hMod, 0);
}


void WINAPI Unload() {
	CloseHandle(MyCreateThread(UnloadThread));
}