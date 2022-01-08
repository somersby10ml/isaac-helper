#pragma once

#include "debug.h"
#include "common.h"
#include <Windows.h>

#pragma warning( push, 0 )

// stb
#include "stb_image.h"

// glew
#include <GL/glew.h>

// glfw
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// imgui
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_win32.h"


#ifdef DEBUG_MODE
//#pragma comment(lib, "glew32D.lib")
#pragma comment(lib, "libglew32D.lib")
#pragma comment(lib, "glfw3D.lib")

#else
//#pragma comment(lib, "glew32R.lib")
#pragma comment(lib, "libglew32D.lib")
#pragma comment(lib, "glfw3R.lib")
#endif

// opengl32
#pragma comment(lib, "opengl32.lib")


#pragma warning( pop ) 



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MyWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
void MyglSwapBuffers(HDC hDc);

void mapItemMenu();
void ScreenUnload();

