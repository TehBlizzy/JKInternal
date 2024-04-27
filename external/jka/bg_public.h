//#include "bg_weapons.h"

//these two defs are shared now because we do clientside ent parsing

typedef enum {
	GT_FFA,				// free for all
	GT_HOLOCRON,		// holocron ffa
	GT_JEDIMASTER,		// jedi master
	GT_DUEL,		// one on one tournament
	GT_POWERDUEL,
	GT_SINGLE_PLAYER,	// single player ffa

	//-- team games go after this --

	GT_TEAM,			// team deathmatch
	GT_SIEGE,			// siege
	GT_CTF,				// capture the flag
	GT_CTY,
	GT_MAX_GAME_TYPE
} gametype_t;

typedef enum {
	STAT_HEALTH,
	STAT_HOLDABLE_ITEM,
	STAT_HOLDABLE_ITEMS,
	STAT_PERSISTANT_POWERUP,
	//MAKE SURE STAT_WEAPONS REMAINS 4!!!!
	//There is a hardcoded reference in msg.cpp to send it in 32 bits -rww
	STAT_WEAPONS = 4,					// 16 bit fields
	STAT_ARMOR,
	STAT_DEAD_YAW,					// look this direction when dead (FIXME: get rid of?)
	STAT_CLIENTS_READY,				// bit mask of clients wishing to exit the intermission (FIXME: configstring?)
	STAT_MAX_HEALTH					// health / armor limit, changable by handicap
} statIndex_t;

typedef enum {
	PERS_SCORE,						// !!! MUST NOT CHANGE, SERVER AND GAME BOTH REFERENCE !!!
	PERS_HITS,						// total points damage inflicted so damage beeps can sound on change
	PERS_RANK,						// player rank or team rank
	PERS_TEAM,						// player team
	PERS_SPAWN_COUNT,				// incremented every respawn
	PERS_PLAYEREVENTS,				// 16 bits that can be flipped for events
	PERS_ATTACKER,					// clientnum of last damage inflicter
	PERS_ATTACKEE_ARMOR,			// health/armor of last person we attacked
	PERS_KILLED,					// count of the number of times you died
	// player awards tracking
	PERS_IMPRESSIVE_COUNT,			// two railgun hits in a row
	PERS_EXCELLENT_COUNT,			// two successive kills in a short amount of time
	PERS_DEFEND_COUNT,				// defend awards
	PERS_ASSIST_COUNT,				// assist awards
	PERS_GAUNTLET_FRAG_COUNT,		// kills with the gauntlet
	PERS_CAPTURES					// captures
} persEnum_t;

typedef enum {
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_SPECIAL,				/**< rww - force fields */
	ET_HOLOCRON,			/**< rww - holocron icon displays */
	ET_MOVER,
	ET_BEAM,
	ET_PORTAL,
	ET_SPEAKER,
	ET_PUSH_TRIGGER,
	ET_TELEPORT_TRIGGER,
	ET_INVISIBLE,
	ET_NPC,					/**< ghoul2 player-like entity */
	ET_TEAM,
	ET_BODY,
	ET_TERRAIN,
	ET_FX,
	ET_FIRESPOT,			/**< A firespot with the potential to move */
	ET_CONQHOLO,
	ET_CONQNOHOLO,
#ifdef GRAPPLE_HOOK
	ET_GRAPPLE,
#endif
#ifdef MANDO_FIBERCORD_WHIP
	ET_FIBERCORD,
#endif

	ET_EVENTS				/**< any of the EV_* events can be added freestanding
							   * by setting eType to ET_EVENTS + eventNum.
							   * This avoids having to set eFlags and eventNum
							*/
} entityType_t;

#define	EF_DEAD					(1<<1)		// don't draw a foe marker over players with EF_DEAD

#define	MASK_ALL				(0xFFFFFFFFu)
#define	MASK_SOLID				(CONTENTS_SOLID|CONTENTS_TERRAIN)
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY|CONTENTS_TERRAIN)
#define	MASK_NPCSOLID			(CONTENTS_SOLID|CONTENTS_MONSTERCLIP|CONTENTS_BODY|CONTENTS_TERRAIN)
#define	MASK_DEADSOLID			(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_TERRAIN)
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA|CONTENTS_TERRAIN)
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE|CONTENTS_TERRAIN)
