/*
 * Contains definitions of particle update functions
 */

// TODO: May be worth changing life and time to integers to speed up computation

#include "CSCIx229.h"
#include "particle.h"

int dt = 0.01;

// Initialize particle parameters
particle particleInit(void)
{
    particle p;
    // Randomize life between 0.8 and 1.0
    p.life = (rand() / RAND_MAX / 5) + 0.8;
    p.time = 0.0;
    p.position = (vec3){0.0, 1.0, 0.0};
    p.velocity = (vec3){rand() / RAND_MAX - 0.5, rand() / RAND_MAX - 0.5, rand() / RAND_MAX - 0.5};

    return p;
}

// Reset a dead particle
void particleReset(particle *p)
{
    p->life = (rand() / RAND_MAX / 5) + 0.8;
    p->time = 0.0;
    p->velocity = (vec3){rand() / RAND_MAX, rand() / RAND_MAX, rand() / RAND_MAX};
}

// Decrease life and increase time, vertex shader will take care of transformation
void particleUpdate(particle *p)
{
    p->life -= 0.005;
    p->time += 0.1;
    if (p->life < -0.5)
    {
        particleReset(p);
    }
    
}

// Updates all particles and render active ones
void particleSystemUpdateAndRender(particle *particleArray, int size)
{
    // Update particle states
    for (int i = 0; i < size; i++)
    {
        // Point to the next particle indexed in array
        particleUpdate((particleArray + i));
        particleRender();
    }
}

// // Initializes buffers for all particles
// void particleSystemInit(GLuint *vao, GLuint *vbo)
// {
//     for (int i = 0; i < size; i++)
//     {
//         particleArray[i] = particleInit();
//     }

// }


/*
Here's what's going to happen. All particles will be initialized on CPU here,
but they will receive position updates based on their constant velocity
in the shader. Therefore, the velocity needs to be an input attribute to the
vertex shader.

Actually, I think this won't work, because I can't update the life of particles.
Let's just try updating particles here and then sending all info to shader.
Could also make color a function of life, with the G going from 1 to 0.

Store all particle vertices in single array and VBO. If particle is dead,
discard in fragment shader. Update
*/