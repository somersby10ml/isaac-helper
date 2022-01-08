#pragma once

#ifndef __MY_UTIL__
#define __MY_UTIL__
#include <Windows.h>
#include <string>

HANDLE MyCreateThread(LPTHREAD_START_ROUTINE lpThread, LPVOID lpParameter = nullptr);

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args) {
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	//if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::string buf(size, 0);
	snprintf((char*)buf.data(), size, format.c_str(), args ...);
	return std::string(buf.c_str()); // We don't want the '\0' inside
}
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);


std::wstring string_to_unicode(std::string multiByte);
std::string unicode_to_string(std::wstring unicode);
std::string path_join(std::string path1, std::string path2);

bool file_exist(std::string& fileName);

#endif // __MY_UTIL__