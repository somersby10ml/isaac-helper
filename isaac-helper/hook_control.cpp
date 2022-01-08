#include "hook_control.h"
#include <cstdarg>

void* hookControl::GetAddress(const char* libName, const char* funName) {
	HMODULE hMod = GetModuleHandleA(libName);
	if (!hMod)
		hMod = LoadLibraryA(libName);

	if (!hMod) {
		return nullptr;
	}

	return GetProcAddress(hMod, funName);
}

hookObject hookControl::addHot(void* orgAddress, std::string asmString, int num, ...) {
	const int hotPatch_size = 2;

	std::vector<uint8_t>orgBytes(hotPatch_size, 0);
	memcpy(orgBytes.data(), orgAddress, hotPatch_size);

	// vsprintf( szData, pFormat, argptr );

	va_list ap;
	va_start(ap, num);
	for (int i = 0; i < num; i++) {
		int n = va_arg(ap, int);
		std::string bb = string_format("{%d}", i);
		std::string cc = string_format("0x%x", n);
		asmString = ReplaceAll(asmString, bb, cc);
	}
	va_end(ap);


	hookObject h;
	h.orgAddress = orgAddress;
	h.newAddress = nullptr;
	h.asmString = asmString;
	h.retAddress = addPtr(orgAddress, 2);
	h.orgBytes = orgBytes;
	h.type = HOOK_TYPE::HOT;
	hookList.push_back(h);

	return h;
}

hookObject hookControl::addJmp(void* orgAddress, std::string asmString, int size, int num, ...) {
	std::vector<uint8_t>orgBytes(size, 0);
	memcpy(orgBytes.data(), orgAddress, size);

	// vsprintf( szData, pFormat, argptr );

	va_list ap;
	va_start(ap, num);
	for (int i = 0; i < num; i++) {
		unsigned int n = va_arg(ap, unsigned int);
		std::string bb = string_format("{%d}", i);
		std::string cc = string_format("0x%x", n);
		asmString = ReplaceAll(asmString, bb, cc);
	}
	va_end(ap);


	hookObject h;
	h.orgAddress = orgAddress;
	h.newAddress = nullptr;
	h.asmString = asmString;
	h.retAddress = addPtr(orgAddress, size);
	h.orgBytes = orgBytes;
	h.type = HOOK_TYPE::JMP;
	hookList.push_back(h);

	return h;
}
