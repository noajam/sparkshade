/*
 * Contains definitions of particle update functions
 */

// TODO: May be worth changing time to integer to speed up computation

#include "CSCIx229.h"
#include "particle.h"

// Initializes buffers for all particles
void initParticleSystem(ParticleSystem *ps, int size, GLuint *vao, GLuint *vbo, int shader)
{
    for (int i = 0; i < size; i++)
    {
        // Set time to be between 1.0 and 2.0
        ps->time[2*i] = (float)rand() / (float)RAND_MAX + 1.0;

        // Trailing vertex will be 0.01 behind
        ps->time[2*i+1] = ps->time[i] - 0.01;

        // Velocity components between -0.5 and 0.5
        // Velocity will randomize in a square pyramid directly horizontal
        // +1.0x, z and y between -0.5 and 0.5
        ps->velocity[i*6] = 1.0;
        ps->velocity[i*6+1] = (float)rand() / (float)RAND_MAX - 0.5;
        ps->velocity[i*6+2] = (float)rand() / (float)RAND_MAX - 0.5;

        ps->velocity[i*6+3] = ps->velocity[i*6];
        ps->velocity[i*6+4] = ps->velocity[i*6+1];
        ps->velocity[i*6+5] = ps->velocity[i*6+2];
    }

    glUseProgram(shader);

    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    // Time ad velocity buffer only since initial position is always the same
    glGenBuffers(2, vbo);

    // Time buffer
    //loc = glGetAttribLocation(shader, "time");
    //printf("timeLoc %i\n", loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 2, ps->time, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Velocity buffer
    //loc = glGetAttribLocation(shader, "velocity");
    //printf("veloLoc %i\n", loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 6, ps->velocity, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glUseProgram(0);
    ErrCheck("initParticleSystem");
}

// Check for fully active or fully inactive array
int checkActivity(ParticleSystem *ps, int size, int systemIsOn)
{
    for (int i = 0; i < size; i++)
    {
        if (ps->active[2*i] != systemIsOn)
            return 0;
    }
    return 1;
}

// Update particle buffers
void bufferUpdateParticleSystem(ParticleSystem *ps, int size, GLuint *vbo, int systemIsOn, int *transitioning)
{
    // If not transitioning between active and inactive, just update time
    if(!*transitioning)
    {
        for (int i = 0; i < size; i++)
        {
            ps->time[2*i] += 0.01;
            ps->time[2*i+1] += 0.01;
        }
    }
    // Otherwise update particle activity as they are respawned
    else
    {
        int isTransitionComplete = checkActivity(ps, size, systemIsOn);
        if (!isTransitionComplete)
        {
            int throwaway;
            float age;

            for (int i = 0; i < size; i++)
            {
                int index = 2*i;
                if (ps->active[index] != systemIsOn)
                {
                    // Update activities of particles being respawned.
                    // If the fractional part of time is approximately
                    // *.00 (whole number), particle is respawning in shader.
                    age = modf(ps->time[index], &throwaway);
                    if (age > 0.0 && age < 0.03)
                    {
                        ps->active[index] = systemIsOn;
                        ps->active[index+1] = systemIsOn;
                    }
                }

                ps->time[index] += 0.01;
                ps->time[index+1] += 0.01;
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                ps->time[2*i] += 0.01;
                ps->time[2*i+1] += 0.01;
            }
            *transitioning = 0;
        }

    }

    //printf("time %.2f", ps->time[0]);
    // printf("velocity1: [%.2f, %.2f, %.2f]\n", ps->velocity[0], ps->velocity[1], ps->velocity[2]);
    // printf("velocity2: [%.2f, %.2f, %.2f]\n", ps->velocity[3], ps->velocity[4], ps->velocity[5]);

    // Update VBOs with new data

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size * 2, ps->time);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * size * 6, ps->velocity);
}

void stopParticleSystem()
{
    // Hit button. Call this function. All existing particles with time away from 0 will continue to be active.
}

// Render particle system using drawArrays
void renderParticleSystem(GLuint *vao, int size)
{
    // Bind particleSystem VAO and push modelview matrix
    glBindVertexArray(*vao);
    glPushMatrix();

    glDrawArrays(GL_LINES, 0, 2*size);

    // Unbind VAO and pop modelview matrix
    glPopMatrix();
    glBindVertexArray(0);
    ErrCheck("renderParticleSystem");
}


// How can I switch off particle generation but keep remaining particles still moving?
// Make time increment slower and velocities faster. Once I flip switch, pass uniform
// to shader that says to stop new particles. New particles are generated at the origin.
// New particles have fractional 0. Maybe uniform can be an array mapping of which
// particles are new? E.g. normally all particles are generated, but when the emitter
// is turned off, any particle that reappears near the origin is killed. Basically,
// if time / (float)ceil(time) 