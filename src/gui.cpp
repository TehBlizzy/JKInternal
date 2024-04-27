#include "jka.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_opengl3.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_internal.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	if (!RegisterClassEx(&windowClass))
		return false;
	return true;
}
void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, 0, 0, windowClass.hInstance, 0);
	if (!window)
		return false;
	return true;
}
void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupOGL()
{
	HDC hDc = ::GetDC(window);
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	const int pf = ::ChoosePixelFormat(hDc, &pfd);
	if (pf == 0)
		return false;
	if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
		return false;
	::ReleaseDC(window, hDc);

	hdc = ::GetDC(window);
	if (!myContext)
		myContext = wglCreateContext(hdc);
	return true;
}

void gui::DestroyOGL()
{
	wglMakeCurrent(NULL, NULL);
	::ReleaseDC(window, hdc);
}*/

/*bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");
	if (!handle)
		return false;
	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create)
		return false;
	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS pm = {};
	pm.BackBufferWidth = 0;
	pm.BackBufferHeight = 0;
	pm.BackBufferFormat = D3DFMT_UNKNOWN;
	pm.BackBufferCount = 0;
	pm.MultiSampleType = D3DMULTISAMPLE_NONE;
	pm.MultiSampleQuality = NULL;
	pm.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pm.hDeviceWindow = window;
	pm.Windowed = 1;
	pm.EnableAutoDepthStencil = 0;
	pm.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	pm.Flags = NULL;
	pm.FullScreen_RefreshRateInHz = 0;
	pm.PresentationInterval = 0;

	if (d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, window, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &pm, &device) < 0)
		return false;

	return true;
}
void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	if (!SetupWindowClass("JKAOverlay"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("JKAOverlay"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupOGL())
		throw std::runtime_error("Failed to create OGL device");

	DestroyWindow();
	DestroyWindowClass();
}
*/
void gui::SetupMenu(HDC hDc) noexcept
{
	window = WindowFromDC(hDc);
	hdc = hDc;
	originalContext = wglGetCurrentContext();
	originalWindowProcess = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,
	                                                                   reinterpret_cast<LONG_PTR>(WindowProcess)));
	myContext = wglCreateContext(hDc);
	wglMakeCurrent(hDc, myContext);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	glOrtho(0, m_viewport[2], m_viewport[3], 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 1);
	int width, height, channels;
	unsigned char* imagePixels = stbi_load_from_memory(LogoData, 390 * 250 * 4, &width, &height, &channels, 0);
	glGenTextures(1, &logo_texture);
	glBindTexture(GL_TEXTURE_2D, logo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 390, 250, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagePixels);

	stbi_image_free(imagePixels);

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplOpenGL3_Init();

	wglMakeCurrent(hDc, originalContext);

	setup = true;
}

void gui::Destroy() noexcept
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWindowProcess));
}

void gui::Render() noexcept
{
	wglMakeCurrent(hdc, myContext);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	if (!hooks::IS_SHUTDOWN)
	{
		if (cheat::RadarPosX + cheat::RadarSize > io.DisplaySize.x)
			cheat::RadarPosX = io.DisplaySize.x - cheat::RadarSize - 1;
		if (cheat::RadarPosY + cheat::RadarSize > io.DisplaySize.y)
			cheat::RadarPosY = io.DisplaySize.y - cheat::RadarSize - 1;
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
		ImGui::Begin("Overlay", nullptr,
		             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
		auto pDrawList = ImGui::GetWindowDrawList();
		//pDrawList->AddRectFilled(ImVec2(0, 0), ImVec2(50, 50), ImColor(255, 255, 255, 255));
		void* data = Commands.cmds;
		bool isDone = false;
		while (!isDone)
		{
			if (hooks::IS_SHUTDOWN)
			{
				isDone = true;
				break;
			}
			data = PADP(data, sizeof(void*));
			switch (*static_cast<const int*>(data))
			{
			case RD_TEXT:
				{
					rd_text* cmd;
					cmd = static_cast<rd_text*>(data);
					if (cmd->txt)
						pDrawList->AddText(ImVec2(cmd->x, cmd->y),
						                   ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]),
						                   cmd->txt);
					data = static_cast<void*>(cmd + 1);
					break;
				}
			case RD_RECT:
				{
					rd_rect* cmd;
					cmd = static_cast<rd_rect*>(data);
					pDrawList->AddRect(ImVec2(cmd->x, cmd->y), ImVec2(cmd->w, cmd->h),
					                   ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]));
					data = static_cast<void*>(cmd + 1);
					break;
				}
			case RD_FILLRECT:
				rd_fillrect* cmd;
				cmd = static_cast<rd_fillrect*>(data);
				pDrawList->AddRectFilled(ImVec2(cmd->x, cmd->y), ImVec2(cmd->w, cmd->h),
				                         ImColor(cmd->color[0], cmd->color[1], cmd->color[2], cmd->color[3]));
				data = static_cast<void*>(cmd + 1);
				break;
			case RD_END:
			default:
				isDone = true;
				break;
			}
		}

		ImGui::End();
	}
	if (open)
	{
		ImGui::SetNextWindowSize(ImVec2(420, 400));
		if (ImGui::Begin("JKInternal", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("Tabs"))
			{
				if (ImGui::BeginTabItem("Main"))
				{
					ImGui::SetCursorPosX(15);
					ImGui::Image((void*)static_cast<intptr_t>(logo_texture), ImVec2(390, 250));
					//ImGui::Columns(2);
					ImGui::Text("Settings Window Hotkey:");
					ImGui::SameLine();
					ImGui::Hotkey("##MKEY", &cheat::MenuKey, MKEY, ImVec2(200, 18));
					//ImGui::EndColumns();
					ImGui::Spacing();
					ImGui::Text("JKI by Blizzie. Original implementation by King-OrgY");


					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("ESP"))
				{
					//Extra things? Change colors for boxes/names etc. Weapon info, distance
					if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable ESP: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##EESP", &cheat::EnableESP);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Box ESP: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##EBESP", &cheat::EnableBoxESP);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Name ESP: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##ENESP", &cheat::EnableNameESP);
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Wallhack"))
				{
					if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedFit))
					{
						ImGui::TableSetupColumn("##SETL", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("##SETINP", ImGuiTableColumnFlags_WidthStretch);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Wallhack: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##EWH", &cheat::EnableWallhack);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Custom Shader: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##EWHS", &cheat::EnableWHShader);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enemy Shader: ");
						ImGui::TableNextColumn();
						const char* items[] = {
							"gfx/effects/blaster_blob", "gfx/effects/sabers/red_glow", "gfx/effects/sabers/orange_glow",
							"gfx/effects/sabers/yellow_glow", "gfx/effects/sabers/green_glow",
							"gfx/effects/sabers/blue_glow", "gfx/effects/sabers/purple_glow", "powerups/ysalimarishell",
							"gfx/effects/burn", "gfx/misc/forceprotect"
						};
						static int selectedEnemyIndex = 0;
						ImGui::PushItemWidth(-1);
						if (ImGui::BeginListBox("##WHCES"))
						{
							for (int n = 0; n < IM_ARRAYSIZE(items); n++)
							{
								const bool is_selected_enemy = (selectedEnemyIndex == n);
								if (ImGui::Selectable(items[n], is_selected_enemy))
								{
									selectedEnemyIndex = n;
									cheat::WHEnemyShaderName = items[selectedEnemyIndex];
									cheat::UpdateShader();
								}

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected_enemy)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndListBox();
						}
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Ally Shader: ");
						ImGui::TableNextColumn();
						static int selectedAllyIndex = 0;
						ImGui::PushItemWidth(-1);
						if (ImGui::BeginListBox("##WHCAS"))
						{
							for (int n = 0; n < IM_ARRAYSIZE(items); n++)
							{
								const bool is_selected_ally = (selectedAllyIndex == n);
								if (ImGui::Selectable(items[n], is_selected_ally))
								{
									selectedAllyIndex = n;
									cheat::WHAllyShaderName = items[selectedAllyIndex];
									cheat::UpdateShader();
								}

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected_ally)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndListBox();
						}
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Aimbot"))
				{
					if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Aimbot: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##EABT", &cheat::EnableAimbot);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Aimbot Angle: ");
						ImGui::TableNextColumn();
						ImGui::InputFloat("##AMA", &cheat::AimbotMaxAngle, 0.1f, 1.0f);
						if (cheat::AimbotMaxAngle < 1.0f)
							cheat::AimbotMaxAngle = 1.0f;
						if (cheat::AimbotMaxAngle > 180.0f)
							cheat::AimbotMaxAngle = 180.0f;
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Aimbot Hold Key: ");
						ImGui::TableNextColumn();
						ImGui::Hotkey("##AHK", &cheat::AimbotHoldKey, AHKState, ImVec2(200, 18));
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Aimbot Toggle Key: ");
						ImGui::TableNextColumn();
						ImGui::Hotkey("##ATK", &cheat::AimbotToggleKey, ATKState, ImVec2(200, 18));
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Triggerbot"))
				{
					if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Triggerbot: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##ETBT", &cheat::EnableTriggerBot);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Triggerbot Hold Key: ");
						ImGui::TableNextColumn();
						ImGui::Hotkey("##THK", &cheat::TriggerbotHoldKey, THKState, ImVec2(200, 18));
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Triggerbot Toggle Key: ");
						ImGui::TableNextColumn();
						ImGui::Hotkey("##TTK", &cheat::TriggerbotToggleKey, TTKState, ImVec2(200, 18));
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Radar"))
				{
					if (ImGui::BeginTable("Settings", 2, ImGuiTableFlags_SizingFixedSame))
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Enable Radar: ");
						ImGui::TableNextColumn();
						ImGui::Checkbox("##ERDR", &cheat::EnableRadar);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Radar BG Color: ");
						ImGui::TableNextColumn();
						ImGui::SetNextItemWidth(190);
						ImGui::ColorEdit4("##RBGC", (float*)&cheat::RadarBGColor);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Radar Size: ");
						ImGui::TableNextColumn();
						ImGui::SliderInt("##RPS", &cheat::RadarSize, 20, 400);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Radar X Position: ");
						ImGui::TableNextColumn();

						ImGui::SliderInt("##RPX", &cheat::RadarPosX, 1, io.DisplaySize.x);
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Radar Y Position: ");
						ImGui::TableNextColumn();

						ImGui::SliderInt("##RPY", &cheat::RadarPosY, 1, io.DisplaySize.y);
						ImGui::EndTable();
					}
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::SetCursorPos(ImVec2(120, 370));
			if (ImGui::Button("Load Config"))
			{
				cheat::LoadConfig();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save Config"))
			{
				cheat::SaveConfig();
			}
		}
		ImGui::End();
	}


	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	wglMakeCurrent(hdc, originalContext);
}

static void* gui::R_GetCmdBuffer(int bytes)
{
	rd_Commands* cmdList;
	cmdList = &Commands;
	bytes = PAD(bytes, sizeof(void*));
	if (cmdList->used + bytes > 0x80000)
		return nullptr;
	cmdList->used += bytes;
	return cmdList->cmds + cmdList->used - bytes;
}

void gui::R_DrawText(float x, float y, char* text, float* color)
{
	rd_text* cmd;
	cmd = static_cast<rd_text*>(R_GetCmdBuffer(sizeof(*cmd)));
	if (!cmd)
		return;
	cmd->cmdID = RD_TEXT;
	cmd->x = x;
	cmd->y = y;
	cmd->txt = text;
	cmd->color[0] = color[0];
	cmd->color[1] = color[1];
	cmd->color[2] = color[2];
	cmd->color[3] = color[3];
}

void gui::R_DrawRect(float x, float y, float w, float h, float* color)
{
	rd_rect* cmd;
	cmd = static_cast<rd_rect*>(R_GetCmdBuffer(sizeof(*cmd)));
	if (!cmd)
		return;
	cmd->cmdID = RD_RECT;
	cmd->x = x;
	cmd->y = y;
	cmd->w = w;
	cmd->h = h;
	cmd->color[0] = color[0];
	cmd->color[1] = color[1];
	cmd->color[2] = color[2];
	cmd->color[3] = color[3];
}

void gui::R_DrawFillRect(float x, float y, float w, float h, float* color)
{
	rd_fillrect* cmd;
	cmd = static_cast<rd_fillrect*>(R_GetCmdBuffer(sizeof(*cmd)));
	if (!cmd)
		return;
	cmd->cmdID = RD_FILLRECT;
	cmd->x = x;
	cmd->y = y;
	cmd->w = w;
	cmd->h = h;
	cmd->color[0] = color[0];
	cmd->color[1] = color[1];
	cmd->color[2] = color[2];
	cmd->color[3] = color[3];
}

void gui::R_End()
{
	rd_end* cmd;
	cmd = static_cast<rd_end*>(R_GetCmdBuffer(sizeof(*cmd)));
	if (!cmd)
		return;
	cmd->cmdID = RD_END;
	Commands.used = 0;
}


const char* KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};

static const int KeyCodes[] = {
	0x0, //Undefined
	0x01,
	0x02,
	0x03,
	0x04,
	0x05,
	0x06,
	0x07, //Undefined
	0x08,
	0x09,
	0x0A, //Reserved
	0x0B, //Reserved
	0x0C,
	0x0D,
	0x0E, //Undefined
	0x0F, //Undefined
	0x10,
	0x11,
	0x12,
	0x13,
	0x14,
	0x15,
	0x16, //IME On
	0x17,
	0x18,
	0x19,
	0x1A, //IME Off
	0x1B,
	0x1C,
	0x1D,
	0x1E,
	0x1F,
	0x20,
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x26,
	0x27,
	0x28,
	0x29,
	0x2A,
	0x2B,
	0x2C,
	0x2D,
	0x2E,
	0x2F,
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0x3A, //Undefined
	0x3B, //Undefined
	0x3C, //Undefined
	0x3D, //Undefined
	0x3E, //Undefined
	0x3F, //Undefined
	0x40, //Undefined
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4B,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,
	0x5B,
	0x5C,
	0x5D,
	0x5E, //Rservered
	0x5F,
	0x60, //Numpad1
	0x61, //Numpad2
	0x62, //Numpad3
	0x63, //Numpad4
	0x64, //Numpad5
	0x65, //Numpad6
	0x66, //Numpad7
	0x67, //Numpad8
	0x68, //Numpad8
	0x69, //Numpad9
	0x6A,
	0x6B,
	0x6C,
	0x6D,
	0x6E,
	0x6F,
	0x70, //F1
	0x71, //F2
	0x72, //F3
	0x73, //F4
	0x74, //F5
	0x75, //F6
	0x76, //F7
	0x77, //F8
	0x78, //F9
	0x79, //F10
	0x7A, //F11
	0x7B, //F12
	0x7C, //F13
	0x7D, //F14
	0x7E, //F15
	0x7F, //F16
	0x80, //F17
	0x81, //F18
	0x82, //F19
	0x83, //F20
	0x84, //F21
	0x85, //F22
	0x86, //F23
	0x87, //F24
	0x88, //Unkown
	0x89, //Unkown
	0x8A, //Unkown
	0x8B, //Unkown
	0x8C, //Unkown
	0x8D, //Unkown
	0x8E, //Unkown
	0x8F, //Unkown
	0x90,
	0x91,
	0x92, //OEM Specific
	0x93, //OEM Specific
	0x94, //OEM Specific
	0x95, //OEM Specific
	0x96, //OEM Specific
	0x97, //Unkown
	0x98, //Unkown
	0x99, //Unkown
	0x9A, //Unkown
	0x9B, //Unkown
	0x9C, //Unkown
	0x9D, //Unkown
	0x9E, //Unkown 
	0x9F, //Unkown
	0xA0,
	0xA1,
	0xA2,
	0xA3,
	0xA4,
	0xA5
};

void ImGui::Hotkey(const char* label, int* k, bool& StateHolder, const ImVec2& size_arg)
{
	PushID(label);
	//static bool waitingforkey = false;
	static bool ignoreFirst = false;
	if (StateHolder == false)
	{
		if (Button(KeyNames[*k], size_arg))
		{
			StateHolder = true;
			ignoreFirst = true;
		}
	}
	else if (StateHolder == true)
	{
		Button("ESC To Cancel", size_arg);
		for (auto& Key : KeyCodes)
		{
			if (GetAsyncKeyState(Key) & 1)
			{
				if (Key == VK_ESCAPE || Key == *k)
					StateHolder = false;
				else
				{
					if (ignoreFirst)
						ignoreFirst = false;
					else
					{
						*k = Key;
						StateHolder = false;
					}
				}
			}
		}
	}
	PopID();
}


LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(cheat::MenuKey) & 1)
	{
		ImGuiIO& io = ImGui::GetIO();
		gui::open = !gui::open;
		if (gui::open)
			io.MouseDrawCursor = true;
		else
			io.MouseDrawCursor = false;
	}
	if (gui::open)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return 1L;
	}
	return CallWindowProc(gui::originalWindowProcess, hWnd, msg, wParam, lParam);
}
