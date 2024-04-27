#pragma once

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long ulong;

typedef enum { qfalse, qtrue } qboolean;

typedef float vec_t;
typedef float vec2_t[2], vec3_t[3], vec4_t[4], vec5_t[5];
typedef int	  ivec2_t[2], ivec3_t[3], ivec4_t[4], ivec5_t[5];
typedef vec3_t vec3pair_t[2], matrix3_t[3];

#define M_PI		3.14159265358979323846f	// matches value in gcc v2 math.h
// angle indexes
#define	PITCH	0		// up / down
#define	YAW		1		// left / right
#define	ROLL	2		// fall over
// plane_t structure
typedef struct cplane_s {
	vec3_t	normal;
	float	dist;
	byte	type;			// for fast side tests: 0,1,2 = axial, 3 = nonaxial
	byte	signbits;		// signx + (signy<<1) + (signz<<2), used as lookup during collision
	byte	pad[2];
} cplane_t;
