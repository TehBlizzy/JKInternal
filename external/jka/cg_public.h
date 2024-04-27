/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#pragma once

#define	CGAME_API_VERSION		2

#define	CMD_BACKUP			64
#define	CMD_MASK			(CMD_BACKUP - 1)
// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP


#define	MAX_ENTITIES_IN_SNAPSHOT	256

// snapshots are a view of the server at a given time

// Snapshots are generated at regular time intervals by the server,
// but they may not be sent if a client's rate level is exceeded, or
// they may be dropped by the network.
typedef struct snapshot_s {
	int				snapFlags;			// SNAPFLAG_RATE_DELAYED, etc
	int				ping;

	int				serverTime;		// server time the message is valid for (in msec)

	byte			areamask[MAX_MAP_AREA_BYTES];		// portalarea visibility bits

	playerState_t	ps;						// complete information about the current player at this time
	playerState_t	vps; //vehicle I'm riding's playerstate (if applicable) -rww

	int				numEntities;			// all of the entities that need to be presented
	entityState_t	entities[MAX_ENTITIES_IN_SNAPSHOT];	// at the time of this snapshot

	int				numServerCommands;		// text based server commands to execute when this
	int				serverCommandSequence;	// snapshot becomes current
} snapshot_t;

typedef enum cgameEvent_e {
	CGAME_EVENT_NONE=0,
	CGAME_EVENT_TEAMMENU,
	CGAME_EVENT_SCOREBOARD,
	CGAME_EVENT_EDITHUD
} cgameEvent_t;

typedef struct autoMapInput_s {
	float		up;
	float		down;
	float		yaw;
	float		pitch;
	qboolean	goToDefaults;
} autoMapInput_t;

// CG_POINT_CONTENTS
typedef struct TCGPointContents_s {
	vec3_t		mPoint;			// input
	int			mPassEntityNum;	// input
} TCGPointContents;

// CG_GET_BOLT_POS
typedef struct TCGGetBoltData_s {
	vec3_t		mOrigin;		// output
	vec3_t		mAngles;		// output
	vec3_t		mScale;			// output
	int			mEntityNum;		// input
} TCGGetBoltData;

// CG_IMPACT_MARK
typedef struct TCGImpactMark_s {
	int		mHandle;
	vec3_t	mPoint;
	vec3_t	mAngle;
	float	mRotation;
	float	mRed;
	float	mGreen;
	float	mBlue;
	float	mAlphaStart;
	float	mSizeStart;
} TCGImpactMark;

// CG_GET_LERP_ORIGIN
// CG_GET_LERP_ANGLES
// CG_GET_MODEL_SCALE
typedef struct TCGVectorData_s {
	int			mEntityNum;		// input
	vec3_t		mPoint;			// output
} TCGVectorData;

// CG_TRACE/CG_G2TRACE
typedef struct TCGTrace_s {
	trace_t mResult;					// output
	vec3_t	mStart, mMins, mMaxs, mEnd;	// input
	int		mSkipNumber, mMask;			// input
} TCGTrace;

// CG_G2MARK
typedef struct TCGG2Mark_s {
	int			shader;
	float		size;
	vec3_t		start, dir;
} TCGG2Mark;

// CG_INCOMING_CONSOLE_COMMAND
typedef struct TCGIncomingConsoleCommand_s {
	char conCommand[1024];
} TCGIncomingConsoleCommand;

// CG_FX_CAMERASHAKE
typedef struct TCGCameraShake_s {
	vec3_t	mOrigin;					// input
	float	mIntensity;					// input
	int		mRadius;					// input
	int		mTime;						// input
} TCGCameraShake;

// CG_MISC_ENT
typedef struct TCGMiscEnt_s {
	char	mModel[MAX_QPATH];			// input
	vec3_t	mOrigin, mAngles, mScale;	// input
} TCGMiscEnt;

typedef struct {
	refEntity_t		ent;				// output
	void* ghoul2;			// input
	int				modelIndex;			// input
	int				boltIndex;			// input
	vec3_t			origin;				// input
	vec3_t			angles;				// input
	vec3_t			modelScale;			// input
} TCGPositionOnBolt;

//ragdoll callback structs -rww
#define RAG_CALLBACK_NONE				0
#define RAG_CALLBACK_DEBUGBOX			1
typedef struct ragCallbackDebugBox_s {
	vec3_t			mins;
	vec3_t			maxs;
	int				duration;
} ragCallbackDebugBox_t;

#define RAG_CALLBACK_DEBUGLINE			2
typedef struct ragCallbackDebugLine_s {
	vec3_t			start;
	vec3_t			end;
	int				time;
	int				color;
	int				radius;
} ragCallbackDebugLine_t;

#define RAG_CALLBACK_BONESNAP			3
typedef struct ragCallbackBoneSnap_s {
	char			boneName[128]; //name of the bone in question
	int				entNum; //index of entity who owns the bone in question
} ragCallbackBoneSnap_t;

#define RAG_CALLBACK_BONEIMPACT			4
typedef struct ragCallbackBoneImpact_s {
	char			boneName[128]; //name of the bone in question
	int				entNum; //index of entity who owns the bone in question
} ragCallbackBoneImpact_t;

#define RAG_CALLBACK_BONEINSOLID		5
typedef struct ragCallbackBoneInSolid_s {
	vec3_t			bonePos; //world coordinate position of the bone
	int				entNum; //index of entity who owns the bone in question
	int				solidCount; //higher the count, the longer we've been in solid (the worse off we are)
} ragCallbackBoneInSolid_t;

#define RAG_CALLBACK_TRACELINE			6
typedef struct ragCallbackTraceLine_s {
	trace_t			tr;
	vec3_t			start;
	vec3_t			end;
	vec3_t			mins;
	vec3_t			maxs;
	int				ignore;
	int				mask;
} ragCallbackTraceLine_t;

#define	MAX_CG_SHARED_BUFFER_SIZE		2048

typedef enum cgameImportLegacy_e {
	CG_PRINT = 0,
	CG_ERROR,
	CG_MILLISECONDS,
	CG_PRECISIONTIMER_START,
	CG_PRECISIONTIMER_END,
	CG_CVAR_REGISTER,
	CG_CVAR_UPDATE,
	CG_CVAR_SET,
	CG_CVAR_VARIABLESTRINGBUFFER,
	CG_CVAR_GETHIDDENVALUE,
	CG_ARGC,
	CG_ARGV,
	CG_ARGS,
	CG_FS_FOPENFILE,
	CG_FS_READ,
	CG_FS_WRITE,
	CG_FS_FCLOSEFILE,
	CG_FS_GETFILELIST,
	CG_SENDCONSOLECOMMAND,
	CG_ADDCOMMAND,
	CG_REMOVECOMMAND,
	CG_SENDCLIENTCOMMAND,
	CG_UPDATESCREEN,
	CG_CM_LOADMAP,
	CG_CM_NUMINLINEMODELS,
	CG_CM_INLINEMODEL,
	CG_CM_TEMPBOXMODEL,
	CG_CM_TEMPCAPSULEMODEL,
	CG_CM_POINTCONTENTS,
	CG_CM_TRANSFORMEDPOINTCONTENTS,
	CG_CM_BOXTRACE,
	CG_CM_CAPSULETRACE,
	CG_CM_TRANSFORMEDBOXTRACE,
	CG_CM_TRANSFORMEDCAPSULETRACE,
	CG_CM_MARKFRAGMENTS,
	CG_S_GETVOICEVOLUME,
	CG_S_MUTESOUND,
	CG_S_STARTSOUND,
	CG_S_STARTLOCALSOUND,
	CG_S_CLEARLOOPINGSOUNDS,
	CG_S_ADDLOOPINGSOUND,
	CG_S_UPDATEENTITYPOSITION,
	CG_S_ADDREALLOOPINGSOUND,
	CG_S_STOPLOOPINGSOUND,
	CG_S_RESPATIALIZE,
	CG_S_SHUTUP,
	CG_S_REGISTERSOUND,
	CG_S_STARTBACKGROUNDTRACK,
	CG_S_UPDATEAMBIENTSET,
	CG_AS_PARSESETS,
	CG_AS_ADDPRECACHEENTRY,
	CG_S_ADDLOCALSET,
	CG_AS_GETBMODELSOUND,
	CG_R_LOADWORLDMAP,
	CG_R_REGISTERMODEL,
	CG_R_REGISTERSKIN,
	CG_R_REGISTERSHADER,
	CG_R_REGISTERSHADERNOMIP,
	CG_R_REGISTERFONT,
	CG_R_FONT_STRLENPIXELS,
	CG_R_FONT_STRLENCHARS,
	CG_R_FONT_STRHEIGHTPIXELS,
	CG_R_FONT_DRAWSTRING,
	CG_LANGUAGE_ISASIAN,
	CG_LANGUAGE_USESSPACES,
	CG_ANYLANGUAGE_READCHARFROMSTRING,

	CGAME_MEMSET = 100,
	CGAME_MEMCPY,
	CGAME_STRNCPY,
	CGAME_SIN,
	CGAME_COS,
	CGAME_ATAN2,
	CGAME_SQRT,
	CGAME_MATRIXMULTIPLY,
	CGAME_ANGLEVECTORS,
	CGAME_PERPENDICULARVECTOR,
	CGAME_FLOOR,
	CGAME_CEIL,
	CGAME_TESTPRINTINT,
	CGAME_TESTPRINTFLOAT,
	CGAME_ACOS,
	CGAME_ASIN,

	CG_R_CLEARSCENE = 200,
	CG_R_CLEARDECALS,
	CG_R_ADDREFENTITYTOSCENE,
	CG_R_ADDPOLYTOSCENE,
	CG_R_ADDPOLYSTOSCENE,
	CG_R_ADDDECALTOSCENE,
	CG_R_LIGHTFORPOINT,
	CG_R_ADDLIGHTTOSCENE,
	CG_R_ADDADDITIVELIGHTTOSCENE,
	CG_R_RENDERSCENE,
	CG_R_SETCOLOR,
	CG_R_DRAWSTRETCHPIC,
	CG_R_MODELBOUNDS,
	CG_R_LERPTAG,
	CG_R_DRAWROTATEPIC,
	CG_R_DRAWROTATEPIC2,
	CG_R_SETRANGEFOG,
	CG_R_SETREFRACTIONPROP,
	CG_R_REMAP_SHADER,
	CG_R_GET_LIGHT_STYLE,
	CG_R_SET_LIGHT_STYLE,
	CG_R_GET_BMODEL_VERTS,
	CG_R_GETDISTANCECULL,
	CG_R_GETREALRES,
	CG_R_AUTOMAPELEVADJ,
	CG_R_INITWIREFRAMEAUTO,
	CG_FX_ADDLINE,
	CG_GETGLCONFIG,
	CG_GETGAMESTATE,
	CG_GETCURRENTSNAPSHOTNUMBER,
	CG_GETSNAPSHOT,
	CG_GETDEFAULTSTATE,
	CG_GETSERVERCOMMAND,
	CG_GETCURRENTCMDNUMBER,
	CG_GETUSERCMD,
	CG_SETUSERCMDVALUE,
	CG_SETCLIENTFORCEANGLE,
	CG_SETCLIENTTURNEXTENT,
	CG_OPENUIMENU,
	CG_TESTPRINTINT,
	CG_TESTPRINTFLOAT,
	CG_MEMORY_REMAINING,
	CG_KEY_ISDOWN,
	CG_KEY_GETCATCHER,
	CG_KEY_SETCATCHER,
	CG_KEY_GETKEY,
 	CG_PC_ADD_GLOBAL_DEFINE,
	CG_PC_LOAD_SOURCE,
	CG_PC_FREE_SOURCE,
	CG_PC_READ_TOKEN,
	CG_PC_SOURCE_FILE_AND_LINE,
	CG_PC_LOAD_GLOBAL_DEFINES,
	CG_PC_REMOVE_ALL_GLOBAL_DEFINES,
	CG_S_STOPBACKGROUNDTRACK,
	CG_REAL_TIME,
	CG_SNAPVECTOR,
	CG_CIN_PLAYCINEMATIC,
	CG_CIN_STOPCINEMATIC,
	CG_CIN_RUNCINEMATIC,
	CG_CIN_DRAWCINEMATIC,
	CG_CIN_SETEXTENTS,
	CG_GET_ENTITY_TOKEN,
	CG_R_INPVS,
	CG_FX_REGISTER_EFFECT,
	CG_FX_PLAY_EFFECT,
	CG_FX_PLAY_ENTITY_EFFECT,
	CG_FX_PLAY_EFFECT_ID,
	CG_FX_PLAY_PORTAL_EFFECT_ID,
	CG_FX_PLAY_ENTITY_EFFECT_ID,
	CG_FX_PLAY_BOLTED_EFFECT_ID,
	CG_FX_ADD_SCHEDULED_EFFECTS,
	CG_FX_INIT_SYSTEM,
	CG_FX_SET_REFDEF,
	CG_FX_FREE_SYSTEM,
	CG_FX_ADJUST_TIME,
	CG_FX_DRAW_2D_EFFECTS,
	CG_FX_RESET,
	CG_FX_ADDPOLY,
	CG_FX_ADDBEZIER,
	CG_FX_ADDPRIMITIVE,
	CG_FX_ADDSPRITE,
	CG_FX_ADDELECTRICITY,
	CG_SP_GETSTRINGTEXTSTRING,
	CG_ROFF_CLEAN,
	CG_ROFF_UPDATE_ENTITIES,
	CG_ROFF_CACHE,
	CG_ROFF_PLAY,
	CG_ROFF_PURGE_ENT,
	CG_TRUEMALLOC,
	CG_TRUEFREE,
	CG_G2_LISTSURFACES,
	CG_G2_LISTBONES,
	CG_G2_SETMODELS,
	CG_G2_HAVEWEGHOULMODELS,
	CG_G2_GETBOLT,
	CG_G2_GETBOLT_NOREC,
	CG_G2_GETBOLT_NOREC_NOROT,
	CG_G2_INITGHOUL2MODEL,
	CG_G2_SETSKIN,
	CG_G2_COLLISIONDETECT,
	CG_G2_COLLISIONDETECTCACHE,
	CG_G2_CLEANMODELS,
	CG_G2_ANGLEOVERRIDE,
	CG_G2_PLAYANIM,
	CG_G2_GETBONEANIM,
	CG_G2_GETBONEFRAME,
	CG_G2_GETGLANAME,
	CG_G2_COPYGHOUL2INSTANCE,
	CG_G2_COPYSPECIFICGHOUL2MODEL,
	CG_G2_DUPLICATEGHOUL2INSTANCE,
	CG_G2_HASGHOUL2MODELONINDEX,
	CG_G2_REMOVEGHOUL2MODEL,
	CG_G2_SKINLESSMODEL,
	CG_G2_GETNUMGOREMARKS,
	CG_G2_ADDSKINGORE,
	CG_G2_CLEARSKINGORE,
	CG_G2_SIZE,
	CG_G2_ADDBOLT,
	CG_G2_ATTACHENT,
	CG_G2_SETBOLTON,
	CG_G2_SETROOTSURFACE,
	CG_G2_SETSURFACEONOFF,
	CG_G2_SETNEWORIGIN,
	CG_G2_DOESBONEEXIST,
	CG_G2_GETSURFACERENDERSTATUS,
	CG_G2_GETTIME,
	CG_G2_SETTIME,
	CG_G2_ABSURDSMOOTHING,
	CG_G2_SETRAGDOLL,
	CG_G2_ANIMATEG2MODELS,
	CG_G2_RAGPCJCONSTRAINT,
	CG_G2_RAGPCJGRADIENTSPEED,
	CG_G2_RAGEFFECTORGOAL,
	CG_G2_GETRAGBONEPOS,
	CG_G2_RAGEFFECTORKICK,
	CG_G2_RAGFORCESOLVE,
	CG_G2_SETBONEIKSTATE,
	CG_G2_IKMOVE,
	CG_G2_REMOVEBONE,
	CG_G2_ATTACHINSTANCETOENTNUM,
	CG_G2_CLEARATTACHEDINSTANCE,
	CG_G2_CLEANENTATTACHMENTS,
	CG_G2_OVERRIDESERVER,
	CG_G2_GETSURFACENAME,
	CG_SET_SHARED_BUFFER,
	CG_CM_REGISTER_TERRAIN,
	CG_RMG_INIT,
	CG_RE_INIT_RENDERER_TERRAIN,
	CG_R_WEATHER_CONTENTS_OVERRIDE,
	CG_R_WORLDEFFECTCOMMAND,
	CG_WE_ADDWEATHERZONE
} cgameImportLegacy_t;

typedef enum cgameExportLegacy_e {
	CG_INIT,
	CG_SHUTDOWN,
	CG_CONSOLE_COMMAND,
	CG_DRAW_ACTIVE_FRAME,
	CG_CROSSHAIR_PLAYER,
	CG_LAST_ATTACKER,
	CG_KEY_EVENT,
	CG_MOUSE_EVENT,
	CG_EVENT_HANDLING,
	CG_POINT_CONTENTS,
	CG_GET_LERP_ORIGIN,
	CG_GET_LERP_DATA,
	CG_GET_GHOUL2,
	CG_GET_MODEL_LIST,
	CG_CALC_LERP_POSITIONS,
	CG_TRACE,
	CG_G2TRACE,
	CG_G2MARK,
	CG_RAG_CALLBACK,
	CG_INCOMING_CONSOLE_COMMAND,
	CG_GET_USEABLE_FORCE,
	CG_GET_ORIGIN,
	CG_GET_ANGLES,
	CG_GET_ORIGIN_TRAJECTORY,
	CG_GET_ANGLE_TRAJECTORY,
	CG_ROFF_NOTETRACK_CALLBACK,
	CG_IMPACT_MARK,
	CG_MAP_CHANGE,
	CG_AUTOMAP_INPUT,
	CG_MISC_ENT,
	CG_GET_SORTED_FORCE_POWER,
	CG_FX_CAMERASHAKE,
} cgameExportLegacy_t;

typedef struct {
	// common
	void* Print;// const char *msg, ... );
	void* Error;//(int level, const char* fmt, ...);
	void* SnapVector;// float *v );
	void* MemoryRemaining;// void );
	void* RegisterSharedMemory;// char *memory );
	void* TrueMalloc;// void **ptr, int size );
	void* TrueFree;// void **ptr );

	// timing
	void* Milliseconds;// void );
	void* RealTime;// qtime_t *qtime );
	void* PrecisionTimerStart;// void **timer );
	void* PrecisionTimerEnd;// void *timer );

	// cvar
	void* Cvar_Register;// vmCvar_t *vmCvar, const char *varName, const char *defaultValue, uint32_t flags );
	void* Cvar_Set;// const char *var_name, const char *value );
	void* Cvar_Update;// vmCvar_t *vmCvar );
	void* Cvar_VariableStringBuffer;// const char *var_name, char *buffer, int bufsize );

	// command
	void* AddCommand;// const char *cmd_name );
	void* Cmd_Argc;// void );
	void* Cmd_Args;// char *buffer, int bufferLength );
	void* Cmd_Argv;// int arg, char *buffer, int bufferLength );
	void* RemoveCommand;// const char *cmd_name );
	void* SendClientCommand;// const char *cmd );
	void* SendConsoleCommand;// const char *text );

	// filesystem
	void* FS_Close;// fileHandle_t f );
	void* FS_GetFileList;// const char *path, const char *extension, char *listbuf, int bufsize );
	void* FS_Open;// const char *qpath, fileHandle_t *f, fsMode_t mode );
	void* FS_Read;// void *buffer, int len, fileHandle_t f );
	void* FS_Write;// const void *buffer, int len, fileHandle_t f );

	// screen
	void* UpdateScreen;// void );

	// clip model
	void* CM_InlineModel;// int index );
	void* CM_LoadMap;// const char *mapname, qboolean subBSP );
	void* CM_NumInlineModels;// void );
	void* CM_PointContents;// const vec3_t p, clipHandle_t model );
	void* CM_RegisterTerrain;// const char *config );
	void* CM_TempModel;// const vec3_t mins, const vec3_t maxs, int capsule );
	void* CM_Trace;// trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, int capsule );
	void* CM_TransformedPointContents;// const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles );
	void* CM_TransformedTrace;// trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles, int capsule );
	void* RMG_Init;// int terrainID, const char *terrainInfo );

	// sound
	void* S_AddLocalSet;// const char *name, vec3_t listener_origin, vec3_t origin, int entID, int time );
	void* S_AddLoopingSound;// int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
	void* S_ClearLoopingSounds;// void );
	void* S_GetVoiceVolume;// int entID );
	void* S_MuteSound;// int entityNum, int entchannel );
	void* S_RegisterSound;// const char *sample );
	void* S_Respatialize;// int entityNum, const vec3_t origin, vec3_t axis[3], int inwater );
	void* S_Shutup;// qboolean shutup );
	void* S_StartBackgroundTrack;// const char *intro, const char *loop, qboolean bReturnWithoutStarting );
	void* S_StartLocalSound;// sfxHandle_t sfx, int channelNum );
	void* S_StartSound;// const vec3_t origin, int entnum, int entchannel, sfxHandle_t sfx );
	void* S_StopBackgroundTrack;// void );
	void* S_StopLoopingSound;// int entityNum );
	void* S_UpdateEntityPosition;
	void* S_UpdateAmbientSet;// const char *name, vec3_t origin ) ;

	// ambient sound
	void* AS_AddPrecacheEntry;// const char *name );
	void* AS_GetBModelSound;// const char *name, int stage );
	void* AS_ParseSets;// void );

	// renderer
	void* R_AddAdditiveLightToScene;// const vec3_t org, float intensity, float r, float g, float b );
	void* R_AddDecalToScene;// qhandle_t shader, const vec3_t origin, const vec3_t dir, float orientation, float r, float g, float b, float a, qboolean alphaFade, float radius, qboolean temporary );
	void* R_AddLightToScene;// const vec3_t org, float intensity, float r, float g, float b );
	void* R_AddPolysToScene;// qhandle_t hShader , int numVerts, const polyVert_t *verts, int num );
	void* R_AddRefEntityToScene;
	void* R_AnyLanguage_ReadCharFromString;// const char *psText, int *piAdvanceCount, qboolean *pbIsTrailingPunctuation );
	void* R_AutomapElevationAdjustment;// float newHeight );
	void* R_ClearDecals;// void );
	void* R_ClearScene;// void );
	void* R_DrawStretchPic;// float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader );	// 0 = white
	void* R_DrawRotatePic;// float x, float y, float w, float h, float s1, float t1, float s2, float t2, float a1, qhandle_t hShader );	// 0 = white
	void* R_DrawRotatePic2;// float x, float y, float w, float h, float s1, float t1, float s2, float t2, float a1, qhandle_t hShader );	// 0 = white
	void* R_Font_DrawString;// int ox, int oy, const char *text, const float *rgba, const int setIndex, int iCharLimit, const float scale );
	void* R_Font_HeightPixels;// const int iFontIndex, const float scale );
	void* R_Font_StrLenChars;// const char *text );
	void* R_Font_StrLenPixels;// const char *text, const int iFontIndex, const float scale );
	void* R_GetBModelVerts;// int bmodelIndex, vec3_t *vec, vec3_t normal );
	void* R_GetDistanceCull;// void );
	void* R_GetEntityToken;// char *buffer, int size );
	void* R_GetLightStyle;// int style, color4ub_t color );
	void* R_GetRealRes;// int *w, int *h );
	void* R_InitializeWireframeAutomap;// void );
	void* R_InPVS;// const vec3_t p1, const vec3_t p2, byte *mask );
	void* R_Language_IsAsian;// void );
	void* R_Language_UsesSpaces;// void );
	void* R_LerpTag;// orientation_t *tag,  qhandle_t model, int startFrame, int endFrame, float frac, const char *tagName );
	void* R_LightForPoint;// vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir );
	void* R_LoadWorld;// const char *name );
	void* R_MarkFragments;// int numPoints, const vec3_t *points, const vec3_t projection, int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer );
	void* R_ModelBounds;// qhandle_t model, vec3_t mins, vec3_t maxs );
	void* R_RegisterFont;// const char *fontName );
	void* R_RegisterModel;// const char *name );
	void* R_RegisterShader;// const char *name );
	void* R_RegisterShaderNoMip;// const char *name );
	void* R_RegisterSkin;// const char *name );
	void* R_RemapShader;// const char *oldShader, const char *newShader, const char *offsetTime );
	void* R_RenderScene;// const refdef_t *fd );
	void* R_SetColor;// const float *rgba );	// NULL = 1,1,1,1
	void* R_SetLightStyle;// int style, int color );
	void* R_SetRangedFog;// float range );
	void* R_SetRefractionProperties;// float distortionAlpha, float distortionStretch, qboolean distortionPrePost, qboolean distortionNegate );
	void* R_WorldEffectCommand;// const char *cmd );
	void* RE_InitRendererTerrain;// const char *info );
	void* WE_AddWeatherZone;// vec3_t mins, vec3_t maxs );

	// client
	void* GetCurrentSnapshotNumber;// int *snapshotNumber, int *serverTime );
	void* GetCurrentCmdNumber;// void );
	void* GetDefaultState;
	void* GetGameState;
	void* GetGlconfig;// glconfig_t *glconfig );
	void* GetServerCommand;// int serverCommandNumber );
	void* GetSnapshot;
} cgameImport_t;

typedef struct {
	void* Init;
	void* Shutdown;//void);
	void* ConsoleCommand;//void);
	void* DrawActiveFrame;
	void* CrosshairPlayer;//void);
	void* LastAttacker;//void);
	void* KeyEvent;//int key, qboolean down);
	void* MouseEvent;//int x, int y);
	void* EventHandling;//int type);
	void* PointContents;//void);
	void* GetLerpOrigin;//void);
	void* GetLerpData;//void);
	void* Trace;//void);
	void* G2Trace;//void);
	void* G2Mark;//void);
	void* RagCallback;//int callType);
	void* IncomingConsoleCommand;//)	(void);
	void* NoUseableForce;//void);
	void* GetOrigin;//int entID, vec3_t out);
	void* GetAngles;//int entID, vec3_t out);
	void* GetOriginTrajectory;//int entID);
	void* GetAngleTrajectory;//int entID);
	void* ROFF_NotetrackCallback;//)	(int entID, const char* notetrack);
	void* MapChange;//void);
	void* AutomapInput;//void);
	void* MiscEnt;//void);
	void* CameraShake;//void);
} cgameExport_t;

typedef struct cgImport_t {
	// common
	void			(*Print)								(const char*, ...);
	void			(*Error)(int, const char*, ...);
	void			(*SnapVector)							(float*);
	int				(*MemoryRemaining)						(void);
	void			(*RegisterSharedMemory)					(char*);
	void			(*TrueMalloc)							(void**, int);
	void			(*TrueFree)								(void**);

	// timing
	int				(*Milliseconds)							(void);
	int				(*RealTime)								(qtime_t*);
	void			(*PrecisionTimerStart)					(void**);
	int				(*PrecisionTimerEnd)					(void*);

	// cvar
	void			(*Cvar_Register)						(vmCvar_t*, const char*, const char*, uint32_t);
	void			(*Cvar_Set)								(const char*, const char*);
	void			(*Cvar_Update)							(vmCvar_t*);
	void			(*Cvar_VariableStringBuffer)			(const char*, char*, int);

	// command
	void			(*AddCommand)							(const char*);
	int				(*Cmd_Argc)								(void);
	void			(*Cmd_Args)								(char*, int);
	void			(*Cmd_Argv)								(int, char*, int);
	void			(*RemoveCommand)						(const char*);
	void			(*SendClientCommand)					(const char*);
	void			(*SendConsoleCommand)					(const char*);

	// filesystem
	void			(*FS_Close)								(fileHandle_t);
	int				(*FS_GetFileList)						(const char*, const char*, char*, int);
	int				(*FS_Open)								(const char*, fileHandle_t*, int);
	int				(*FS_Read)								(void*, int, fileHandle_t);
	int				(*FS_Write)								(const void*, int, fileHandle_t);

	// screen
	void			(*UpdateScreen)							(void);

	// clip model
	clipHandle_t(*CM_InlineModel)							(int);
	void			(*CM_LoadMap)							(const char*, qboolean);
	int				(*CM_NumInlineModels)					(void);
	int				(*CM_PointContents)						(const vec3_t, clipHandle_t);
	int				(*CM_RegisterTerrain)					(const char*);
	clipHandle_t(*CM_TempModel)								(const vec3_t, const vec3_t, int);
	void			(*CM_Trace)								(trace_t*, const vec3_t, const vec3_t, const vec3_t, const vec3_t, clipHandle_t, int, int);
	int				(*CM_TransformedPointContents)			(const vec3_t, clipHandle_t, const vec3_t, const vec3_t);
	void			(*CM_TransformedTrace)					(trace_t*, const vec3_t, const vec3_t, const vec3_t, const vec3_t, clipHandle_t, int, const vec3_t, const vec3_t, int);
	void			(*RMG_Init)								(int, const char*);

	// sound
	int				(*S_AddLocalSet)						(const char*, vec3_t, vec3_t, int, int);
	void			(*S_AddLoopingSound)					(int, const vec3_t, const vec3_t, sfxHandle_t);
	void			(*S_ClearLoopingSounds)					(void);
	int				(*S_GetVoiceVolume)						(int);
	void			(*S_MuteSound)							(int, int);
	sfxHandle_t(*S_RegisterSound)							(const char*);
	void			(*S_Respatialize)						(int, const vec3_t, vec3_t[], int);
	void			(*S_Shutup)								(qboolean);
	void			(*S_StartBackgroundTrack)				(const char*, const char*, qboolean);
	void			(*S_StartLocalSound)					(sfxHandle_t, int);
	void			(*S_StartSound)							(const vec3_t, int, int, sfxHandle_t);
	void			(*S_StopBackgroundTrack)				(void);
	void			(*S_StopLoopingSound)					(int);
	void			(*S_UpdateEntityPosition)				(int, const vec3_t);
	void			(*S_UpdateAmbientSet)					(const char*, vec3_t);

	// ambient sound
	void			(*AS_AddPrecacheEntry)					(const char*);
	sfxHandle_t(*AS_GetBModelSound)							(const char*, int);
	void			(*AS_ParseSets)							(void);

	// renderer
	void			(*R_AddAdditiveLightToScene)			(const vec3_t, float, float, float, float);
	void			(*R_AddDecalToScene)					(qhandle_t, const vec3_t, const vec3_t, float, float, float, float, float, qboolean, float, qboolean);
	void			(*R_AddLightToScene)					(const vec3_t, float, float, float, float);
	void			(*R_AddPolysToScene)					(qhandle_t, int, typename polyVert_t*, int);
	void			(*R_AddRefEntityToScene)				(refEntity_t*);
	unsigned int	(*R_AnyLanguage_ReadCharFromString)		(char*, int*, qboolean*);
	void			(*R_AutomapElevationAdjustment)			(float);
	void			(*R_ClearDecals)						(void);
	void			(*R_ClearScene)							(void);
	void			(*R_DrawStretchPic)						(float, float, float, float, float, float, float, float, qhandle_t);	// 0 = white
	void			(*R_DrawRotatePic)						(float, float, float, float, float, float, float, float, float, qhandle_t);	// 0 = white
	void			(*R_DrawRotatePic2)						(float, float, float, float, float, float, float, float, float, qhandle_t);	// 0 = white
	void			(*R_Font_DrawString)					(int, int, const char*, const float*, const int, int, const float);
	int				(*R_Font_HeightPixels)					(const int, const float);
	int				(*R_Font_StrLenChars)					(const char*);
	int				(*R_Font_StrLenPixels)					(const char*, const int, const float);
	void			(*R_GetBModelVerts)						(int, vec3_t*, vec3_t);
	float			(*R_GetDistanceCull)					(void);
	qboolean(*R_GetEntityToken)						(char*, int);
	void			(*R_GetLightStyle)						(int, color4ub_t);
	void			(*R_GetRealRes)							(int*, int*);
	qboolean(*R_InitializeWireframeAutomap)			(void);
	qboolean(*R_InPVS)								(vec3_t, vec3_t, byte*);
	qboolean(*R_Language_IsAsian)					(void);
	qboolean(*R_Language_UsesSpaces)				(void);
	int				(*R_LerpTag)							(orientation_t*, qhandle_t, int, int, float, const char*);
	int				(*R_LightForPoint)						(vec3_t, vec3_t, vec3_t, vec3_t);
	void			(*R_LoadWorld)							(const char*);
	int				(*R_MarkFragments)						(int, vec3_t*, vec3_t, int, vec3_t, int, markFragment_t*);
	void			(*R_ModelBounds)						(qhandle_t, vec3_t, vec3_t);
	qhandle_t(*R_RegisterFont)						(const char*);
	qhandle_t(*R_RegisterModel)						(const char*);
	qhandle_t(*R_RegisterShader)						(const char*);
	qhandle_t(*R_RegisterShaderNoMip)				(const char*);
	qhandle_t(*R_RegisterSkin)						(const char*);
	void			(*R_RemapShader)						(const char*, const char*, const char*);
	void			(*R_RenderScene)						(refdef_t*);
	void			(*R_SetColor)							(const float*);	// NULL = 1,1,1,1
	void			(*R_SetLightStyle)						(int, int);
	void			(*R_SetRangedFog)						(float);
	void			(*R_SetRefractionProperties)			(float, float, qboolean, qboolean);
	void			(*R_WorldEffectCommand)					(const char*);
	void			(*RE_InitRendererTerrain)				(const char*);
	void			(*WE_AddWeatherZone)					(vec3_t, vec3_t);

	// client
	void			(*GetCurrentSnapshotNumber)				(int*, int*);
	int				(*GetCurrentCmdNumber)					(void);
	qboolean(*GetDefaultState)						(int, entityState_t*);
	void			(*GetGameState)							(gameState_t*);
	void			(*GetGlconfig)							(glconfig_t*);
	qboolean(*GetServerCommand)						(int);
	qboolean(*GetSnapshot)							(int, snapshot_t*);
};

typedef struct {
	void* Init;
	void* Shutdown;//void);
	void* ConsoleCommand;//void);
	void* DrawActiveFrame;
	void* CrosshairPlayer;//void);
	void* LastAttacker;//void);
	void* KeyEvent;//int key, qboolean down);
	void* MouseEvent;//int x, int y);
	void* EventHandling;//int type);
	void* PointContents;//void);
	void* GetLerpOrigin;//void);
	void* GetLerpData;//void);
	void* Trace;//void);
	void* G2Trace;//void);
	void* G2Mark;//void);
	void* RagCallback;//int callType);
	void* IncomingConsoleCommand;//)	(void);
	void* NoUseableForce;//void);
	void* GetOrigin;//int entID, vec3_t out);
	void* GetAngles;//int entID, vec3_t out);
	void* GetOriginTrajectory;//int entID);
	void* GetAngleTrajectory;//int entID);
	void* ROFF_NotetrackCallback;//)	(int entID, const char* notetrack);
	void* MapChange;//void);
	void* AutomapInput;//void);
	void* MiscEnt;//void);
	void* CameraShake;//void);
} cgExport_t;

