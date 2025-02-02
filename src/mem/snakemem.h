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

	uintptr_t ReadPointer(uintptr_t base, unsigned int offset);

	uintptr_t ReadPointers(uintptr_t base, const std::vector<unsigned int>& offsets);

	DWORD GetProcessId();
	HANDLE GetProcessHandle();
	uintptr_t GetModuleAddress(const char* module);

	template <typename T>
	bool WriteMemory(LPVOID address, T value) {
		return WriteProcessMemory(hProcess, address, &value, sizeof(T), nullptr);
	}

	template <typename T>
	bool ReadMemory(LPVOID address, T& value) {
		return ReadProcessMemory(hProcess, address, &value, sizeof(T), nullptr);
	}

};