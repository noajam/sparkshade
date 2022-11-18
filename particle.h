#ifndef PARTICLE
#define PARTICLE

#ifndef CSCIx229
#define CSCIx229
#include "CSCIx229.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Color data
typedef struct
{
    float r, g, b, a;
} vec4;

// Position and velocity data
typedef struct
{
    float x, y, z;
} vec3;

// Particle data
typedef struct
{
    float life;
    vec3 position, velocity;
    vec4 color;
} particle;


#ifdef __cplusplus
}
#endif

#endif
