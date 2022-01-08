#include "util.h"

#include <pathcch.h>
#pragma comment(lib, "Pathcch.lib")

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

std::string path_join(std::string path1, std::string path2) {
	wchar_t Buffer[MAX_PATH] = { 0, };
	std::wstring path1_uni = string_to_unicode(path1);
	std::wstring path2_uni = string_to_unicode(path2);
	std::wstring path3(260, 0);
	PathCchCombine(Buffer, _countof(Buffer), path1_uni.c_str(), path2_uni.c_str());
	return unicode_to_string(Buffer);
}

bool file_exist(std::string& fileName) {
	return PathFileExists(fileName.c_str());
}



HANDLE MyCreateThread(LPTHREAD_START_ROUTINE lpThread, LPVOID lpParameter) {
	DWORD dwThreadID;
	HANDLE hThread = CreateThread(NULL, 0, lpThread, lpParameter, NULL, &dwThreadID);
	return hThread;
}

std::wstring string_to_unicode(std::string multiByte) {
	std::wstring strUnicode;
	int nLen = MultiByteToWideChar(CP_ACP, 0, multiByte.c_str(), multiByte.size(), NULL, NULL);
	strUnicode.resize(nLen);
	MultiByteToWideChar(CP_ACP, 0, multiByte.c_str(), multiByte.size(), (LPWSTR)strUnicode.data(), strUnicode.size());
	return strUnicode;
}

std::string unicode_to_string(std::wstring unicode) {
	std::string strMultiBytes;
	int nLen = WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), unicode.size(), nullptr, 0, nullptr, nullptr);
	strMultiBytes.resize(nLen);
	WideCharToMultiByte(CP_ACP, 0, unicode.c_str(), unicode.size(), (LPSTR)strMultiBytes.data(), strMultiBytes.size(), nullptr, nullptr);
	return strMultiBytes;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while (( start_pos = str.find(from, start_pos) ) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}