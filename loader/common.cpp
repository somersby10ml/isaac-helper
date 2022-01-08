#include "common.h"


unsigned long getProcess(tstring processName) {
	unsigned long processID = -1;

	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	do {
		if (!_tcscmp(procEntry.szExeFile, processName.c_str())) {
			processID = procEntry.th32ProcessID;
			break;
		}
	} while (Process32Next(hPID, &procEntry));

	CloseHandle(hPID);
	return processID;
}

tstring findDllFullPath(tstring name) {
	// is fullpath?
	if (name.find('\\') != std::string::npos) {
		if (PathFileExists(name.c_str()))
			return name;
	}
	else {
		tstring lpBuffer(MAX_PATH, 0);
		DWORD test = SearchPath(NULL, name.c_str(), nullptr, MAX_PATH, (TCHAR*)lpBuffer.data(), nullptr);
		if (test) {
			lpBuffer = lpBuffer.substr(0, test);
			if (PathFileExists(lpBuffer.c_str())) {
				return lpBuffer;
			}
		}

	}
	return _T("");
}

tstring getErrorString(unsigned long error_code) {
	LPTSTR lpMsgBuf = nullptr;
	unsigned long len = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error_code,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);


	if (lpMsgBuf && len) {
		tstring result(lpMsgBuf);
		result = result.substr(0, len - 2); // remove \n
		LocalFree(lpMsgBuf);
		return result;
	}

	return _T("");
}

unsigned long injection(unsigned long pid, tstring dllPath) {

	std::unique_ptr<void, handle_deleter> processHandle(
		OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pid)
	);
	size_t dllPathLength = dllPath.size() * sizeof(TCHAR);
	LPVOID allocatedMem = nullptr;

	if (processHandle.get() == INVALID_HANDLE_VALUE)
		return GetLastError();

	allocatedMem = VirtualAllocEx(processHandle.get(), NULL, dllPathLength, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (allocatedMem == nullptr)
		return GetLastError();

	BOOL bWrite = WriteProcessMemory(processHandle.get(), allocatedMem, dllPath.c_str(), dllPathLength, NULL);
	if (!bWrite)
		return GetLastError();

	HANDLE hThread = CreateRemoteThread(processHandle.get(), 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, allocatedMem, 0, 0);
	std::unique_ptr<void, handle_deleter> threadHandle(hThread);
	if (threadHandle.get() == nullptr)
		return GetLastError();

	return 0;
}

