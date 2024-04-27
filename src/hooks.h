#pragma once
#include "Scanner.h"
#include "jka.h"


namespace hooks
{
	void Setup();
	void Destroy() noexcept;

	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept
	{
		return (*static_cast<void***>(thisptr))[index];
	}

	inline LPVOID pSwapBuffers;
	inline LPVOID pGetProcAddress;

	using SwapBufferFn = bool(__stdcall*)(HDC hdc) noexcept;
	inline SwapBufferFn SwapBufferOriginal = nullptr;
	bool __stdcall SwapBuffer(HDC hdc) noexcept;

	inline cgImport_t* CGI;
	inline cgExport_t* CGE;

	inline bool IsOJK = false;
	inline bool IsMB2 = false;
	inline bool isLegacy = false;
	inline bool IS_SHUTDOWN = true;
	//inline int APILoadCount = 0;
	inline Scanner::module GameModule = {};

	using GetProcAddressFn = FARPROC(WINAPI*)(HMODULE moduleName, LPCSTR lpProcName) noexcept;
	inline GetProcAddressFn GetProcAddressOriginal = nullptr;
	FARPROC WINAPI GetProcAdd(HMODULE moduleName, LPCSTR lpProcName) noexcept;


	inline const char* OJK_VMCreateSig = "\x55\x8B\xEC\x53\x8B\x5D\x08\x8B";
	inline const char* OJK_VMCreateMask = "xxxxxxxx";
	using VMCreateFn = vm_t * (__cdecl*)(vmSlots_t);
	inline VMCreateFn VMCreateOriginal;
	vm_t* VMCreate(vmSlots_t slot);
	inline vmSlots_t VMSlot;

	inline const char* MB2_VMCreateSig = "\x55\x8B\xEC\x53\x8B\x5D\x0C\x85";
	inline const char* MB2_VMCreateMask = "xxxxxxxx";
	using VMCreateLegacyFn = vm_t * (__cdecl*)(vmSlots_t, intptr_t*);
	inline VMCreateLegacyFn VMCreateLegacyOriginal;
	vm_t* VMCreateLegacy(vmSlots_t slot, intptr_t*);
	inline vmSlots_t LegacyVMSlot;

	using DLLEntryFn = void(__cdecl*)(intptr_t(__cdecl*)(intptr_t, ...));
	inline DLLEntryFn DLLEntryOriginal;
	void DLLEntry(int (__cdecl* syscallptr)(int arg, ...));

	using VMMainFn = int(__cdecl*) (int, int, int, int, int, int, int, int, int, int, int, int, int);
	inline VMMainFn VMMainOriginal;
	int VMMain(int cmd, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11);

	using SysCallFn = int(__cdecl*)(int arg, ...);
	inline SysCallFn SysCallOriginal;
	int SysCall(int cmd, ...);

	using GetCGameAPIFn = int *(__cdecl*)(int entityNum, int* imports);
	inline GetCGameAPIFn GetCGameAPIOriginal;
	int* GetCGameAPI(int api, int* imports);

	inline const char* OJK_MouseEventSig = "\x55\x8B\xEC\x83\x3D\x00\x00\x00\x00\x00\x74\x00\xFF";// "\x55\x8B\xEC\x51\xC7\x45\xFC\xCC\xCC\xCC\xCC\x0F\xB6"; debug
	inline const char* OJK_MouseEventSigMask = "xxxxx?????x?x";// "xxxxxxxxxxxxx"; debug
	inline const char* MB2_MouseEventSig = "\x55\x8B\xEC\x51\x0F";
	inline const char* MB2_MouseEventSigMask = "xxxxx";
	using MouseEventFn = void(__cdecl*)(int dx, int dy, int time);
	inline MouseEventFn MouseEventOriginal;
	void MouseEvent(int dx, int dy, int time);

	inline const char* OJK_InputFrameSig = "\x56\xE8\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x83\xF8";// "\x55\x8B\xEC\x83\xEC\x08\xC7\x45\xF8\xCC\xCC\xCC\xCC\xC7\x45\xFC\xCC\xCC\xCC\xCC\xE8\x00\x00\x00\x00\x83\x3D\x00\x00\x00\x00\x01"; debug
	inline const char* OJK_InputFrameSigMask = "xx????x????xx";// "xxxxxxxxxxxxxxxxxxxxx????xx????x"; debug
	inline const char* MB2_InputFrameSig = "\x56\xE8\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x83\xF8";
	inline const char* MB2_InputFrameSigMask = "xx????x????xx";
	using InputFrameFn = void(__cdecl*)();
	inline InputFrameFn InputFrameOriginal;
	void InputFrame();

	inline const char* OJK_InputDeactivateMouseSig = "\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x68";// "\x55\x8B\xEC\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x85"; +12 offset debug
	inline const char* OJK_InputDeactivateMouseSigMask = "x?x????xx?xxxx????x";
	inline const char* MB2_InputDeactivateMouseSig = "\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x85\xC0\x0F\x84\x00\x00\x00\x00\x68";
	inline const char* MB2_InputDeactivateMouseSigMask = "x?x????xx?xxxx????x";
	using InputDeactivateMouseFn = void(__cdecl*)();
	inline InputDeactivateMouseFn InputDeactivateMouseOriginal;


	inline const char* OJK_SetCvar2Sig = "\x55\x8B\xEC\x51\x56";
	inline const char* OJK_SetCvar2Mask = "xxxxx";
	inline const char* MB2_SetCvar2Sig = "\x55\x8B\xEC\x51\x56\x57";
	inline const char* MB2_SetCvar2Mask = "xxxxxx";
	using SetCvar2Fn = cvar_t * (__cdecl*)(const char* var_name, const char* value, uint32_t defaultFlags, qboolean force);
	inline SetCvar2Fn SetCvar2Original;

	inline const char* OJK_InputProcessEventSig = "\x55\x8B\xEC\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xFC\x6A";// "\x6A\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x85\xC0\x75\x00\xE9\x00\x00\x00\x00\x8D"; // +16 offset debug
	inline const char* OJK_InputProcessEventSigMask = "xxxxx?x????xxxxxx";//  "x?x????xx?xxx?x????x"; debug
	inline const char* MB2_InputProcessEventSig = "\x55\x8B\xEC\x83\xEC\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xFC\x6A";
	inline const char* MB2_InputProcessEventSigMask = "xxxxx?x????xxxxxx";
	using InputProcessEventsFn = void(__cdecl*)();
	inline InputProcessEventsFn InputProcessEventsOriginal;

	inline const char* OJK_ClearStateSig = "\x81\xEC\x00\x00\x00\x00\xA1\x00\x00\x00\x00\x33\xC5\x89\x45\xFC\x56\x57\xE8\x00\x00\x00\x00\x68";// "\x55\x8B\xEC\x68\x00\x00\x00\x00\x6A\x00\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4\x00\x3B"; debug
	inline const char* OJK_ClearStateSigMask = "xx????x????xxxxxxxx????x";//  "xxxx????x?x????x????xx?x"; debug
	inline const char* MB2_ClearStateSig = "\xE8\x00\x00\x00\x00\x83\xC4\x00\xEB\x00\x83\x3D\x00\x00\x00\x00\x00\x0F";
	inline const char* MB2_ClearStateSigMask = "x????xx?x?xx?????x";
	//intptr_t clActivePtr = NULL;

	/*inline const char* CGWeaponSelectableSig = "\x55\x8B\xEC\x83\x7D\x08\x00\x75\x00\x33\xC0\xE9";
	inline const char* CGWeaponSelectableMask = "xxxxxxxx?xxx";
	using CGWeaponSelectableFn = qboolean(__cdecl*)(int i);
	inline CGWeaponSelectableFn CGWeaponSelectableOriginal;
	qboolean WeaponSelectable(int i);

	inline const char* CGOutOfAmmoChangeSig = "\x55\x8B\xEC\x51\x56\xC7\x45\xFC\xCC\xCC\xCC\xCC\xA1\x00\x00\x00\x00\xA3";
	inline const char* CGOutOfAmmoChangeMask = "xxxxxxxxxxxxx????x";
	using CGOutOfAmmoChangeFn = void(__cdecl*)(int i);
	inline CGOutOfAmmoChangeFn CGOutOfAmmoChangeOriginal;*/
	//void OutOfAmmoChange(int i);

	inline char* MB2CG;

	using GetGameStateFn = void(__cdecl*)(gameState_t* gs);
	inline GetGameStateFn GetGameStateOriginal;
	void GetGameState(gameState_t* gs);

	using UpdateEntityPosFn = void(__cdecl*)(int entityNum, const float*);
	inline UpdateEntityPosFn UpdateEntityPosOriginal;
	void UpdateEntityPos(int entityNum, const float* origin);

	using AddRefEntityFn = void(__cdecl*)(refEntity_t* re);
	inline AddRefEntityFn AddRefEntityOriginal;
	void AddRefEntity(refEntity_t* re);

	using GetSnapshotFn = qboolean(__cdecl*)(int snapshotNumber, snapshot_t* snapshot);
	inline GetSnapshotFn GetSnapshotOriginal;
	qboolean GetSnapshot(int snapshotNumber, snapshot_t* snapshot);

	using GetDefaultStateFn = qboolean(__cdecl*)(int index, entityState_t* state);
	inline GetDefaultStateFn GetDefaultStateOriginal;
	qboolean GetDefaultState(int index, entityState_t* state);

	using InitFn = void(__cdecl*)(int serverMessageNum, int serverCommandSequence, int clientNum);
	inline InitFn InitOriginal;
	void Init(int serverMessageNum, int serverCommandSequence, int clientNum);

	using DrawActiveFrameFn = void(__cdecl*)(int serverTime, int stereoView, bool demoPlayback);
	inline DrawActiveFrameFn DrawActiveFrameOriginal;
	void DrawActiveFrame(int serverTime, int stereoView, bool demoPlayback);

}