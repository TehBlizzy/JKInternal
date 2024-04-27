#define WIN32_LEAN_AND_MEAN

#include <thread>
#include <cstdint>

#include "jka.h"

void Setup(const HMODULE instance)
{
	try
	{
		//gui::Setup();
		cheat::Init();
		hooks::Setup();
	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "Error", MB_OK | MB_ICONEXCLAMATION);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_PAUSE))
	{
		std::this_thread::sleep_for(std::chrono::microseconds(200));
	}

UNLOAD:
	hooks::Destroy();
	gui::Destroy();
	FreeLibraryAndExitThread(instance, 0);
}

bool WINAPI DllMain(const HMODULE instance, const std::uintptr_t reason, const void* reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), instance, 0, nullptr);
	}
	return true;
}