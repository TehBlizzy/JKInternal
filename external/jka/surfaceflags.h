#pragma once
#define CONTENTS_NONE			(0x00000000u)
#define	CONTENTS_SOLID			(0x00000001u) // Default setting. An eye is never valid in a solid
#define	CONTENTS_LAVA			(0x00000002u) //
#define	CONTENTS_WATER			(0x00000004u) //
#define	CONTENTS_FOG			(0x00000008u) //
#define	CONTENTS_PLAYERCLIP		(0x00000010u) //
#define	CONTENTS_MONSTERCLIP	(0x00000020u) // Physically block bots
#define CONTENTS_BOTCLIP		(0x00000040u) // A hint for bots - do not enter this brush by navigation (if possible)
#define CONTENTS_SHOTCLIP		(0x00000080u) //
#define	CONTENTS_BODY			(0x00000100u) // should never be on a brush, only in game
#define	CONTENTS_CORPSE			(0x00000200u) // should never be on a brush, only in game
#define	CONTENTS_TRIGGER		(0x00000400u) //
#define	CONTENTS_NODROP			(0x00000800u) // don't leave bodies or items (death fog, lava)
#define CONTENTS_TERRAIN		(0x00001000u) // volume contains terrain data
#define CONTENTS_LADDER			(0x00002000u) //
#define CONTENTS_ABSEIL			(0x00004000u) // (SOF2) used like ladder to define where an NPC can abseil
#define CONTENTS_OPAQUE			(0x00008000u) // defaults to on, when off, solid can be seen through
#define CONTENTS_OUTSIDE		(0x00010000u) // volume is considered to be in the outside (i.e. not indoors)
#define CONTENTS_SLIME			(0x00020000u) // CHC needs this since we use same tools
#define CONTENTS_LIGHTSABER		(0x00040000u) // ""
#define CONTENTS_TELEPORTER		(0x00080000u) // ""
#define CONTENTS_ITEM			(0x00100000u) // ""
#define CONTENTS_NOSHOT			(0x00200000u) // shots pass through me
#define CONTENTS_UNUSED00400000	(0x00400000u) //
#define CONTENTS_UNUSED00800000	(0x00800000u) //
#define CONTENTS_UNUSED01000000	(0x01000000u) //
#define CONTENTS_UNUSED02000000	(0x02000000u) //
#define CONTENTS_UNUSED04000000	(0x04000000u) //
#define	CONTENTS_DETAIL			(0x08000000u) // brushes not used for the bsp
#define	CONTENTS_INSIDE			(0x10000000u) // volume is considered to be inside (i.e. indoors)
#define CONTENTS_UNUSED20000000	(0x20000000u) //
#define CONTENTS_UNUSED40000000	(0x40000000u) //
#define	CONTENTS_TRANSLUCENT	(0x80000000u) // don't consume surface fragments inside
#define CONTENTS_ALL			(0xFFFFFFFFu)
