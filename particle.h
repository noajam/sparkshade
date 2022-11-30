#ifndef PARTICLE
#define PARTICLE

#ifndef CSCIx229
#define CSCIx229
#include "CSCIx229.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define numParticles 100

typedef struct
{
    float time[numParticles*2];
    int active[numParticles*2];
    float velocity[numParticles*6];
} ParticleSystem;

void initParticleSystem(ParticleSystem *ps, int size, GLuint *vao, GLuint *vbo, int shader);
void bufferUpdateParticleSystem(ParticleSystem *ps, int size, GLuint *vbo, int systemIsOn, int *transitioning);
void renderParticleSystem(GLuint *vao, int size, float x, float y, float z);

#ifdef __cplusplus
}
#endif

#endif
