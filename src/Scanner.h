#pragma once
#include <iostream>

#include <Windows.h>
#include <TlHelp32.h>

namespace Scanner
{
	struct module {
		uintptr_t dwBase, dwSize;
	};

	bool MemoryCompare(const BYTE* bData, const BYTE* bSig, const char* szMask) noexcept;

	uintptr_t FindSignature(int procID, module mod, const char* sig, const char* mask) noexcept;
}