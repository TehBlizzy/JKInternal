/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
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

// q_shared.h -- included first by ALL program modules.
// A user mod should never modify this file

#include "q_math.h"

#define MAX_WORLD_COORD		( 64 * 1024 )
#define MIN_WORLD_COORD		( -64 * 1024 )
#define WORLD_SIZE			( MAX_WORLD_COORD - MIN_WORLD_COORD )

#define MAX_TEAMNAME 32


#include <assert.h>
#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <stddef.h>
#include <cstddef>
#include <cstdint>

typedef union fileBuffer_u {
	void* v;
	char* c;
	byte* b;
} fileBuffer_t;

typedef int32_t qhandle_t, thandle_t, fxHandle_t, sfxHandle_t, fileHandle_t, clipHandle_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define	MAX_STRING_CHARS	1024	// max length of a string passed to Cmd_TokenizeString
#define	MAX_STRING_TOKENS	1024	// max tokens resulting from Cmd_TokenizeString
#define	MAX_TOKEN_CHARS		1024	// max length of an individual token

#define	MAX_INFO_STRING		1024
#define	MAX_INFO_KEY		1024
#define	MAX_INFO_VALUE		1024

#define	BIG_INFO_STRING		8192  // used for system info key only
#define	BIG_INFO_KEY		  8192
#define	BIG_INFO_VALUE		8192

#define NET_ADDRSTRMAXLEN 48 // maximum length of an IPv6 address string including trailing '\0'

// moved these from ui_local.h so we can access them everywhere
#define MAX_ADDRESSLENGTH		256//64
#define MAX_HOSTNAMELENGTH		256//22
#define MAX_MAPNAMELENGTH		256//16
#define MAX_STATUSLENGTH		256//64

#define	MAX_QPATH			64		// max length of a quake game pathname
#ifdef PATH_MAX
#define MAX_OSPATH			PATH_MAX
#else
#define	MAX_OSPATH			256		// max length of a filesystem pathname
#endif

#define	MAX_NAME_LENGTH		32		// max length of a client name
#define MAX_NETNAME			36

#define	MAX_SAY_TEXT	150
#define	MAX_MAP_AREA_BYTES		32		// bit vector of area visibility

typedef enum
{
	FP_FIRST = 0,//marker
	FP_HEAL = 0,//instant
	FP_LEVITATION,//hold/duration
	FP_SPEED,//duration
	FP_PUSH,//hold/duration
	FP_PULL,//hold/duration
	FP_TELEPATHY,//instant
	FP_GRIP,//hold/duration
	FP_LIGHTNING,//hold/duration
	FP_RAGE,//duration
	FP_PROTECT,
	FP_ABSORB,
	FP_TEAM_HEAL,
	FP_TEAM_FORCE,
	FP_DRAIN,
	FP_SEE,
	FP_SABER_OFFENSE,
	FP_SABER_DEFENSE,
	FP_SABERTHROW,
	NUM_FORCE_POWERS
} forcePowers_t;

// nothing outside the Cvar_*() functions should modify these fields!
typedef struct cvar_s {
	char* name;
	char* description;
	char* string;
	char* resetString;		// cvar_restart will reset to this value
	char* latchedString;		// for CVAR_LATCH vars
	uint32_t		flags;
	qboolean		modified;			// set each time the cvar is changed
	int				modificationCount;	// incremented each time the cvar is changed
	float			value;				// atof( string )
	int				integer;			// atoi( string )
	qboolean		validate;
	qboolean		integral;
	float			min, max;

	struct cvar_s* next, * prev;
	struct cvar_s* hashNext, * hashPrev;
	int				hashIndex;
} cvar_t;

#define	MAX_CVAR_VALUE_STRING	256

typedef int	cvarHandle_t;

// the modules that run in the virtual machine can't access the cvar_t directly,
// so they must ask for structured updates
typedef struct vmCvar_s {
	cvarHandle_t	handle;
	int			modificationCount;
	float		value;
	int			integer;
	char		string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;


#include "surfaceflags.h"			// shared with the q3map utility


typedef struct trace_s {
	byte		allsolid;	// if true, plane is not valid
	byte		startsolid;	// if true, the initial point was in a solid area
	short		entityNum;	// entity the contacted sirface is a part of

	float		fraction;	// time completed, 1.0 = didn't hit anything
	vec3_t		endpos;		// final position
	cplane_t	plane;		// surface normal at impact, transformed to world space
	int			surfaceFlags;	// surface hit
	int			contents;	// contents on other side of surface hit
} trace_t;

typedef struct markFragment_s {
	int		firstPoint;
	int		numPoints;
} markFragment_t;



typedef struct orientation_s {
	vec3_t		origin;
	matrix3_t	axis;
} orientation_t;


//
// per-level limits
//
#define	MAX_CLIENTS			32		// absolute limit
#define MAX_RADAR_ENTITIES	MAX_GENTITIES
#define MAX_TERRAINS		1//32 //rwwRMG: inserted
#define MAX_LOCATIONS		64

#define	GENTITYNUM_BITS	10		// don't need to send any more
#define	MAX_GENTITIES	(1<<GENTITYNUM_BITS)

// these are also in be_aas_def.h - argh (rjr)
#define	MAX_MODELS			512		// these are sent over the net as -12 bits
#define	MAX_SOUNDS			256		// so they cannot be blindly increased
#define MAX_ICONS			64		// max registered icons you can have per map
#define MAX_FX				64		// max effects strings, I'm hoping that 64 will be plenty

#define	MAX_CONFIGSTRINGS	1700 //this is getting pretty high. Try not to raise it anymore than it already is.

#define	MAX_GAMESTATE_CHARS	16000
typedef struct gameState_s {
	int			stringOffsets[MAX_CONFIGSTRINGS];
	char		stringData[MAX_GAMESTATE_CHARS];
	int			dataCount;
} gameState_t;

//=========================================================

// all the different tracking "channels"
typedef enum {
	TRACK_CHANNEL_NONE = 50,
	TRACK_CHANNEL_1,
	TRACK_CHANNEL_2, // force speed
	TRACK_CHANNEL_3, // force rage
	TRACK_CHANNEL_4,
	TRACK_CHANNEL_5, // force sight
	NUM_TRACK_CHANNELS
} trackchan_t;

#define TRACK_CHANNEL_MAX (NUM_TRACK_CHANNELS-50)

typedef struct forcedata_s {
	int			forcePowerDebounce[NUM_FORCE_POWERS];	//for effects that must have an interval
	int			forcePowersKnown;
	int			forcePowersActive;
	int			forcePowerSelected;
	int			forceButtonNeedRelease;
	int			forcePowerDuration[NUM_FORCE_POWERS];
	int			forcePower;
	int			forcePowerMax;
	int			forcePowerRegenDebounceTime;
	int			forcePowerLevel[NUM_FORCE_POWERS];		//so we know the max forceJump power you have
	int			forcePowerBaseLevel[NUM_FORCE_POWERS];
	int			forceUsingAdded;
	float		forceJumpZStart;					//So when you land, you don't get hurt as much
	float		forceJumpCharge;					//you're current forceJump charge-up level, increases the longer you hold the force jump button down
	int			forceJumpSound;
	int			forceJumpAddTime;
	int			forceGripEntityNum;					//what entity I'm gripping
	int			forceGripDamageDebounceTime;		//debounce for grip damage
	float		forceGripBeingGripped;				//if > level.time then client is in someone's grip
	int			forceGripCripple;					//if != 0 then make it so this client can't move quickly (he's being gripped)
	int			forceGripUseTime;					//can't use if > level.time
	float		forceGripSoundTime;
	float		forceGripStarted;					//level.time when the grip was activated
	int			forceHealTime;
	int			forceHealAmount;

	//This hurts me somewhat to do, but there's no other real way to allow completely "dynamic" mindtricking.
	int			forceMindtrickTargetIndex; //0-15
	int			forceMindtrickTargetIndex2; //16-32
	int			forceMindtrickTargetIndex3; //33-48
	int			forceMindtrickTargetIndex4; //49-64

	int			forceRageRecoveryTime;
	int			forceDrainEntNum;
	float		forceDrainTime;

	int			forceDoInit;

	int			forceSide;
	int			forceRank;

	int			forceDeactivateAll;

	int			killSoundEntIndex[TRACK_CHANNEL_MAX]; //this goes here so it doesn't get wiped over respawn

	qboolean	sentryDeployed;

	int			saberAnimLevelBase;//sigh...
	int			saberAnimLevel;
	int			saberDrawAnimLevel;

	int			suicides;

	int			privateDuelTime;
} forcedata_t;


// bit field limits
#define	MAX_STATS				16
#define	MAX_PERSISTANT			16
#define	MAX_POWERUPS			16
#define	MAX_WEAPONS				19
#define MAX_AMMO_TRANSMIT		16 // This is needed because the ammo array is 19 but only 16 sized array is networked
#define MAX_AMMO				MAX_WEAPONS

#define	MAX_PS_EVENTS			2

typedef struct playerState_s {
	int			commandTime;	// cmd->serverTime of last executed command
	int			pm_type;
	int			bobCycle;		// for view bobbing and footstep generation
	int			pm_flags;		// ducked, jump_held, etc
	int			pm_time;

	vec3_t		origin;
	vec3_t		velocity;

	vec3_t		moveDir; //NOT sent over the net - nor should it be.

	int			weaponTime;
	int			weaponChargeTime;
	int			weaponChargeSubtractTime;
	int			gravity;
	float		speed;
	int			basespeed; //used in prediction to know base server g_speed value when modifying speed between updates
	int			delta_angles[3];	// add to command angles to get view direction
									// changed by spawns, rotating objects, and teleporters

	int			slopeRecalcTime; //this is NOT sent across the net and is maintained seperately on game and cgame in pmove code.

	int			useTime;

	int			groundEntityNum;// ENTITYNUM_NONE = in air

	int			legsTimer;		// don't change low priority animations until this runs out
	int			legsAnim;

	int			torsoTimer;		// don't change low priority animations until this runs out
	int			torsoAnim;

	qboolean	legsFlip; //set to opposite when the same anim needs restarting, sent over in only 1 bit. Cleaner and makes porting easier than having that god forsaken ANIM_TOGGLEBIT.
	qboolean	torsoFlip;

	int			movementDir;	// a number 0 to 7 that represents the reletive angle
								// of movement to the view angle (axial and diagonals)
								// when at rest, the value will remain unchanged
								// used to twist the legs during strafing

	int			eFlags;			// copied to entityState_t->eFlags
	int			eFlags2;		// copied to entityState_t->eFlags2, EF2_??? used much less frequently

	int			eventSequence;	// pmove generated events
	int			events[MAX_PS_EVENTS];
	int			eventParms[MAX_PS_EVENTS];

	int			externalEvent;	// events set on player from another source
	int			externalEventParm;
	int			externalEventTime;

	int			clientNum;		// ranges from 0 to MAX_CLIENTS-1
	int			weapon;			// copied to entityState_t->weapon
	int			weaponstate;

	vec3_t		viewangles;		// for fixed views
	int			viewheight;

	// damage feedback
	int			damageEvent;	// when it changes, latch the other parms
	int			damageYaw;
	int			damagePitch;
	int			damageCount;
	int			damageType;

	int			painTime;		// used for both game and client side to process the pain twitch - NOT sent across the network
	int			painDirection;	// NOT sent across the network
	float		yawAngle;		// NOT sent across the network
	qboolean	yawing;			// NOT sent across the network
	float		pitchAngle;		// NOT sent across the network
	qboolean	pitching;		// NOT sent across the network

	int			stats[MAX_STATS];
	int			persistant[MAX_PERSISTANT];	// stats that aren't cleared on death
	int			powerups[MAX_POWERUPS];	// level.time that the powerup runs out
	int			ammo[MAX_AMMO];

	int			generic1;
	int			loopSound;
	int			jumppad_ent;	// jumppad entity hit this frame

	// not communicated over the net at all
	int			ping;			// server to game info for scoreboard
	int			pmove_framecount;	// FIXME: don't transmit over the network
	int			jumppad_frame;
	int			entityEventSequence;

	int			lastOnGround;	//last time you were on the ground

	qboolean	saberInFlight;

	int			saberMove;
	int			saberBlocking;
	int			saberBlocked;

	int			saberLockTime;
	int			saberLockEnemy;
	int			saberLockFrame; //since we don't actually have the ability to get the current anim frame
	int			saberLockHits; //every x number of buttons hits, allow one push forward in a saber lock (server only)
	int			saberLockHitCheckTime; //so we don't allow more than 1 push per server frame
	int			saberLockHitIncrementTime; //so we don't add a hit per attack button press more than once per server frame
	qboolean	saberLockAdvance; //do an advance (sent across net as 1 bit)

	int			saberEntityNum;
	float		saberEntityDist;
	int			saberEntityState;
	int			saberThrowDelay;
	qboolean	saberCanThrow;
	int			saberDidThrowTime;
	int			saberDamageDebounceTime;
	int			saberHitWallSoundDebounceTime;
	int			saberEventFlags;

	int			rocketLockIndex;
	float		rocketLastValidTime;
	float		rocketLockTime;
	float		rocketTargetTime;

	int			emplacedIndex;
	float		emplacedTime;

	qboolean	isJediMaster;
	qboolean	forceRestricted;
	qboolean	trueJedi;
	qboolean	trueNonJedi;
	int			saberIndex;

	int			genericEnemyIndex;
	float		droneFireTime;
	float		droneExistTime;

	int			activeForcePass;

	qboolean	hasDetPackPlanted; //better than taking up an eFlag isn't it?

	float		holocronsCarried[NUM_FORCE_POWERS];
	int			holocronCantTouch;
	float		holocronCantTouchTime; //for keeping track of the last holocron that just popped out of me (if any)
	int			holocronBits;

	int			electrifyTime;

	int			saberAttackSequence;
	int			saberIdleWound;
	int			saberAttackWound;
	int			saberBlockTime;

	int			otherKiller;
	int			otherKillerTime;
	int			otherKillerDebounceTime;

	forcedata_t	fd;
	qboolean	forceJumpFlip;
	int			forceHandExtend;
	int			forceHandExtendTime;

	int			forceRageDrainTime;

	int			forceDodgeAnim;
	qboolean	quickerGetup;

	int			groundTime;		// time when first left ground

	int			footstepTime;

	int			otherSoundTime;
	float		otherSoundLen;

	int			forceGripMoveInterval;
	int			forceGripChangeMovetype;

	int			forceKickFlip;

	int			duelIndex;
	int			duelTime;
	qboolean	duelInProgress;

	int			saberAttackChainCount;

	int			saberHolstered;

	int			forceAllowDeactivateTime;

	// zoom key
	int			zoomMode;		// 0 - not zoomed, 1 - disruptor weapon
	int			zoomTime;
	qboolean	zoomLocked;
	float		zoomFov;
	int			zoomLockTime;

	int			fallingToDeath;

	int			useDelay;

	qboolean	inAirAnim;

	vec3_t		lastHitLoc;

	int			heldByClient; //can only be a client index - this client should be holding onto my arm using IK stuff.

	int			ragAttach; //attach to ent while ragging

	int			iModelScale;

	int			brokenLimbs;

	//for looking at an entity's origin (NPCs and players)
	qboolean	hasLookTarget;
	int			lookTarget;

	int			customRGBA[4];

	int			standheight;
	int			crouchheight;

	//If non-0, this is the index of the vehicle a player/NPC is riding.
	int			m_iVehicleNum;

	//lovely hack for keeping vehicle orientation in sync with prediction
	vec3_t		vehOrientation;
	qboolean	vehBoarding;
	int			vehSurfaces;

	//vehicle turnaround stuff (need this in ps so it doesn't jerk too much in prediction)
	int			vehTurnaroundIndex;
	int			vehTurnaroundTime;

	//vehicle has weapons linked
	qboolean	vehWeaponsLinked;

	//when hyperspacing, you just go forward really fast for HYPERSPACE_TIME
	int			hyperSpaceTime;
	vec3_t		hyperSpaceAngles;

	//hacking when > time
	int			hackingTime;
	//actual hack amount - only for the proper percentage display when
	//drawing progress bar (is there a less bandwidth-eating way to do
	//this without a lot of hassle?)
	int			hackingBaseTime;

	//keeps track of jetpack fuel
	int			jetpackFuel;

	//keeps track of cloak fuel
	int			cloakFuel;

	//rww - spare values specifically for use by mod authors.
	//See psf_overrides.txt if you want to increase the send
	//amount of any of these above 1 bit.
	int			userInt1;
	int			userInt2;
	int			userInt3;
	float		userFloat1;
	float		userFloat2;
	float		userFloat3;
	vec3_t		userVec1;
	vec3_t		userVec2;

#ifdef _ONEBIT_COMBO
	int			deltaOneBits;
	int			deltaNumBits;
#endif
} playerState_t;

typedef struct usercmd_s {
	int				serverTime;
	int				angles[3];
	int 			buttons;
	byte			weapon;           // weapon
	byte			forcesel;
	byte			invensel;
	byte			generic_cmd;
	signed char	forwardmove, rightmove, upmove;
} usercmd_t;


typedef enum {
	TR_STATIONARY,
	TR_INTERPOLATE,				// non-parametric, but interpolate between snapshots
	TR_LINEAR,
	TR_LINEAR_STOP,
	TR_NONLINEAR_STOP,
	TR_SINE,					// value = base + sin( time / duration ) * delta
	TR_GRAVITY
} trType_t;

typedef struct trajectory_s {
	trType_t	trType;
	int		trTime;
	int		trDuration;			// if non 0, trTime + trDuration = stop time
	vec3_t	trBase;
	vec3_t	trDelta;			// velocity, etc
} trajectory_t;

// entityState_t is the information conveyed from the server
// in an update message about entities that the client will
// need to render in some way
// Different eTypes may use the information in different ways
// The messages are delta compressed, so it doesn't really matter if
// the structure size is fairly large
typedef struct entityState_s {
	int		number;			// entity index
	int		eType;			// entityType_t
	int		eFlags;
	int		eFlags2;		// EF2_??? used much less frequently

	trajectory_t	pos;	// for calculating position
	trajectory_t	apos;	// for calculating angles

	int		time;
	int		time2;

	vec3_t	origin;
	vec3_t	origin2;

	vec3_t	angles;
	vec3_t	angles2;

	//rww - these were originally because we shared g2 info client and server side. Now they
	//just get used as generic values everywhere.
	int		bolt1;
	int		bolt2;

	//rww - this is necessary for determining player visibility during a jedi mindtrick
	int		trickedentindex; //0-15
	int		trickedentindex2; //16-32
	int		trickedentindex3; //33-48
	int		trickedentindex4; //49-64

	float	speed;

	int		fireflag;

	int		genericenemyindex;

	int		activeForcePass;

	int		emplacedOwner;

	int		otherEntityNum;	// shotgun sources, etc
	int		otherEntityNum2;

	int		groundEntityNum;	// ENTITYNUM_NONE = in air

	int		constantLight;	// r + (g<<8) + (b<<16) + (intensity<<24)
	int		loopSound;		// constantly loop this sound
	qboolean	loopIsSoundset; //qtrue if the loopSound index is actually a soundset index

	int		soundSetIndex;

	int		modelGhoul2;
	int		g2radius;
	int		modelindex;
	int		modelindex2;
	int		clientNum;		// 0 to (MAX_CLIENTS - 1), for players and corpses
	int		frame;

	qboolean	saberInFlight;
	int			saberEntityNum;
	int			saberMove;
	int			forcePowersActive;
	int			saberHolstered;//sent in only only 2 bits - should be 0, 1 or 2

	qboolean	isJediMaster;

	qboolean	isPortalEnt; //this needs to be seperate for all entities I guess, which is why I couldn't reuse another value.

	int		solid;			// for client side prediction, trap_linkentity sets this properly

	int		event;			// impulse events -- muzzle flashes, footsteps, etc
	int		eventParm;

	// so crosshair knows what it's looking at
	int			owner;
	int			teamowner;
	qboolean	shouldtarget;

	// for players
	int		powerups;		// bit flags
	int		weapon;			// determines weapon and flash model, etc
	int		legsAnim;
	int		torsoAnim;

	qboolean	legsFlip; //set to opposite when the same anim needs restarting, sent over in only 1 bit. Cleaner and makes porting easier than having that god forsaken ANIM_TOGGLEBIT.
	qboolean	torsoFlip;

	int		forceFrame;		//if non-zero, force the anim frame

	int		generic1;

	int		heldByClient; //can only be a client index - this client should be holding onto my arm using IK stuff.

	int		ragAttach; //attach to ent while ragging

	int		iModelScale; //rww - transfer a percentage of the normal scale in a single int instead of 3 x-y-z scale values

	int		brokenLimbs;

	int		boltToPlayer; //set to index of a real client+1 to bolt the ent to that client. Must be a real client, NOT an NPC.

	//for looking at an entity's origin (NPCs and players)
	qboolean	hasLookTarget;
	int			lookTarget;

	int			customRGBA[4];

	//I didn't want to do this, but I.. have no choice. However, we aren't setting this for all ents or anything,
	//only ones we want health knowledge about on cgame (like siege objective breakables) -rww
	int			health;
	int			maxhealth; //so I know how to draw the stupid health bar

	//NPC-SPECIFIC FIELDS
	//------------------------------------------------------------
	int		npcSaber1;
	int		npcSaber2;

	//index values for each type of sound, gets the folder the sounds
	//are in. I wish there were a better way to do this,
	int		csSounds_Std;
	int		csSounds_Combat;
	int		csSounds_Extra;
	int		csSounds_Jedi;

	int		surfacesOn; //a bitflag of corresponding surfaces from a lookup table. These surfaces will be forced on.
	int		surfacesOff; //same as above, but forced off instead.

	//Allow up to 4 PCJ lookup values to be stored here.
	//The resolve to configstrings which contain the name of the
	//desired bone.
	int		boneIndex1;
	int		boneIndex2;
	int		boneIndex3;
	int		boneIndex4;

	//packed with x, y, z orientations for bone angles
	int		boneOrient;

	//I.. feel bad for doing this, but NPCs really just need to
	//be able to control this sort of thing from the server sometimes.
	//At least it's at the end so this stuff is never going to get sent
	//over for anything that isn't an NPC.
	vec3_t	boneAngles1; //angles of boneIndex1
	vec3_t	boneAngles2; //angles of boneIndex2
	vec3_t	boneAngles3; //angles of boneIndex3
	vec3_t	boneAngles4; //angles of boneIndex4

	int		NPC_class; //we need to see what it is on the client for a few effects.

	//If non-0, this is the index of the vehicle a player/NPC is riding.
	int		m_iVehicleNum;

	//rww - spare values specifically for use by mod authors.
	//See netf_overrides.txt if you want to increase the send
	//amount of any of these above 1 bit.
	int			userInt1;
	int			userInt2;
	int			userInt3;
	float		userFloat1;
	float		userFloat2;
	float		userFloat3;
	vec3_t		userVec1;
	vec3_t		userVec2;
} entityState_t;

typedef struct qtime_s {
	int tm_sec;     /* seconds after the minute - [0,59] */
	int tm_min;     /* minutes after the hour - [0,59] */
	int tm_hour;    /* hours since midnight - [0,23] */
	int tm_mday;    /* day of the month - [1,31] */
	int tm_mon;     /* months since January - [0,11] */
	int tm_year;    /* years since 1900 */
	int tm_wday;    /* days since Sunday - [0,6] */
	int tm_yday;    /* days since January 1 - [0,365] */
	int tm_isdst;   /* daylight savings time flag */
} qtime_t;

#define SAY_ALL		0
#define SAY_TEAM	1
#define SAY_TELL	2

typedef enum vmSlots_e {
	VM_GAME = 0,
	VM_CGAME,
	VM_UI,
	MAX_VM
} vmSlots_t;

typedef struct vm_s {
	vmSlots_t	slot; // VM_GAME, VM_CGAME, VM_UI
	char		name[MAX_QPATH];
	void* dllHandle;
	qboolean	isLegacy; // uses the legacy syscall/vm_call api, is set by VM_CreateLegacy

	// fill the import/export tables
	void* (*GetModuleAPI)(int apiVersion, ...);

	// legacy stuff
	struct {
		intptr_t* main; // module vmMain
		intptr_t(__cdecl* syscall)(intptr_t* parms);	// engine syscall handler
	} legacy;
} vm_t;