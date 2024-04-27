#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <wtypes.h>
#include "Logo.h"

namespace gui
{
	inline bool open = false;
	inline bool setup = false;
	inline bool gotGameState = false;

	inline HWND window = nullptr;
	inline HDC hdc = nullptr;
	inline static HGLRC myContext = nullptr;
	inline static HGLRC originalContext = nullptr;
	//inline WNDCLASSEX windowClass = {};
	inline WNDPROC originalWindowProcess = nullptr;

	inline GLuint logo_texture;

	inline static bool MKEY = false;
	inline static bool AHKState = false;
	inline static bool ATKState = false;
	inline static bool THKState = false;
	inline static bool TTKState = false;
	

	#define PAD(base, alignment)	(((base)+(alignment)-1) & ~((alignment)-1))
	#define PADP(base, alignment)	((void *) PAD((intptr_t) (base), (alignment)))

	//void R_Clear();
	void R_DrawText(float x, float y, char* text, float* color);
	void R_DrawRect(float x, float y, float w, float h, float* color);
	void R_DrawFillRect(float x, float y, float w, float h, float* color);
	void R_End();



	enum rd_cmdIDs
	{
		RD_END,
		RD_TEXT,
		RD_RECT,
		RD_FILLRECT
	};

	struct rd_Commands
	{
		byte cmds[0x80000];
		int used;
	};

	static rd_Commands Commands;

	struct rd_cmd
	{
		int cmdID;
	};

	struct rd_text : rd_cmd
	{
		char* txt;
		float x, y;
		float color[4];
	};

	struct rd_rect : rd_cmd
	{
		float x, y, w, h;
		float color[4];
	};

	struct rd_fillrect : rd_cmd
	{
		float x, y, w, h;
		float color[4];
	};

	struct rd_end : rd_cmd
	{

	};

	static void* R_GetCmdBuffer(int bytes);

	//inline LPDIRECT3DDEVICE9 device = nullptr;
	//inline LPDIRECT3D9 d3d9 = nullptr;
	//bool SetupWindowClass(const char* windowClassName) noexcept;
	//void DestroyWindowClass() noexcept;

	//bool SetupWindow(const char* windowName) noexcept;
	//void DestroyWindow() noexcept;

	//bool SetupDirectX() noexcept;
	//void DestroyDirectX() noexcept;
	//bool SetupOGL() noexcept;
	//void DestroyOGL() noexcept;

	//void Setup();

	void SetupMenu(HDC hdc) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
}