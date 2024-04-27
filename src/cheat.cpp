#pragma once
#include "jka.h"
#include <fstream>
#include <cmath>


void cheat::Init() noexcept
{
	cl = {};
	LoadConfig();
}

void cheat::LoadConfig()
{
	std::ifstream f(ConfigName);
	if (!f.fail())
	{
		Config data = Config::parse(f);
		if (data.size() != 0)
		{
			createDefaultConfig = false;
			MenuKey = data["MenuKey"];

			EnableESP = data["ESP"]["EnableESP"];
			EnableBoxESP = data["ESP"]["EnableBox"];
			EnableNameESP = data["ESP"]["EnableNames"];

			EnableWallhack = data["Wallhack"]["EnableWH"];
			EnableWallhack = data["Wallhack"]["EnableShader"];
			WHAllyShaderName = data["Wallhack"]["AllyShader"];
			WHEnemyShaderName = data["Wallhack"]["EnemyShader"];

			EnableAimbot = data["Aimbot"]["EnableAim"];
			AimbotHoldKey = data["Aimbot"]["AimbotHoldKey"];
			AimbotToggleKey = data["Aimbot"]["AimbotToggleKey"];
			AimbotMaxAngle = data["Aimbot"]["AimbotAngle"];

			EnableTriggerBot = data["Triggerbot"]["EnableTBot"];
			TriggerbotHoldKey = data["Triggerbot"]["TriggerbotHoldKey"];
			TriggerbotToggleKey = data["Triggerbot"]["TriggerbotToggleKey"];

			EnableRadar = data["Radar"]["EnableRadar"];
			RadarBGColor[0] = data["Radar"]["RadarBGColor"]["R"];
			RadarBGColor[1] = data["Radar"]["RadarBGColor"]["G"];
			RadarBGColor[2] = data["Radar"]["RadarBGColor"]["B"];
			RadarBGColor[3] = data["Radar"]["RadarBGColor"]["A"];
			RadarSize = data["Radar"]["RadarSize"];
			RadarPosX = data["Radar"]["RadarPositionX"];
			RadarPosY = data["Radar"]["RadarPositionY"];
		}
		f.close();
	}
}

void cheat::SaveConfig()
{
	Config c = {};
	c["MenuKey"] = MenuKey;
	c["ESP"]["EnableESP"] = EnableESP;
	c["ESP"]["EnableBox"] = EnableBoxESP;
	c["ESP"]["EnableNames"] = EnableNameESP;
	c["Wallhack"]["EnableWH"] = EnableWallhack;
	c["Wallhack"]["EnableShader"] = EnableWHShader;
	c["Wallhack"]["AllyShader"] = WHAllyShaderName;
	c["Wallhack"]["EnemyShader"] = WHEnemyShaderName;
	c["Aimbot"]["EnableAim"] = EnableAimbot;
	c["Aimbot"]["AimbotHoldKey"] = AimbotHoldKey;
	c["Aimbot"]["AimbotToggleKey"] = AimbotToggleKey;
	c["Aimbot"]["AimbotAngle"] = AimbotMaxAngle;
	c["Triggerbot"]["EnableTBot"] = EnableTriggerBot;
	c["Triggerbot"]["TriggerbotHoldKey"] = TriggerbotHoldKey;
	c["Triggerbot"]["TriggerbotToggleKey"] = TriggerbotToggleKey;
	c["Radar"]["EnableRadar"] = EnableRadar;
	c["Radar"]["RadarBGColor"]["R"] = RadarBGColor[0];
	c["Radar"]["RadarBGColor"]["G"] = RadarBGColor[1];
	c["Radar"]["RadarBGColor"]["B"] = RadarBGColor[2];
	c["Radar"]["RadarBGColor"]["A"] = RadarBGColor[3];
	c["Radar"]["RadarSize"] = RadarSize;
	c["Radar"]["RadarPositionX"] = RadarPosX;
	c["Radar"]["RadarPositionY"] = RadarPosY;

	std::string final = c.dump(4);
	std::ofstream o(ConfigName);
	o << final << std::endl;
	o.close();
}

void cheat::CG_Init() noexcept
{
	if (createDefaultConfig)
	{
		WHAllyShaderName = "gfx/effects/blaster_blob";
		WHEnemyShaderName = "gfx/misc/forceprotect";
		SaveConfig();
	}
	UpdateShader();
}

void cheat::UpdateShader()
{
	if (!hooks::isLegacy)
	{
		if (hooks::CGI)
		{
			WHAllyShader = hooks::CGI->R_RegisterShader(WHAllyShaderName.c_str());
			WHEnemyShader = hooks::CGI->R_RegisterShader(WHEnemyShaderName.c_str());
		}
	}
	else
	{
		WHAllyShader = hooks::SysCallOriginal(CG_R_REGISTERSHADER, WHAllyShaderName.c_str());
		WHEnemyShader = hooks::SysCallOriginal(CG_R_REGISTERSHADER, WHEnemyShaderName.c_str());
	}
}

void cheat::CG_DrawActiveFrame() noexcept
{
	if (!cl.LocalClientNumber || !cl.RefDef || !cl.PlayerState)
	{
		gui::R_End();
		hooks::IS_SHUTDOWN = true;
		return;
	}
	hooks::IS_SHUTDOWN = false;
	if (EnableESP)
		DrawESP();

	if (EnableWallhack)
		cl.RefDef->rdflags |= RDF_ForceSightOn;

	if (EnableAimbot)
		DoAimbot();

	if (EnableRadar)
		DoRadar();

	gui::R_End();

	hooks::SetCvar2Original("sv_cheats", "1", 0, qtrue);
}


void cheat::DoRadar()
{
	// Frame
	//R_FillRect(RadarPosX - RadarSize, RadarPosY - RadarSize, RadarSize * 2, RadarSize * 2, RadarBGColor);
	gui::R_DrawFillRect(RadarPosX, RadarPosY, RadarPosX + RadarSize, RadarPosY + RadarSize, RadarBGColor);

	//R_FillRect(RadarPosX - RadarSize, RadarPosY - 1, RadarSize * 2, 2, colorBlack);
	// left right bar
	gui::R_DrawFillRect(RadarPosX, RadarPosY + (RadarSize / 2) - 1, RadarPosX + RadarSize,
	                    RadarPosY + (RadarSize / 2) + 1, colorBlack);

	//R_FillRect(RadarPosX - 1, RadarPosY - RadarSize, 2, RadarSize * 2, colorBlack);
	gui::R_DrawFillRect(RadarPosX + (RadarSize / 2) - 1, RadarPosY, RadarPosX + (RadarSize / 2) + 1,
	                    RadarPosY + RadarSize, colorBlack);

	//R_DrawRect(RadarPosX - RadarSize, RadarPosY - RadarSize, RadarSize * 2, RadarSize * 2, 1, colorBlack);
	gui::R_DrawRect(RadarPosX - 1, RadarPosY - 1, RadarPosX + RadarSize + 1, RadarPosY + RadarSize + 1, colorBlack);

	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (cl.Entities[i] && cl.Entities[i]->currentValid && !(cl.Entities[i]->currentState.eFlags & EF_DEAD))
		{
			if ((*cl.GameType < GT_SINGLE_PLAYER) || (*cl.GameType > GT_SINGLE_PLAYER && !IsSameTeam(cl.Entities[i])))
			{
				int ScreenX, ScreenY;
				if (GetRadarScreenPosition(cl.Entities[i]->currentState.pos.trBase, ScreenX, ScreenY))
				{
					vec4_t rectColor = {1, 0, 0, 1};
					if (!IsEntityVisible(cl.Entities[i]->currentState.pos.trBase))
					{
						rectColor[0] = 1;
						rectColor[1] = 1;
						rectColor[2] = 0;
						rectColor[3] = 1;
					}
					gui::R_DrawFillRect(ScreenX, ScreenY, ScreenX + 4, ScreenY + 4, rectColor);
				}
			}
		}
	}
}


bool cheat::GetRadarScreenPosition(vec3_t origin, int& screenx, int& screeny)
{
	vec3_t vec, rot;
	float distance;
	VectorSubtract(origin, cl.RefDef->vieworg, vec);
	vec[2] = 0.0f;
	distance = VectorLength(vec) * 0.025;
	VecToAngles(vec, rot);
	rot[1] = AngleNormalize180(rot[1] - cl.RefDef->viewangles[1]);
	AngleVectors(rot, vec, nullptr, nullptr);
	VectorScale(vec, distance, vec);

	if (vec[0] > RadarSize)
		VectorScale(vec, RadarSize / vec[0], vec);
	else if (vec[0] < -RadarSize)
		VectorScale(vec, -RadarSize / vec[0], vec);
	if (vec[1] > RadarSize)
		VectorScale(vec, RadarSize / vec[1], vec);
	else if (vec[1] < -RadarSize)
		VectorScale(vec, -RadarSize / vec[1], vec);

	if ((static_cast<int>(-vec[1]) > RadarSize) || (static_cast<int>(-vec[0]) > RadarSize)) return false;
	if ((static_cast<int>(-vec[1]) < -RadarSize) || (static_cast<int>(-vec[0]) < -RadarSize)) return false;
	screenx = static_cast<int>(-vec[1]) + RadarPosX + (RadarSize / 2);
	screeny = static_cast<int>(-vec[0]) + RadarPosY + (RadarSize / 2);
	return true;
}

void cheat::DoAimbot()
{
	if (GetAsyncKeyState(AimbotToggleKey) & 1)
		AimbotToggle = !AimbotToggle;
	if (AimbotToggle || GetAsyncKeyState(AimbotHoldKey))
	{
		if (!cl.PlayerState)
			return;
		AimTarget = nullptr;
		if (isHoldingAimKey && LockTarget)
		{
			if (LockTarget->currentValid && ~LockTarget->currentState.eFlags & EF_DEAD)
			{
				if (!hooks::IsMB2)
					if (IsEntityVisible(LockTarget->lerpOrigin))
						AimTarget = LockTarget;
					else
						LockTarget = nullptr;
				else if (IsEntityVisible(LockTarget->mb2_lerpOrigin))
					AimTarget = LockTarget;
				else
					LockTarget = nullptr;
			}
			else
				LockTarget = nullptr;
		}
		else
		{
			AimTarget = GetClosestTarget();
			if (AimTarget)
				LockTarget = AimTarget;
		}
		if (!AimTarget)
			return;

		vec3_t playerDifference, playerPredictedPosition;
		float distance, eta;
		float projSpeed = 7000;
		if (!hooks::IsMB2)
			VectorSubtract(AimTarget->lerpOrigin, cl.PlayerState->origin, playerDifference);
		else
			VectorSubtract(AimTarget->mb2_lerpOrigin, cl.PlayerState->origin, playerDifference);
		distance = VectorNormalize(playerDifference);
		if (cl.PlayerState->weapon == WP_BRYAR_PISTOL || cl.PlayerState->weapon == WP_BLASTER || cl.PlayerState->weapon
			== WP_DEMP2 || cl.PlayerState->weapon == WP_FLECHETTE)
			projSpeed = WEAPON_UNIVERSAL_SPEED;
		else if (cl.PlayerState->weapon == WP_ROCKET_LAUNCHER)
			projSpeed = 1400;
		else if (cl.PlayerState->weapon == WP_CONCUSSION)
			projSpeed = 3500;
		else if (cl.PlayerState->weapon == WP_REPEATER || cl.PlayerState->weapon == WP_BOWCASTER)
			projSpeed = FAST_WEAPON_UNIVERSAL_SPEED;

		eta = (distance / projSpeed);
		if (!hooks::IsMB2)
			VectorMA(AimTarget->lerpOrigin, eta, AimTarget->currentState.pos.trDelta, playerPredictedPosition);
		else
			VectorMA(AimTarget->mb2_lerpOrigin, eta, AimTarget->currentState.pos.trDelta, playerPredictedPosition);
		playerPredictedPosition[2] += 8.0;
		VectorToViewangles(playerPredictedPosition, aX, aY);
		cl.ViewAngles[1] += aX;
		cl.ViewAngles[0] += aY;

		isHoldingAimKey = true;
	}
	if (!GetAsyncKeyState(AimbotHoldKey))
	{
		isHoldingAimKey = false;
		LockTarget = nullptr;
	}
}

void cheat::AnglesToAxis(const vec3_t angles, vec3_t axis[3])
{
	vec3_t right;
	AngleVectors(angles, axis[0], right, axis[2]);
	VectorSubtract(vec3_origin, right, axis[1]);
}

void cheat::VectorAngles(const vec3_t forward, vec3_t angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

void cheat::VectorToViewangles(float* fEntTarg, float& fAngleX, float& fAngleY)
{
	vec3_t vecEntity, vecAngle;
	VectorSubtract(fEntTarg, cl.RefDef->vieworg, vecEntity);
	vecEntity[2] += 20;
	VectorAngles(vecEntity, vecAngle);
	vecAngle[PITCH] = -vecAngle[PITCH];
	if (vecAngle[YAW] > 180.0f)
		vecAngle[YAW] -= 360.0f;
	else if (vecAngle[YAW] < -180.0f)
		vecAngle[YAW] += 360.0f;
	if (vecAngle[PITCH] > 180.0f)
		vecAngle[PITCH] -= 360.0f;
	else if (vecAngle[PITCH] < -180.0f)
		vecAngle[PITCH] += 360.0f;
	AnglesToAxis(vecAngle, cl.RefDef->viewaxis);
	vecAngle[YAW] -= cl.RefDef->viewangles[YAW];
	if (vecAngle[YAW] > 180.0f)
		vecAngle[YAW] -= 360.0f;
	else if (vecAngle[YAW] < -180.0f)
		vecAngle[YAW] += 360.0f;
	vecAngle[PITCH] -= cl.RefDef->viewangles[PITCH];
	if (vecAngle[PITCH] > 180.0f)
		vecAngle[PITCH] -= 360.0f;
	else if (vecAngle[PITCH] < -180.0f)
		vecAngle[PITCH] -= 360.0f;
	fAngleX = vecAngle[YAW];
	fAngleY = vecAngle[PITCH];
}

centity_t* cheat::GetClosestTarget()
{
	float ScreenX = cl.RefDef->width / 2.0;
	float ScreenY = cl.RefDef->height / 2.0;
	float PosX, PosY = 0.0f;
	float Nearest = static_cast<float>(INT_MAX);
	float Screen[3];

	centity_t* pRet = nullptr;
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		centity_t* pPlayer = cl.Entities[i];
		if (!pPlayer)
			continue;
		if (!pPlayer->currentValid || cl.Entities[i]->currentState.eFlags & EF_DEAD)
			continue;
		if (pPlayer->currentState.clientNum == *cl.LocalClientNumber)
			continue;
		if (IsSameTeam(pPlayer))
			continue;
		if (!hooks::IsMB2)
		{
			if (!WorldToScreen(cl.Entities[i]->lerpOrigin, &Screen[0], &Screen[1], &Screen[2]))
				continue;
			if (!IsEntityVisible(cl.Entities[i]->lerpOrigin))
				continue;
		}
		else
		{
			if (!WorldToScreen(cl.Entities[i]->mb2_lerpOrigin, &Screen[0], &Screen[1], &Screen[2]))
				continue;
			if (!IsEntityVisible(cl.Entities[i]->mb2_lerpOrigin))
				continue;
		}

		PosX = Screen[0] > ScreenX ? Screen[0] - ScreenX : ScreenX - Screen[0];
		PosY = Screen[1] > ScreenY ? Screen[1] - ScreenY : ScreenY - Screen[1];
		float Temp = sqrt(PosX * PosX + PosY * PosY);

		if (Temp > Nearest || Temp > AimbotMaxAngle)
			continue;
		pRet = pPlayer;
		Nearest = Temp;
	}
	return pRet;
}

bool cheat::IsSameTeam(centity_t* ent)
{
	if (!hooks::IsMB2)
	{
		if (!cl.OJK_ClientInfo[ent->currentState.clientNum].infoValid)
			return false;
		if (cl.OJK_ClientInfo[ent->currentState.clientNum].team == cl.PlayerState->persistant[PERS_TEAM])
			return true;
	}
	else
	{
		if (!cl.MB2_ClientInfo[ent->currentState.clientNum].infoValid)
			return false;
		if (cl.MB2_ClientInfo[ent->currentState.clientNum].team == cl.PlayerState->persistant[PERS_TEAM])
			return true;
	}

	return false;
}

void cheat::DrawESP() noexcept
{
	float Screen[3];
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (cl.Entities[i] && cl.Entities[i]->currentValid && !(cl.Entities[i]->currentState.eFlags & EF_DEAD))
		{
			bool onScreen = false;
			if (!hooks::IsMB2)
			{
				onScreen = WorldToScreen(cl.Entities[i]->lerpOrigin, &Screen[0], &Screen[1], &Screen[2]);
			}
			else
			{
				onScreen = WorldToScreen(cl.Entities[i]->mb2_lerpOrigin, &Screen[0], &Screen[1], &Screen[2]);
			}
			if (onScreen)
			{
				// 1400 = 1, .001 = 3
				if (Screen[2] > 1400.0)
					Screen[2] = 1400.0f;
				float scale = 5.0 + ((Screen[2] - 0.001) / (1400.0 - 0.001)) * (0.5 - 5.0);
				bool IsVisible = false;
				if (!hooks::IsMB2)
				{
					IsVisible = IsEntityVisible(cl.Entities[i]->lerpOrigin);
				}
				else
				{
					IsVisible = IsEntityVisible(cl.Entities[i]->mb2_lerpOrigin);
				}
				if (*cl.GameType > GT_SINGLE_PLAYER)
				{
					bool SameTeam = IsSameTeam(cl.Entities[i]);
					// Same team
					if (IsVisible)
					{
						if (EnableNameESP)
						{
							//float offset = cl.Entities[i]->playerState->standheight;
							if (SameTeam)
							{
								//R_DrawText(Screen[0], Screen[1] + offset, CleanName, colorGreen);
								if (!hooks::IsMB2)
									gui::R_DrawText(Screen[0], Screen[1], cl.OJK_ClientInfo[i].cleanname, colorGreen);
								else
									gui::R_DrawText(Screen[0], Screen[1], cl.MB2_ClientInfo[i].name, colorGreen);
							}
							else
							{
								//R_DrawText(Screen[0], Screen[1] + offset, CleanName, colorRed);
								if (!hooks::IsMB2)
									gui::R_DrawText(Screen[0], Screen[1], cl.OJK_ClientInfo[i].cleanname, colorRed);
								else
									gui::R_DrawText(Screen[0], Screen[1], cl.MB2_ClientInfo[i].name, colorRed);
							}
						}
						if (EnableBoxESP)
						{
							if (SameTeam)
								//R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, 1, colorGreen);
								gui::R_DrawRect(Screen[0] - (10 * scale), Screen[1] - (20 * scale),
								                Screen[0] + (10 * scale), Screen[1] + (20 * scale), colorGreen);
							else
								//R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, 1, colorRed);
								gui::R_DrawRect(Screen[0] - (10 * scale), Screen[1] - (20 * scale),
								                Screen[0] + (10 * scale), Screen[1] + (20 * scale), colorRed);
						}
					}
					else
					{
						if (EnableNameESP)
						{
							if (EnableNameESP)
							{
								if (SameTeam)
								{
									//R_DrawText(Screen[0], Screen[1] + offset, CleanName, colorGreen);
									if (!hooks::IsMB2)
										gui::R_DrawText(Screen[0], Screen[1], cl.OJK_ClientInfo[i].cleanname,
										                colorCyan);
									else
										gui::R_DrawText(Screen[0], Screen[1], cl.MB2_ClientInfo[i].name, colorCyan);
								}
								else
								{
									//R_DrawText(Screen[0], Screen[1] + offset, CleanName, colorRed);
									if (!hooks::IsMB2)
										gui::R_DrawText(Screen[0], Screen[1], cl.OJK_ClientInfo[i].cleanname,
										                colorYellow);
									else
										gui::R_DrawText(Screen[0], Screen[1], cl.MB2_ClientInfo[i].name, colorYellow);
								}
							}
						}
						if (EnableBoxESP)
						{
							if (SameTeam)
								//R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, 1, colorCyan);
								gui::R_DrawRect(Screen[0] - (10 * scale), Screen[1] - (20 * scale),
								                Screen[0] + (10 * scale), Screen[1] + (20 * scale), colorCyan);
							else
								//R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, 1, colorYellow);
								gui::R_DrawRect(Screen[0] - (10 * scale), Screen[1] - (20 * scale),
								                Screen[0] + (10 * scale), Screen[1] + (20 * scale), colorYellow);
						}
					}
				}
				else
				{
					if (IsVisible)
					{
						if (EnableNameESP)
						{
							//gui::R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, colorRed);
						}
						if (EnableBoxESP)
						{
							//R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, 1, colorRed);
							gui::R_DrawRect(Screen[0] - (10 * scale), Screen[1] - (20 * scale),
							                Screen[0] + (10 * scale), Screen[1] + (20 * scale), colorRed);
						}
					}
					else
					{
						if (EnableNameESP)
						{
							//gui::R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, colorYellow);
						}
						if (EnableBoxESP)
						{
							//R_DrawRect(Screen[0] - 10, Screen[1] - 20, 20, 40, 1, colorYellow);
							gui::R_DrawRect(Screen[0] - (10 * scale), Screen[1] - (20 * scale),
							                Screen[0] + (10 * scale), Screen[1] + (20 * scale), colorYellow);
						}
					}
				}
			}
		}
	}
}

int cheat::IsEntityPlayer()
{
	if (lastEntityIndex > MAX_GENTITIES)
	{
		return 0;
	}

	if (!cl.Entities || !cl.Entities[lastEntityIndex])
		return 0;

	if (cl.Entities[lastEntityIndex]->currentState.clientNum > MAX_CLIENTS)
	{
		return 0;
	}

	if (cl.Entities[lastEntityIndex]->currentState.eType != ET_PLAYER
		|| cl.Entities[lastEntityIndex]->currentState.eFlags & EF_DEAD)
	{
		return 0;
	}

	if (cl.Entities[lastEntityIndex]->currentState.clientNum == *cl.LocalClientNumber)
	{
		return 2;
	}

	return 1;
}


/*void cheat::R_DrawRect(float x, float y, float width, float height, float size, const float* color) {
	R_SetColor(color);
	R_DrawTopBottom(x, y, width, height, size);
	R_DrawSides(x, y, width, height, size);
	R_SetColor(NULL);
}
void cheat::R_FillRect(float x, float y, float width, float height, const float* color) {
	R_SetColor(color);
	R_DrawStretchPic(x, y, width, height, 0, 0, 0, 1, 7);//cl.cgs->media.whiteShader);
	R_SetColor(NULL);
}
void cheat::R_DrawSides(float x, float y, float w, float h, float size) {
	size *= *cl.ScreenXScale;
	R_DrawStretchPic(x, y, size, h, 0, 0, 0, 0, 7);//cl.cgs->media.whiteShader);
	R_DrawStretchPic(x + w - size, y, size, h, 0, 0, 0, 0, 7);//cl.cgs->media.whiteShader);
}

void cheat::R_DrawTopBottom(float x, float y, float w, float h, float size) {
	size *= *cl.ScreenYScale;
	R_DrawStretchPic(x, y, w, size, 0, 0, 0, 0, 7);//cl.cgs->media.whiteShader);
	R_DrawStretchPic(x, y + h - size, w, size, 0, 0, 0, 0, 7);//cl.cgs->media.whiteShader);
}

void cheat::R_SetColor(const float* rgba)
{
	if (!hooks::isLegacy)
		hooks::CGI->R_SetColor(rgba);
	else
		hooks::SysCallOriginal(CG_R_SETCOLOR, rgba);
}

void cheat::R_DrawStretchPic(float x, float y, float w, float h,
	float s1, float t1, float s2, float t2, qhandle_t hShader) {
	if (!hooks::isLegacy)
		hooks::CGI->R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, hShader);
	else
		hooks::SysCallOriginal(CG_R_DRAWSTRETCHPIC, x, y, w, h, s1, t1, s2, t2, hShader);
}

void cheat::R_DrawText(float x, float y, const char* str, vec4_t color)
{
	if (!hooks::isLegacy)
	{
		int size = hooks::CGI->R_Font_StrLenPixels(str, FONT_MEDIUM, 0.7);
		hooks::CGI->R_Font_DrawString(x - (size / 2), y, str, color, FONT_MEDIUM, -1, 0.7);
	}
	else
	{
		int size = hooks::SysCallOriginal(CG_R_FONT_STRLENPIXELS, str, FONT_MEDIUM, 0.7);
		hooks::SysCallOriginal(CG_R_FONT_DRAWSTRING, x - (size / 2), y, str, color, FONT_MEDIUM, -1, 0.7);
	}
}*/

bool cheat::IsEntityVisible(float* pos) noexcept
{
	trace_t t;

	if (!hooks::isLegacy)
		hooks::CGI->CM_Trace(&t, cl.RefDef->vieworg, pos, nullptr, nullptr, 0, MASK_SHOT & ~CONTENTS_BODY, 0);
	else
		hooks::SysCallOriginal(CG_CM_BOXTRACE, &t, cl.RefDef->vieworg, pos, NULL, NULL, 0, MASK_SHOT & ~CONTENTS_BODY,
		                       0);

	return (t.fraction == 1.0f);
}

bool cheat::WorldToScreen(vec3_t worldCoord, float* x, float* y, float* z)
{
	vec3_t trans;
	float xc, yc;
	float px, py;
	float zi;

	px = tan(cl.RefDef->fov_x * (M_PI / 360));
	py = tan(cl.RefDef->fov_y * (M_PI / 360));

	VectorSubtract(worldCoord, cl.RefDef->vieworg, trans);

	xc = cl.RefDef->width / 2.0;
	yc = cl.RefDef->height / 2.0;

	// z = how far is the object in our forward direction
	zi = DotProduct(trans, cl.RefDef->viewaxis[0]);
	if (zi <= 0.001)
		return false;

	*x = xc - DotProduct(trans, cl.RefDef->viewaxis[1]) * xc / (zi * px);
	*y = yc - DotProduct(trans, cl.RefDef->viewaxis[2]) * yc / (zi * py);
	*z = zi;

	return true;
}

void cheat::VectorAdd(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut)
{
	vecOut[0] = vec1[0] + vec2[0];
	vecOut[1] = vec1[1] + vec2[1];
	vecOut[2] = vec1[2] + vec2[2];
}

void cheat::VectorSubtract(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut)
{
	vecOut[0] = vec1[0] - vec2[0];
	vecOut[1] = vec1[1] - vec2[1];
	vecOut[2] = vec1[2] - vec2[2];
}

float cheat::DotProduct(const vec3_t vec1, const vec3_t vec2)
{
	return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];
}

void cheat::AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up)
{
	float angle;
	static float sr, sp, sy, cr, cp, cy;
	// static to help MS compiler fp bugs

	angle = angles[YAW] * (M_PI * 2 / 360);
	sy = sinf(angle);
	cy = cosf(angle);
	angle = angles[PITCH] * (M_PI * 2 / 360);
	sp = sinf(angle);
	cp = cosf(angle);
	angle = angles[ROLL] * (M_PI * 2 / 360);
	sr = sinf(angle);
	cr = cosf(angle);

	if (forward)
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	if (right)
	{
		right[0] = (-1 * sr * sp * cy + -1 * cr * -sy);
		right[1] = (-1 * sr * sp * sy + -1 * cr * cy);
		right[2] = -1 * sr * cp;
	}
	if (up)
	{
		up[0] = (cr * sp * cy + -sr * -sy);
		up[1] = (cr * sp * sy + -sr * cy);
		up[2] = cr * cp;
	}
}

float cheat::VectorLength(vec3_t v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void cheat::MakeVector(const vec3_t ain, vec3_t vout)
{
	float pitch;
	float yaw;
	float tmp;

	pitch = ain[0] * M_PI / 180;
	yaw = ain[1] * M_PI / 180;
	tmp = cos(pitch);

	vout[0] = -tmp * -cos(yaw);
	vout[1] = sin(yaw) * tmp;
	vout[2] = -sin(pitch);
}

float cheat::AngleNormalize180(float angle)
{
	angle = AngleNormalize360(angle);
	if (angle > 180.0)
	{
		angle -= 360.0;
	}
	return angle;
}

float cheat::AngleNormalize360(float angle)
{
	return (360.0 / 65536) * (static_cast<int>(angle * (65536 / 360.0)) & 65535);
}

void cheat::VecToAngles(const vec3_t value1, vec3_t angles)
{
	float forward;
	float yaw, pitch;

	if (value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if (value1[2] > 0)
		{
			pitch = 90;
		}
		else
		{
			pitch = 270;
		}
	}
	else
	{
		if (value1[0])
		{
			yaw = (atan2(value1[1], value1[0]) * 180 / M_PI);
		}
		else if (value1[1] > 0)
		{
			yaw = 90;
		}
		else
		{
			yaw = 270;
		}
		if (yaw < 0)
		{
			yaw += 360;
		}

		forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
		pitch = (atan2(value1[2], forward) * 180 / M_PI);
		if (pitch < 0)
		{
			pitch += 360;
		}
	}

	angles[PITCH] = -pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0;
}

void cheat::VectorRotateX(const vec3_t in, float angle, vec3_t out)
{
	float a, c, s;

	a = angle * M_PI / 180;
	c = cos(a);
	s = sin(a);
	out[0] = in[0];
	out[1] = c * in[1] - s * in[2];
	out[2] = s * in[1] + c * in[2];
}

void cheat::VectorRotateY(const vec3_t in, float angle, vec3_t out)
{
	float a, c, s;

	a = angle * M_PI / 180;
	c = cos(a);
	s = sin(a);
	out[0] = c * in[0] + s * in[2];
	out[1] = in[1];
	out[2] = -s * in[0] + c * in[2];
}

void cheat::VectorRotateZ(const vec3_t in, float angle, vec3_t out)
{
	float a, c, s;

	a = angle * M_PI / 180;
	c = cos(a);
	s = sin(a);
	out[0] = c * in[0] - s * in[1];
	out[1] = s * in[0] + c * in[1];
	out[2] = in[2];
}

float cheat::VectorNormalize(vec3_t vec)
{
	float length, ilength;

	length = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
	length = sqrtf(length);

	if (length)
	{
		ilength = 1 / length;
		vec[0] *= ilength;
		vec[1] *= ilength;
		vec[2] *= ilength;
	}

	return length;
}
