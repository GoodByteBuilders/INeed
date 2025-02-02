#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class SnakeMem {
private:
	DWORD processId = NULL;
	HANDLE hProcess = INVALID_HANDLE_VALUE;

public:
	SnakeMem(const char* processName);

	DWORD GetProcessId() const;
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