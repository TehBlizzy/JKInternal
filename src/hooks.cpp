#pragma once

#include "jka.h"
#include <stdexcept>
#include "../external/winhook/MinHook.h"
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_opengl3.h"
#include "../external/imgui/imgui_impl_win32.h"

void hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to init MinHook.");

	HMODULE hMod = GetModuleHandle("opengl32.dll");
	if (!hMod)
		throw std::runtime_error("Unable to get opengl32.dll");

	pSwapBuffers = GetProcAddress(hMod, "wglSwapBuffers");
	if (!pSwapBuffers)
		throw std::runtime_error("Unable to get wglSwapBuffers.");
	if (MH_CreateHook(pSwapBuffers, &SwapBuffer, reinterpret_cast<void**>(&SwapBufferOriginal)))
		throw std::runtime_error("Unable to hook SwapBuffers()");

	HMODULE GPAMod = GetModuleHandle("kernel32.dll");
	if (!GPAMod)
		throw std::runtime_error("Unable to get kernel32.dll");

	pGetProcAddress = GetProcAddress(GPAMod, "GetProcAddress");
	if (!pGetProcAddress)
		throw std::runtime_error("Unable to get GetProcAddress.");
	if (MH_CreateHook(pGetProcAddress, &GetProcAdd, reinterpret_cast<void**>(&GetProcAddressOriginal)))
		throw std::runtime_error("Unable to hook GetProcAddress()");

	uintptr_t procID = GetCurrentProcessId();
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	GameModule = {NULL,NULL};
	do
	{
		if (!strcmp(mEntry.szModule, "openjk.x86.exe"))
		{
			IsOJK = true;
			GameModule = {(uintptr_t)mEntry.hModule, mEntry.modBaseSize};
		}
		if (!strcmp(mEntry.szModule, "mbii.x86.exe"))
		{
			IsMB2 = true;
			GameModule = {(uintptr_t)mEntry.hModule, mEntry.modBaseSize};
		}
	}
	while (Module32Next(hModule, &mEntry));
	CloseHandle(hModule);

	if (!IsOJK && !IsMB2)
	{
		throw std::runtime_error(
			"Make sure the target process has the original executeable file names for OJK or MBII. \n (openjk.x86.exe or mbii.x86.exe)");
		return;
	}

	if (IsOJK && GameModule.dwBase) //&& vmMainMod.dwBase)
	{
		uintptr_t VMCreatePtr = FindSignature(procID, GameModule, OJK_VMCreateSig, OJK_VMCreateMask);
		if (!VMCreatePtr)
			throw std::runtime_error("Unable to find pointer to VM_Create.");
		if (MH_CreateHook(reinterpret_cast<void*>(GameModule.dwBase + VMCreatePtr), &VMCreate,
		                  reinterpret_cast<void**>(&VMCreateOriginal)))
			throw std::runtime_error("Unable to hook VM_Create.");

		uintptr_t MouseEventPtr = FindSignature(procID, GameModule, OJK_MouseEventSig, OJK_MouseEventSigMask);
		if (!MouseEventPtr)
			throw std::runtime_error("Unable to find pointer to CL_MouseEvent.");
		if (MH_CreateHook(reinterpret_cast<void*>(GameModule.dwBase + MouseEventPtr), &MouseEvent,
		                  reinterpret_cast<void**>(&MouseEventOriginal)))
			throw std::runtime_error("Unable to hook CL_MouseEvent.");

		uintptr_t InputFramePtr = FindSignature(procID, GameModule, OJK_InputFrameSig, OJK_InputFrameSigMask);
		if (!InputFramePtr)
			throw std::runtime_error("Unable to find pointer to IN_InputFrame.");
		if (MH_CreateHook(reinterpret_cast<void*>(GameModule.dwBase + InputFramePtr), &InputFrame,
		                  reinterpret_cast<void**>(&InputFrameOriginal)))
			throw std::runtime_error("Unable to hook IN_InputFrame.");

		uintptr_t InputDeactivateMousePtr = FindSignature(procID, GameModule, OJK_InputDeactivateMouseSig,
		                                                  OJK_InputDeactivateMouseSigMask);
		if (!InputDeactivateMousePtr)
			throw std::runtime_error("Unable to find pointer to IN_DeactivateMouse.");
		InputDeactivateMouseOriginal = reinterpret_cast<InputDeactivateMouseFn>(GameModule.dwBase +
			InputDeactivateMousePtr);
		if (!InputDeactivateMouseOriginal)
			throw std::runtime_error("Unable to hook IN_DeactivateMouse.");

		uintptr_t InputProcessEventsPtr = FindSignature(procID, GameModule, OJK_InputProcessEventSig,
		                                                OJK_InputProcessEventSigMask);
		if (!InputProcessEventsPtr)
			throw std::runtime_error("Unable to find pointer to IN_ProcessEvents.");
		InputProcessEventsOriginal = reinterpret_cast<InputProcessEventsFn>(GameModule.dwBase + InputProcessEventsPtr);
		// -0x28); debug
		if (!InputProcessEventsOriginal)
			throw std::runtime_error("Unable to hook IN_ProcessEvents.");

		uintptr_t SetCvar2Ptr = FindSignature(procID, GameModule, OJK_SetCvar2Sig, OJK_SetCvar2Mask);
		if (!SetCvar2Ptr)
			throw std::runtime_error("Unable to find pointer to Cvar_Set2.");
		SetCvar2Original = reinterpret_cast<SetCvar2Fn>(GameModule.dwBase + SetCvar2Ptr); // -0x28); debug
		if (!SetCvar2Original)
			throw std::runtime_error("Unable to hook Cvar_Set2.");

		uintptr_t CLClearStatePtr = FindSignature(procID, GameModule, OJK_ClearStateSig, OJK_ClearStateSigMask);
		if (!CLClearStatePtr)
			throw std::runtime_error("Unable to find pointer to CL_ClearState.");

		uintptr_t OJK_ClientActive = GameModule.dwBase + CLClearStatePtr + 0x1F; // 0x0B; debug
		auto ca = (clientActive_t*)*(intptr_t*)OJK_ClientActive;
		if (!ca)
			throw std::runtime_error("Unable to get pointer to clientActive");
		cheat::cl.ViewAngles = static_cast<float*>(ca->viewangles);
	}
	else if (IsMB2 && GameModule.dwBase) //&& vmMainMod.dwBase)
	{
		uintptr_t VMCreateLgeacyPtr = FindSignature(procID, GameModule, MB2_VMCreateSig, MB2_VMCreateMask);
		if (!VMCreateLgeacyPtr)
			throw std::runtime_error("Unable to find pointer to VM_CreateLegacy.");
		if (MH_CreateHook(reinterpret_cast<void*>(GameModule.dwBase + VMCreateLgeacyPtr), &VMCreateLegacy,
		                  reinterpret_cast<void**>(&VMCreateLegacyOriginal)))
			throw std::runtime_error("Unable to hook VM_CreateLegacy.");


		uintptr_t MouseEventPtr = FindSignature(procID, GameModule, MB2_MouseEventSig, MB2_MouseEventSigMask);
		if (!MouseEventPtr)
			throw std::runtime_error("Unable to find pointer to CL_MouseEvent.");
		if (MH_CreateHook(reinterpret_cast<void*>(GameModule.dwBase + MouseEventPtr), &MouseEvent,
		                  reinterpret_cast<void**>(&MouseEventOriginal)))
			throw std::runtime_error("Unable to hook CL_MouseEvent.");

		uintptr_t InputFramePtr = FindSignature(procID, GameModule, MB2_InputFrameSig, MB2_InputFrameSigMask);
		if (!InputFramePtr)
			throw std::runtime_error("Unable to find pointer to IN_InputFrame.");
		if (MH_CreateHook(reinterpret_cast<void*>(GameModule.dwBase + InputFramePtr), &InputFrame,
		                  reinterpret_cast<void**>(&InputFrameOriginal)))
			throw std::runtime_error("Unable to hook IN_InputFrame.");

		uintptr_t InputDeactivateMousePtr = FindSignature(procID, GameModule, MB2_InputDeactivateMouseSig,
		                                                  MB2_InputDeactivateMouseSigMask);
		if (!InputDeactivateMousePtr)
			throw std::runtime_error("Unable to find pointer to IN_DeactivateMouse.");
		InputDeactivateMouseOriginal = reinterpret_cast<InputDeactivateMouseFn>(GameModule.dwBase +
			InputDeactivateMousePtr);
		if (!InputDeactivateMouseOriginal)
			throw std::runtime_error("Unable to hook IN_DeactivateMouse.");

		uintptr_t InputProcessEventsPtr = FindSignature(procID, GameModule, MB2_InputProcessEventSig,
		                                                MB2_InputProcessEventSigMask);
		if (!InputProcessEventsPtr)
			throw std::runtime_error("Unable to find pointer to IN_ProcessEvents.");
		InputProcessEventsOriginal = reinterpret_cast<InputProcessEventsFn>(GameModule.dwBase + InputProcessEventsPtr);
		// -0x28);
		if (!InputProcessEventsOriginal)
			throw std::runtime_error("Unable to hook IN_ProcessEvents.");

		uintptr_t SetCvar2Ptr = FindSignature(procID, GameModule, MB2_SetCvar2Sig, MB2_SetCvar2Mask);
		if (!SetCvar2Ptr)
			throw std::runtime_error("Unable to find pointer to Cvar_Set2.");
		SetCvar2Original = reinterpret_cast<SetCvar2Fn>(GameModule.dwBase + SetCvar2Ptr); // -0x28); debug
		if (!SetCvar2Original)
			throw std::runtime_error("Unable to hook Cvar_Set2.");

		uintptr_t CLClearStatePtr = FindSignature(procID, GameModule, MB2_ClearStateSig, MB2_ClearStateSigMask);
		if (!CLClearStatePtr)
			throw std::runtime_error("Unable to find pointer to CL_ClearState.");

		uintptr_t MB2_ClientActive = GameModule.dwBase + CLClearStatePtr + 0x4C;
		auto ca = (clientActive_t*)*(intptr_t*)MB2_ClientActive;
		if (!ca)
			throw std::runtime_error("Unable to get pointer to clientActive");
		cheat::cl.ViewAngles = static_cast<float*>(ca->viewangles);
	}
	else
	{
		throw std::runtime_error("Unable to get executeable module.");
		return;
	}

	if (MH_EnableHook(nullptr))
		throw std::runtime_error("Unable to enable hooks.");

	//gui::DestroyOGL();
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(nullptr);
	MH_RemoveHook(nullptr);
	MH_Uninitialize();
}

bool __stdcall hooks::SwapBuffer(HDC hdc) noexcept
{
	if (!gui::setup)
		gui::SetupMenu(hdc);

	//if (gui::open)
	gui::Render();

	return SwapBufferOriginal(hdc);
}

void hooks::InputFrame()
{
	if (gui::open)
	{
		InputDeactivateMouseOriginal();
		InputProcessEventsOriginal();
		return;
	}
	InputFrameOriginal();
}

void hooks::MouseEvent(int dx, int dy, int time)
{
	if (gui::open)
		return;
	MouseEventOriginal(dx, dy, time);
}

FARPROC WINAPI hooks::GetProcAdd(HMODULE hModule, LPCSTR lpProcName) noexcept
{
	TCHAR moduleName[MAX_PATH];
	char moduleClean[32];
	int i = 0, markLast = 0;

	if (!GetModuleFileName(hModule, moduleName, sizeof(moduleName)))
	{
		return GetProcAddressOriginal(hModule, lpProcName);
	}

	for (i = 0; i < strlen(moduleName); i++) if (moduleName[i] == '\\') markLast = i + 1;
	memset(moduleClean, 0, sizeof(moduleClean));
	memcpy(moduleClean, moduleName + markLast, strlen(moduleName) - markLast);
	if ((strcmp(moduleClean, "cgamex86.dll") == 0 || VMSlot == VM_CGAME) && strcmp(lpProcName, "GetModuleAPI") == 0)
	{
		/*uintptr_t procID = GetCurrentProcessId();
		HANDLE hProcMod = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		Scanner::module cgamemod = { NULL,NULL };
		do {
			if (!strcmp(mEntry.szModule, "cgamex86.dll")) {
				cgamemod = { (uintptr_t)mEntry.hModule, mEntry.modBaseSize };
			}
		} while (Module32Next(hProcMod, &mEntry));
		CloseHandle(hProcMod);

		if (cgamemod.dwBase)
		{
			uintptr_t CGWeaponSelectablePtr = Scanner::FindSignature(procID, cgamemod, CGWeaponSelectableSig, CGWeaponSelectableMask);
			if (!CGWeaponSelectablePtr)
				throw std::runtime_error("Unable to find pointer to CG_WeaponSelectable.");
			if (MH_CreateHook(reinterpret_cast<qboolean*>(cgamemod.dwBase + CGWeaponSelectablePtr), &WeaponSelectable, reinterpret_cast<void**>(&CGWeaponSelectableOriginal)))
				throw std::runtime_error("Unable to hook CG_WeaponSelectable.");
		}
		*/
		GetCGameAPIOriginal = (GetCGameAPIFn)GetProcAddressOriginal(hModule, lpProcName);
		if (GetCGameAPIOriginal)
		{
			return (FARPROC)(&GetCGameAPI);
		}
		isLegacy = true;
	}
	if (LegacyVMSlot == VM_CGAME && strcmp(lpProcName, "dllEntry") == 0)
	{
		isLegacy = true;
		DLLEntryOriginal = (DLLEntryFn)GetProcAddressOriginal(hModule, lpProcName);
		if (DLLEntryOriginal)
		{
			return (FARPROC)(&DLLEntry);
		}
	}
	if (LegacyVMSlot == VM_CGAME && strcmp(lpProcName, "vmMain") == 0)
	{
		isLegacy = true;
		VMMainOriginal = (VMMainFn)GetProcAddressOriginal(hModule, lpProcName);
		if (VMMainOriginal)
		{
			char* CGInit_Offset = reinterpret_cast<char*>(VMMainOriginal) + 0x36;
			int Offset_Value = 0;
			auto intPtr = reinterpret_cast<int*>(CGInit_Offset);

			for (int i = 0; i < sizeof(int); ++i)
			{
				Offset_Value |= (static_cast<int>(*(CGInit_Offset + i)) & 0xFF) << (i * 8);
			}
			char* CGInitPtr = (CGInit_Offset + 0x04) + Offset_Value;
			char* CGClientNum = CGInitPtr + 0x1A8;

			int Struct_Value = 0;
			for (int i = 0; i < sizeof(int); ++i)
			{
				Struct_Value |= (static_cast<int>(*(CGClientNum + i)) & 0xFF) << (i * 8);
			}

			cheat::cl.LocalClientNumber = (int*)static_cast<size_t>(Struct_Value);
			cheat::cl.RefDef = (refdef_t*)static_cast<size_t>(Struct_Value + 0xD60);
			return (FARPROC)(&VMMain);
		}
	}
	return GetProcAddressOriginal(hModule, lpProcName);
}

vm_t* hooks::VMCreate(vmSlots_t slot)
{
	VMSlot = slot;
	return VMCreateOriginal(slot);
}

vm_t* hooks::VMCreateLegacy(vmSlots_t slot, intptr_t* systemCalls)
{
	LegacyVMSlot = slot;
	return VMCreateLegacyOriginal(slot, systemCalls);
}

void hooks::DLLEntry(int (__cdecl*syscallptr)(int arg, ...))
{
	SysCallOriginal = syscallptr;
	DLLEntryOriginal(SysCall);
}

int hooks::SysCall(int cmd, ...)
{
	int arg[14];
	va_list arglist;
	int count;
	va_start(arglist, cmd);
	for (count = 0; count < 14; count++)
	{
		arg[count] = va_arg(arglist, int);
	}
	va_end(arglist);


	switch (cmd)
	{
	case CG_S_UPDATEENTITYPOSITION:
		UpdateEntityPos(arg[0], (const float*)arg[1]);
		break;
	case CG_R_ADDREFENTITYTOSCENE:
		AddRefEntity((refEntity_t*)arg[0]);
		break;
	case CG_GETGAMESTATE:
		GetGameState((gameState_t*)arg[0]);
		break;
	case CG_GETSNAPSHOT:
		GetSnapshot(arg[0], (snapshot_t*)arg[1]);
		break;
	case CG_GETDEFAULTSTATE:
		GetDefaultState(arg[0], (entityState_t*)arg[1]);
		break;
	}


	return SysCallOriginal(cmd, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10],
	                       arg[11], arg[12], arg[13]);
}

int hooks::VMMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8,
                  int arg9, int arg10, int arg11)
{
	switch (cmd)
	{
	case CG_INIT:
		VMMainOriginal(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
		Init(arg0, arg1, arg2);
		return 0;

	case CG_DRAW_ACTIVE_FRAME:
		VMMainOriginal(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
		DrawActiveFrame(arg0, arg1, arg2);
		return 0;
	case CG_SHUTDOWN:
		IS_SHUTDOWN = true;
		return 0;
	}

	return VMMainOriginal(cmd, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}

int* hooks::GetCGameAPI(int api, int* imports)
{
	CGI = {};

	CGI = (cgImport_t*)imports;

	GetGameStateOriginal = ((cgImport_t*)imports)->GetGameState;
	((cgImport_t*)imports)->GetGameState = &GetGameState;

	UpdateEntityPosOriginal = ((cgImport_t*)imports)->S_UpdateEntityPosition;
	((cgImport_t*)imports)->S_UpdateEntityPosition = &UpdateEntityPos;

	AddRefEntityOriginal = ((cgImport_t*)imports)->R_AddRefEntityToScene;
	((cgImport_t*)imports)->R_AddRefEntityToScene = &AddRefEntity;

	GetSnapshotOriginal = ((cgImport_t*)imports)->GetSnapshot;
	((cgImport_t*)imports)->GetSnapshot = &GetSnapshot;

	GetDefaultStateOriginal = ((cgImport_t*)imports)->GetDefaultState;
	((cgImport_t*)imports)->GetDefaultState = &GetDefaultState;

	CGE = {};

	CGE = (cgExport_t*)GetCGameAPIOriginal(api, imports);

	InitOriginal = static_cast<InitFn>(CGE->Init);
	CGE->Init = &Init;

	DrawActiveFrameOriginal = static_cast<DrawActiveFrameFn>(CGE->DrawActiveFrame);
	CGE->DrawActiveFrame = &DrawActiveFrame;

	return (int*)CGE;
}

void hooks::GetGameState(gameState_t* gs)
{
	//cheat::cl.gstate = gs;
	cgs_t* tmp = nullptr;
	auto cgs = (cgs_t*)((int)gs - (int)&tmp->gameState);
	cheat::cl.GameType = &(cgs->gametype);
	cheat::cl.ScreenXScale = &(cgs->screenXScale);
	cheat::cl.ScreenYScale = &(cgs->screenYScale);
	if (!IsMB2)
		cheat::cl.OJK_ClientInfo = cgs->clientinfo;
	else
		cheat::cl.MB2_ClientInfo = (mb2_clientInfo_t*)((int)cgs + 0xC880);
	if (!isLegacy)
		GetGameStateOriginal(gs);
}

void hooks::UpdateEntityPos(int entityNum, const float* origin)
{
	cheat::lastEntityIndex = entityNum;

	if (!isLegacy)
		UpdateEntityPosOriginal(entityNum, origin);
}

void hooks::AddRefEntity(refEntity_t* re)
{
	if (cheat::IsEntityPlayer() != 0)
	{
		if (cheat::EnableWallhack)
			re->renderfx |= RF_DEPTHHACK | RDF_NOFOG;
		if (cheat::EnableWHShader)
		{
			if (!cheat::IsSameTeam(cheat::cl.Entities[cheat::lastEntityIndex]))
				//	re->customShader = cheat::WHAllyShader;
				//else
				re->customShader = cheat::WHEnemyShader;
		}
		cheat::cl.RefEntities[cheat::lastEntityIndex] = re;
	}

	if (!isLegacy)
		AddRefEntityOriginal(re);
}

qboolean hooks::GetSnapshot(int snapshotNumber, snapshot_t* snapshot)
{
	qboolean result = qfalse;
	if (!isLegacy)
	{
		result = GetSnapshotOriginal(snapshotNumber, snapshot);
		cg_t* tmp = nullptr;
		tmp = (cg_t*)((int)snapshot - (int)&tmp->activeSnapshots);
		if (tmp->latestSnapshotNum != 0)
		{
			cheat::cl.LocalClientNumber = &(tmp->clientNum);
			cheat::cl.RefDef = &(tmp->refdef);
			cheat::cl.PlayerState = &(snapshot->ps);
		}
	}
	else if (IsMB2)
	{
		cheat::cl.PlayerState = &(snapshot->ps);
	}

	return result;
}

qboolean hooks::GetDefaultState(int index, entityState_t* state)
{
	if (!isLegacy)
	{
		cheat::cl.Entities[index] = (centity_t*)state;
		return GetDefaultStateOriginal(index, state);
	}
	cheat::cl.Entities[index] = (centity_t*)state;
	return qtrue;
}

void hooks::Init(int serverMessageNum, int serverCommandSequence, int clientNum)
{
	cheat::CG_Init();
	IS_SHUTDOWN = false;
	if (!isLegacy)
		InitOriginal(serverMessageNum, serverCommandSequence, clientNum);
}

void hooks::DrawActiveFrame(int serverTime, int stereoView, bool demoPlayback)
{
	if (!isLegacy)
		DrawActiveFrameOriginal(serverTime, stereoView, demoPlayback);
	cheat::CG_DrawActiveFrame();
}
