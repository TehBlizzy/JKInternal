#pragma once
#include "windows.h"
#include "winuser.h"
#include "../external/nlohmann/json.hpp"
#include "jka.h"


#define	PITCH	0		// up / down
#define	YAW		1		// left / right
#define	ROLL	2		// fall over
#define M_PI	3.14159265358979323846f
#define VectorCopy(a,b)			((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])
#define	VectorScale(v, s, o)	((o)[0]=(v)[0]*(s),(o)[1]=(v)[1]*(s),(o)[2]=(v)[2]*(s))
#define	VectorMA(v, s, b, o)			((o)[0]=(v)[0]+(b)[0]*(s),(o)[1]=(v)[1]+(b)[1]*(s),(o)[2]=(v)[2]+(b)[2]*(s))

#define WEAPON_UNIVERSAL_SPEED 5500 //current value in g_ShotSpeed
#define FAST_WEAPON_UNIVERSAL_SPEED 4000 //current value in g_FastShotSpeed
//#define MAX_GENTITIES			32

typedef struct
{
	int* LocalClientNumber;
	refdef_t* RefDef;
	gametype_t* GameType;
	float* ScreenXScale;
	float* ScreenYScale;
	clientInfo_t* OJK_ClientInfo;
	mb2_clientInfo_t* MB2_ClientInfo;
	float* ViewAngles;
	centity_t* Entities[MAX_GENTITIES];
	refEntity_t* RefEntities[MAX_GENTITIES];
	playerState_t* PlayerState;

}client;

#define M_PI					3.14159265358979323846f

inline vec4_t		colorBlack = { 0, 0, 0, 1 };
inline vec4_t		colorRed = { 1, 0, 0, 1 };
inline vec4_t		colorGreen = { 0, 1, 0, 1 };
inline vec4_t		colorBlue = { 0, 0, 1, 1 };
inline vec4_t		colorYellow = { 1, 1, 0, 1 };
inline vec4_t		colorMagenta = { 1, 0, 1, 1 };
inline vec4_t		colorCyan = { 0, 1, 1, 1 };
inline vec4_t		colorWhite = { 1, 1, 1, 1 };
inline vec4_t		colorLtGrey = { 0.75, 0.75, 0.75, 1 };
inline vec4_t		colorMdGrey = { 0.5, 0.5, 0.5, 1 };
inline vec4_t		colorDkGrey = { 0.25, 0.25, 0.25, 1 };

inline vec4_t		colorLtBlue = { 0.367f, 0.261f, 0.722f, 1 };
inline vec4_t		colorDkBlue = { 0.199f, 0.0f,   0.398f, 1 };
inline vec3_t		vec3_origin = { 0,0,0 };
inline vec3_t		axisDefault[3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

namespace cheat
{
	inline client cl;
	inline int lastEntityIndex;
	//inline centity_t* LocalPlayer;
	//inline playerState_t* LocalPlayerState;
	//qhandle_t shaderWhite = NULL;

	/*
		Enable ESP
			Box ESP
			Name ESP

		Enable Wallhack
			Enable Glow Shader
				Choose shader for ally/enemy?

		Enable Aimbot
			Set keybind
			Set toggle keybind
			Angle distance

		Enable Triggerbot
			Set toggle keybind

		Enable Radar
			Background alpha

	*/

	inline int MenuKey = VK_INSERT;

	inline bool EnableESP = true;
	inline bool EnableBoxESP = true;
	inline bool EnableNameESP = true;

	inline bool EnableWallhack = true;
	inline bool EnableWHShader = true;
	inline std::string WHAllyShaderName = "gfx/effects/burn";
	inline std::string WHEnemyShaderName = "gfx/misc/forceprotect";
	inline int32_t WHAllyShader = -1;
	inline int32_t WHEnemyShader = -1;
	inline int32_t WhiteShader = 7;

	inline bool EnableAimbot = true;
	inline int AimbotHoldKey = VK_LSHIFT;
	inline int AimbotToggleKey = 0x47; // G
	inline float AimbotMaxAngle = 20.0;
	inline bool AimbotToggle = false;
	inline bool isHoldingAimKey = false;

	inline bool EnableTriggerBot = false;
	inline int TriggerbotHoldKey = VK_LCONTROL;
	inline int TriggerbotToggleKey = 0x48; // H

	inline bool EnableRadar = true;
	inline vec4_t RadarBGColor = { 0.0, 0.0, 0.0, 0.5 };
	inline int RadarPosX = 100;
	inline int RadarPosY = 100;
	inline int RadarSize = 70;

	using Config = nlohmann::json;
	inline std::string ConfigName = "jkiconfig.json";

	inline bool createDefaultConfig = true;

	void SaveConfig();
	void LoadConfig();

	void Init() noexcept;
	void CG_Init() noexcept;
	void CG_DrawActiveFrame() noexcept;
	void DrawESP() noexcept;
	void UpdateShader();
	int IsEntityPlayer();
	bool IsSameTeam(centity_t* ent);
	void DoAimbot();
	void DoRadar();
	bool GetRadarScreenPosition(vec3_t pos, int& screenX, int& screenY);
	inline centity_t* GetClosestTarget();
	inline centity_t* AimTarget = NULL;
	inline centity_t* LockTarget = NULL;
	inline float aX = 0;
	inline float aY = 0;

	void VectorToViewangles(float* fEntTarg, float& fAngleX, float& fAngleY);
	void VectorAngles(const vec3_t forward, vec3_t angles);
	void AnglesToAxis(const vec3_t angles, vec3_t axis[3]);
	bool WorldToScreen(vec3_t worldCoord, float* x, float* y, float* z);
	bool IsEntityVisible(float* pos) noexcept;
	/*void R_DrawRect(float x, float y, float width, float height, float size, const float* color);
	void R_FillRect(float x, float y, float width, float height, const float* color);
	void R_DrawSides(float x, float y, float w, float h, float size);
	void R_DrawTopBottom(float x, float y, float w, float h, float size);
	void R_SetColor(const float* rgba);
	void R_DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader);
	void R_DrawText(float x, float y, const char* str, vec4_t color);
	*/
	void VectorSubtract(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut);
	void VectorAdd(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut);
	float DotProduct(const vec3_t vec1, const vec3_t vec2);
	void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
	void MakeVector(const vec3_t ain, vec3_t vout);
	void VectorRotateX(const vec3_t in, float angle, vec3_t out);
	void VectorRotateY(const vec3_t in, float angle, vec3_t out);
	void VectorRotateZ(const vec3_t in, float angle, vec3_t out);
	float VectorLength(vec3_t vec);
	float AngleNormalize180(float angle);
	float AngleNormalize360(float angle);
	void VecToAngles(const vec3_t value1, vec3_t angles);
	float VectorNormalize(vec3_t vec);
}