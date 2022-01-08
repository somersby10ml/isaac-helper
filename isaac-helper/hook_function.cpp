#pragma once
#include "hook_function.h"


LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return MyWinProc(hWnd, uMsg, wParam, lParam);
}


void _stdcall UserDataCallback(LPVOID data) {
	//printf("%X\n", data);
	//pPlayerData = (LPBYTE)data;
}



void _stdcall ItemDataCallback(ItemObject** itemArray, DWORD itemCount) {
	//pItemArray = itemArray;
	//MapItemCount = itemCount;
	GAME::pItemArray = itemArray;
	GAME::MapItemCount = itemCount;
}


void __stdcall glSwapBuffers_hook(HDC hDc) {
	MyglSwapBuffers(hDc);
}
