#ifndef OBJECTS
#define OBJECTS

#ifndef CSCIx229
#define CSCIx229
#include "CSCIx229.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

//  2D vector
typedef struct
{
    GLfloat x, y;
} vec2;

void initBulb(GLuint *vao, GLuint *vbo);
void renderBulb(GLuint *vao, float x, float y, float z, float th, float ph, float s);

void initFrame(GLuint *vao, GLuint *vbo);
void renderFrame(GLuint *vao, float x, float y, float z, float th, float s);

void initMatress(GLuint *vao, GLuint *vbo);
void renderMatress(GLuint *vao, float x, float y, float z, float th, float s);

void initDesk(GLuint *vao, GLuint *vbo);
void renderDesk(GLuint *vao, float x, float y, float z, float th, float s);

void initComputer(GLuint *vao, GLuint *vbo);
void renderComputer(GLuint *vao, float x, float y, float z, float th, float s);

void initKeyboard(GLuint *vao, GLuint *vbo);
void renderKeyboard(GLuint *vao, float x, float y, float z, float th, float s);

// Other possibilites given time:
//      - Mouse
//      - Door
//      - Window into skybox
//      - I think it would be funny if the room got longer whenever you were looking a different direction.
//      - Or all of a sudden a blood texture is on the matress

#ifdef __cplusplus
}
#endif

#endif
