#define STB_IMAGE_IMPLEMENTATION

#include "screen.h"
#include "isaac_data.h"
#include "util.h"

HWND hGameWindow;
WNDPROC hGameWindowProc;

bool menuShown = true;
bool imGuiInitialized = false;
bool menuFirstShow = false;

// https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height) {
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

LRESULT CALLBACK MyWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_KEYDOWN && wParam == 'H') {
		menuShown = !menuShown;
		return true;
	}

	if (uMsg == WM_KEYDOWN && wParam == VK_END) {
		Unload();
		return true;
	}

	if (menuShown) {
		if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;
	}


	return CallWindowProc(hGameWindowProc, hWnd, uMsg, wParam, lParam);
}

void ImguiInit(HDC hDc) {

	hGameWindow = WindowFromDC(hDc);
	hGameWindowProc = (WNDPROC)SetWindowLongPtr(hGameWindow,
		GWLP_WNDPROC, (LONG_PTR)MyWinProc);

	glewInit();

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hGameWindow);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	ImGui::GetStyle().AntiAliasedFill = false;
	ImGui::GetStyle().AntiAliasedLines = false;
	ImGui::GetIO().IniFilename = nullptr;

	ImGui::GetIO().Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\malgun.ttf)", 18, NULL,
			ImGui::GetIO().Fonts->GetGlyphRangesKorean());


	ImGui::CaptureMouseFromApp();
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
}

void MyglSwapBuffers(HDC hDc) {

	if (!imGuiInitialized) {
		imGuiInitialized = true;
		ImguiInit(hDc);
	}

	RECT rect;
	GetWindowRect(hGameWindow, &rect);
	DWORD width = rect.right - rect.left;
	DWORD height = rect.bottom - rect.top;

	if (!menuShown)
		return;


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!menuFirstShow) {
		menuFirstShow = true;
		ImGui::SetNextWindowPos(ImVec2(width - 350.0f - 10.0f, 0));
		ImGui::SetNextWindowSize(ImVec2(350, 500));
	}
	ImGui::SetNextWindowBgAlpha(0.5f);


	ImGui::Begin("Helper");

	if (ImGui::TreeNode("map-item")) {
		ImGui::PushItemWidth(150);

		mapItemMenu();

		ImGui::PopItemWidth();
		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void mapItemMenu() {
	if (GAME::pItemArray == nullptr)
		return;

	std::unordered_map<int, card>::iterator cardIter;
	std::unordered_map<int, pill>::iterator pillIter;
	std::unordered_map<int, item>::iterator itemIter;
	std::unordered_map<int, trinkets>::iterator trinketsIter;

	for (DWORD i = 0; i < GAME::MapItemCount; i++) {
		if (GAME::pItemArray[i]->type2 != 5) continue;

		std::string itemTypeName = "";
		std::string title, description;
		bool isActive = false;
		std::string image_fullpath;

		switch (GAME::pItemArray[i]->type3) {

			case 100: // 아이템
				itemTypeName = "아이템";
				image_fullpath = path_join(GAME::imagePath, "collectibles\\%d.png");
				image_fullpath = string_format(image_fullpath, GAME::pItemArray[i]->itemID);
				itemIter = GAME::itemMap.find(GAME::pItemArray[i]->itemID);
				if (itemIter != GAME::itemMap.end()) {
					title = itemIter->second.title;
					description = itemIter->second.description;
					isActive = itemIter->second.isActiveItem;
				}
				break;

			case 350: // 장신구
				itemTypeName = "장신구";
				image_fullpath = path_join(GAME::imagePath, "trinkets\\%d.png");
				image_fullpath = string_format(image_fullpath, GAME::pItemArray[i]->itemID);
				trinketsIter = GAME::trinketsMap.find(GAME::pItemArray[i]->itemID);
				if (trinketsIter != GAME::trinketsMap.end()) {
					title = trinketsIter->second.title;
					description = trinketsIter->second.description;
				}
				break;
			case 70: // 알약
				itemTypeName = "알약";
				pillIter = GAME::pillMap.find(GetPillItem(GAME::pItemArray[i]->itemID));
				if (pillIter != GAME::pillMap.end()) {
					title = pillIter->second.title;
					description = pillIter->second.description;
				}
				break;
			case 300: // 카드
				itemTypeName = "카드";
				cardIter = GAME::cardMap.find(GAME::pItemArray[i]->itemID);
				if (cardIter != GAME::cardMap.end()) {
					title = cardIter->second.title;
					description = cardIter->second.description;
				}
				break;
		}

		if (itemTypeName.empty())
			continue;

		if (ImGui::TreeNode(GAME::pItemArray[i], "[%s] %d", itemTypeName.c_str(), GAME::pItemArray[i]->itemID)) {
			if (file_exist(image_fullpath)) {
				int my_image_width = 0, my_image_height = 0;
				GLuint my_image_texture = 0;
				bool ret = LoadTextureFromFile(image_fullpath.c_str(),
					&my_image_texture, &my_image_width, &my_image_height);
				ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width * 2.0f, my_image_height * 2.0f));
			}
			if (title.size() && description.size()) {
				if (isActive)
					ImGui::TextWrapped("[Active Item]");
				ImGui::TextWrapped(title.c_str());
				ImGui::TextWrapped(description.c_str());
			}
			ImGui::TreePop();
		}
	};
}

void ScreenUnload() {
	if (imGuiInitialized) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		hGameWindowProc = (WNDPROC)SetWindowLongPtr(hGameWindow,
			GWLP_WNDPROC, (LONG_PTR)hGameWindowProc);

		glfwTerminate();
	}
}