#include "Scanner.h"




bool Scanner::MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask) noexcept {
	for (; *szMask; szMask++, bData++, bSig++)
		if (*szMask == 'x' && *bData != *bSig)
			return false;
	return true;
}

uintptr_t Scanner::FindSignature(int procID, module mod, const char* sig, const char* mask) noexcept {
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