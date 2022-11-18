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
void renderBulb(float x, float y, float z, float th, float s);


#ifdef __cplusplus
}
#endif

#endif
