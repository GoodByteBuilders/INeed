#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

class SnakeMem {
private:
	DWORD processId = NULL;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	const char* processName;

public:
	SnakeMem(const char* processName);

	uintptr_t ReadPointer(uintptr_t base, DWORD offset);

	uintptr_t ReadPointers(uintptr_t base, const std::vector<unsigned int>& offsets);

	DWORD GetProcessId();
	HANDLE GetProcessHandle();
	uintptr_t GetModuleAddress(const char* module);

	template <typename T>
	bool Write(uintptr_t address, T value) {
		return WriteProcessMemory(hProcess, (LPVOID)address, &value, sizeof(T), nullptr);
	}

	template <typename T>
	T Read(uintptr_t address) {
		T buffer;
		ReadProcessMemory(hProcess, (LPCVOID)address, &buffer, sizeof(T), nullptr);
		return buffer;
	}

};