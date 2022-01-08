#pragma once
#include "isaac_data.h"
#include "screen.h"
#include <windows.h>

LRESULT CALLBACK windowProc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __stdcall UserDataCallback(LPVOID data);
void __stdcall glSwapBuffers_hook(HDC hDc);
void __stdcall ItemDataCallback(ItemObject** itemArray, DWORD itemCount);


