#pragma once

#ifndef __MY_HOOK__
#define __MY_HOOK__

#include "debug.h"
#include "util.h"

#pragma warning( push, 0 )
#pragma warning ( disable : 26495 )
#include <asmjit/asmjit.h>
#include <asmtk/asmtk.h>
#ifdef DEBUG_MODE
#pragma comment(lib, "asmjitD.lib")
#pragma comment(lib, "asmtkD.lib")
#else
#pragma comment(lib, "asmjitR.lib")
#pragma comment(lib, "asmtkR.lib")
#endif

#pragma warning( pop ) 
using namespace asmjit;
using namespace asmtk;

#include <Windows.h>
#include <vector>
#include <string>


enum class HOOK_TYPE : int {
	HOT,
	JMP,
};
class hookObject {
public:
	void* orgAddress;
	void* newAddress;
	void* retAddress;

	std::string asmString;

	HOOK_TYPE type;
	std::vector<uint8_t> orgBytes;

	bool result;
	void* hAlloc;
	DWORD oldProtect;
};

class hookControl {
public:

	void* GetAddress(const char* libName, const char* funName);

	hookObject addHot(void* orgAddress, std::string asmString, int num, ...);

	hookObject addJmp(void* orgAddress, std::string asmString, int size, int num, ...);

	void* addPtr(void* p1, void* p2) {
		return (void*)( (uint32_t)p1 + (uint32_t)p2 );
	}
	void* addPtr(void* p1, uint32_t p2) {
		return (void*)( (uint32_t)p1 + p2 );
	}

	template<typename T>
	std::vector<uint8_t> strToAsmBytes(std::string asmString, T baseAddr) {
		Environment environment;
		environment.init(Arch::kX86);

		CodeHolder code;
		Error error = code.init(environment, (DWORD)baseAddr);
		/*if (error) {
		*
			buildError = error;
			return false;
		}*/

		x86::Assembler a(&code);
		AsmParser p(&a);

		error = p.parse(asmString.c_str());
		/*if (error) {
			buildError = error;
			return false;
		}*/

		CodeBuffer& buffer = code.sectionById(0)->buffer();
		return std::vector<uint8_t>(buffer.data(), buffer.data() + buffer.size());
	}




	void hookAll() {
		for (hookObject h : hookList) {
			void* start = nullptr;
			uint32_t size = 0;

			switch (h.type) {
				case HOOK_TYPE::HOT:
					start = addPtr(h.orgAddress, -5);
					size = 7;
					break;

				case HOOK_TYPE::JMP:
					start = h.orgAddress;
					size = h.orgBytes.size();
					break;

			}

			VirtualProtect(start, size, PAGE_EXECUTE_READWRITE, &h.oldProtect);
			memset(start, 0x90, size);


			// (unsigned char*)VirtualAlloc(NULL, 1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			std::string buildAsm = h.asmString;
			buildAsm = ReplaceAll(buildAsm, "@jmp", string_format("jmp 0x%x", h.retAddress));

			h.hAlloc = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			std::vector<uint8_t> asmCode = strToAsmBytes(buildAsm, h.hAlloc);
			memcpy(h.hAlloc, asmCode.data(), asmCode.size());




			if (h.type == HOOK_TYPE::HOT) {
				std::string jmpCodeStr = string_format("jmp short %d", start);
				std::vector<uint8_t> jmpCode = strToAsmBytes(jmpCodeStr, h.orgAddress);
				memcpy(h.orgAddress, jmpCode.data(), 2);
			}

			std::string jmpCodeStr = string_format("jmp %d", h.hAlloc);
			std::vector<uint8_t>  jmpCode = strToAsmBytes(jmpCodeStr, start);
			memcpy(start, jmpCode.data(), jmpCode.size());

		}
	}


	void unhookAll() {
		for (hookObject h : hookList) {
			if (h.orgBytes.size()) {
				//VirtualProtect(start, size, PAGE_EXECUTE_READWRITE, &h.oldProtect);
				memcpy(h.orgAddress, h.orgBytes.data(), h.orgBytes.size());
				VirtualFree(h.hAlloc, 0, MEM_RELEASE);
			}
		}
	}

private:
	std::vector<hookObject> hookList;
};


#endif // __MY_HOOK__