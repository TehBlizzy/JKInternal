#pragma once

#include <iostream>

#include <Windows.h>
#include <TlHelp32.h>

struct module {
	uintptr_t dwBase, dwSize;
};

namespace SigScanner {
	bool MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask) {
		for (; *szMask; szMask++, bData++, bSig++)
			if (*szMask == 'x' && *bData != *bSig)
				return false;
		return true;
	}

	uintptr_t FindSignature(int procID, module mod, const char* sig, const char* mask) {
		BYTE* data = new BYTE[mod.dwSize];
		SIZE_T bytesRead;

		Toolhelp32ReadProcessMemory(procID, (LPVOID)(mod.dwBase), data, mod.dwSize, &bytesRead);

		for (uintptr_t i = 0; i < mod.dwSize; i++) {
			if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
				delete[] data;
				return i; // returns offset only
			}
		}
		delete[] data;
		return NULL;
	}
};