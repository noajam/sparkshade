#include "CSCIx229.h"
#include "objects.h"

//  Make vec2 unit length
vec2 normalize(float x, float y)
{
    float len = sqrt(x * x + y * y);
    if (len > 0)
    {
        x /= len;
        y /= len;
    }
    return (vec2){x, y};
}

// Reverse array of vertex components
void reverse(GLfloat *arr, int size)
{
    int aux[size];

    for (int i = 0; i < size / 3; i++)
    {
        int index = i * 3;
        aux[index] = arr[size - 3 - index];
        aux[index + 1] = arr[size - 2 - index];
        aux[index + 2] = arr[size - 1 - index];
    }

    for (int i = 0; i < size; i++)
        arr[i] = aux[i];
}

// Outline of bulb for axisymmetric generation
#define Nb 102
vec2 bulb[Nb] = {
    {0.1, -20.0},
    {0.1, -2.0},
    {0.35999999999999993, -2.0},
    {0.3774999999999999, -1.975},
    {0.39, -1.95},
    {0.3975, -1.9249999999999998},
    {0.4, -1.9},
    {0.4, -1.33},
    {0.4052566638948991, -1.3124444444444445},
    {0.4118130310348549, -1.2811851851851852},
    {0.41937807004249616, -1.2499259259259259},
    {0.42795178091782293, -1.2186666666666666},
    {0.43753416366083514, -1.1874074074074075},
    {0.44812521827153295, -1.1561481481481481},
    {0.4597249447499161, -1.124888888888889},
    {0.4723333430959849, -1.0936296296296297},
    {0.48595041330973915, -1.0623703703703704},
    {0.500576155391179, -1.031111111111111},
    {0.5162105693403042, -0.9998518518518519},
    {0.5328536551571149, -0.9685925925925927},
    {0.5505054128416113, -0.9373333333333334},
    {0.5691658423937929, -0.9060740740740741},
    {0.5888349438136603, -0.8748148148148148},
    {0.609512717101213, -0.8435555555555556},
    {0.6311991622564513, -0.8122962962962963},
    {0.653894279279375, -0.7810370370370371},
    {0.6775980681699842, -0.7497777777777779},
    {0.702310528928279, -0.7185185185185186},
    {0.7280316615542592, -0.6872592592592593},
    {0.7547614660479252, -0.656},
    {0.7614459583691344, -0.6482283953077884},
    {0.7818314824680298, -0.6234898018587335},
    {0.8014136218679567, -0.5981105304912159},
    {0.8201722545969559, -0.5721166601221697},
    {0.8380881048918406, -0.5455349012105487},
    {0.8551427630053462, -0.5183925683105249},
    {0.8713187041233894, -0.49071755200393785},
    {0.8865993063730001, -0.4625382902408352},
    {0.9009688679024191, -0.433883739117558},
    {0.9144126230158125, -0.40478334312239383},
    {0.9269167573460217, -0.3752670048793742},
    {0.9384684220497604, -0.34536505442130755},
    {0.9490557470106686, -0.3151082180236207},
    {0.9586678530366607, -0.2845275866310323},
    {0.9672948630390295, -0.25365458390950735},
    {0.9749279121818236, -0.22252093395631448},
    {0.9815591569910653, -0.19115862870137224},
    {0.9871817834144502, -0.15959989503337924},
    {0.9917900138232462, -0.12787716168450586},
    {0.9953791129491982, -0.09602302590768172},
    {0.9979453927503363, -0.06407021998071297},
    {0.9994862162006879, -0.03205157757165508},
    {1.0, 0.0},
    {0.9994862162006879, 0.03205157757165517},
    {0.9979453927503363, 0.06407021998071291},
    {0.9953791129491982, 0.09602302590768176},
    {0.9917900138232462, 0.127877161684506},
    {0.9871817834144502, 0.15959989503337924},
    {0.9815591569910653, 0.19115862870137232},
    {0.9749279121818236, 0.2225209339563144},
    {0.9672948630390295, 0.25365458390950735},
    {0.9586678530366606, 0.28452758663103245},
    {0.9490557470106686, 0.3151082180236207},
    {0.9384684220497604, 0.3453650544213076},
    {0.9269167573460217, 0.37526700487937414},
    {0.9144126230158125, 0.40478334312239383},
    {0.9009688679024191, 0.4338837391175581},
    {0.8865993063730001, 0.46253829024083526},
    {0.8713187041233894, 0.49071755200393785},
    {0.8551427630053462, 0.518392568310525},
    {0.8380881048918406, 0.5455349012105487},
    {0.8201722545969559, 0.5721166601221697},
    {0.8014136218679566, 0.598110530491216},
    {0.7818314824680298, 0.6234898018587335},
    {0.7614459583691344, 0.6482283953077884},
    {0.7402779970753155, 0.6723008902613168},
    {0.7183493500977276, 0.6956825506034864},
    {0.6956825506034864, 0.7183493500977276},
    {0.6723008902613168, 0.7402779970753155},
    {0.6482283953077884, 0.7614459583691344},
    {0.6234898018587336, 0.7818314824680298},
    {0.5981105304912159, 0.8014136218679567},
    {0.5721166601221697, 0.8201722545969559},
    {0.5455349012105486, 0.8380881048918407},
    {0.5183925683105252, 0.8551427630053461},
    {0.4907175520039379, 0.8713187041233894},
    {0.4625382902408352, 0.8865993063730001},
    {0.43388373911755795, 0.9009688679024193},
    {0.4047833431223938, 0.9144126230158125},
    {0.3752670048793742, 0.9269167573460217},
    {0.3453650544213076, 0.9384684220497604},
    {0.31510821802362077, 0.9490557470106686},
    {0.2845275866310322, 0.9586678530366607},
    {0.2536545839095073, 0.9672948630390295},
    {0.22252093395631445, 0.9749279121818236},
    {0.19115862870137226, 0.9815591569910653},
    {0.15959989503337932, 0.9871817834144501},
    {0.127877161684506, 0.9917900138232462},
    {0.09602302590768166, 0.9953791129491982},
    {0.06407021998071295, 0.9979453927503363},
    {0.03205157757165511, 0.9994862162006879},
    {0.0, 1.0},
};

//  Calculate normals for axisymmetric object
void CalcNorm(int N, vec2 *object, vec2 *norm)
{
   //  Calculate normals for each facet
   //  y increases with point index so dy>0
   for (int i=0;i<N-2;i++)
   {
      // Vector in the plane of the facet
      float dx = object[i+1].x - object[i].x;
      float dy = object[i+1].y - object[i].y;
      // Normal is perpendicular
      // dy>0 so normal faces out
      norm[i] = normalize(dy,-dx);
   }
   //  The normal at the last point is straight up
   //  This is NOT true in general
   norm[N-1] = (vec2){0,1};

   //  Average normals of adjacent facets
   //  First and last normal unchanged
   vec2 N2 = norm[0];
   for (int i=1;i<N-2;i++)
   {
      vec2 N1 = N2;
      N2 = norm[i];
      norm[i] = normalize(N1.x+N2.x , N1.y+N2.y);
   }
}

// Initialize bulb VAO and VBOs
void initBulb(GLuint *vao, GLuint *vbo)
{
    // Number of vertices:
    //    Nth = 360 / inc
    //    (Nb * (Nth + 1)) + ((Nb - 2) * (Nth + 1))
    //    Nb * (Nth + 1) + (Nb - 2) * (Nth + 1)
    //    (Nth + 1)(2Nb - 2)
    //    2(Nb - 1)(Nth + 1)
    // Length of arrays:
    //    4 * 2(Nb - 1)(Nth + 1)
    //        8 * 25 * (Nb - 1)
    //    3 * 2(Nb - 1)(Nth + 1)
    //        6 * 25 * (Nb - 1)
    //    2 * 2(Nb - 1)(Nth + 1)
    //        4 * 25 * (Nb - 1)
    // int len4 = 200 * (Nb - 1);
    int len3 = 150 * (Nb - 1);
    // int len2 = 100 * (Nb - 1);

    GLfloat vertArray[len3];
    // GLfloat normArray[len3];
    // GLfloat colorArray[len4];
    // GLfloat texArray[len2];

    vec2 norm[Nb];
    CalcNorm(Nb, bulb, norm);

    int inc = 15;
    int j = 0;

    // Calculate normals for every quad
    for (int i = 0; i < Nb - 1; i++)
    {
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);

            // Each array has Nb * 3 * (360/inc) elements
            // j indexes the array by next element
            vertArray[j]   = c*bulb[i].x;
            vertArray[j+1] = bulb[i].y;
            vertArray[j+2] = s*bulb[i].x;

            vertArray[j+3] = c*bulb[i+1].x;
            vertArray[j+4] = bulb[i+1].y;
            vertArray[j+5] = s*bulb[i+1].x;

            j += 6;
        }
    }

    // Set up VAO
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    // Set up VBOs for vertices, normals, and texture coords?
    glGenBuffers(3, vbo);

    // Bind vertex buffer (set active), then copy vertex data to buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, len3 * sizeof(GLfloat), vertArray, GL_STATIC_DRAW);

    // Assign vertex buffer to first attribute array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    ErrCheck("initBulb");  
}

void renderBulb(GLuint *vao, float x, float y, float z, float th, float ph, float s)
{
    glBindVertexArray(*vao);

    float whiteyellow[] = {1,1,0.8,0.9};

    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(th, 0, 0, 1);
    glRotated(ph, 1, 0, 0);
    glScaled(s, s, s);

    int numVertices = 50 * (Nb - 1);

    // Draw bulb
    glColor4fv(whiteyellow);
    glDrawArrays(GL_QUAD_STRIP, 450, numVertices - 450);

    // Draw base
    glColor3f(0.2, 0.2, 0.2);
    glDrawArrays(GL_QUAD_STRIP, 50, 400);

    // Draw cord
    glColor3f(0.1,0.1,0.1);
    glDrawArrays(GL_QUAD_STRIP, 0, 50);

    glPopMatrix();

    glBindVertexArray(0);
    ErrCheck("renderBulb");
}


// Initialize bed frame VAO and VBOs
void initFrame(GLuint *vao, GLuint *vbo)
{
    // Can include a unique bedpost later.

    // GLfloat vertArray[];
    // GLfloat normArray[len3];
    // GLfloat colorArray[len4];
    // GLfloat texArray[len2];

    // Headboard
    // Let's try drawing everything in render function and do vbos later

}


// Render bed frame
void renderFrame(GLuint *vao, float x, float y, float z, float th, float s)
{
    float postHeight = 2.0;
    float headboardWidth = 0.4;

    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(th, 0, 1, 0);
    glScaled(s, s, s);

    // Headboard: 2 rectangular prisms and arched prism in between

    glBegin(GL_QUADS);
    for (float x = -3.0; x < 3.0; x += 0.25)
    {
        float x2 = x + 0.25;
        float y = -0.1 * (x * x);
        float y2 = -0.1 * (x2 * x2);
        glNormal3f(0, 0, -1);
        glTexCoord2f(x, 0); glVertex3f(x, y + 2, -headboardWidth);
        glTexCoord2f(x2, 0); glVertex3f(x2, y2 + 2, -headboardWidth);
        glTexCoord2f(x2, 0.35); glVertex3f(x2, y2 - 1, -headboardWidth);
        glTexCoord2f(x, 0.35); glVertex3f(x, y - 1, -headboardWidth);

        glNormal3f(0, -1, 0);
        glTexCoord2f(x, 0.35); glVertex3f(x, y - 1, -headboardWidth);
        glTexCoord2f(x2, 0.35); glVertex3f(x2, y2 - 1, -headboardWidth);
        glTexCoord2f(x2, 0.5); glVertex3f(x2, y2 - 1, headboardWidth);
        glTexCoord2f(x, 0.5); glVertex3f(x, y - 1, headboardWidth);

        glNormal3f(0, 0, +1);
        glTexCoord2f(x, 0.5); glVertex3f(x, y - 1, headboardWidth);
        glTexCoord2f(x2, 0.5); glVertex3f(x2, y2 - 1, headboardWidth);
        glTexCoord2f(x2, 0.85); glVertex3f(x2, y2 + 2, headboardWidth);
        glTexCoord2f(x, 0.85); glVertex3f(x, y + 2, headboardWidth);

        glNormal3f(0, 1, 0);
        glTexCoord2f(x, 0.85); glVertex3f(x, y + 2, headboardWidth);
        glTexCoord2f(x2, 0.85); glVertex3f(x2, y2 + 2, headboardWidth);
        glTexCoord2f(x2, 1.0); glVertex3f(x2, y2 + 2, -headboardWidth);
        glTexCoord2f(x, 1.0); glVertex3f(x, y + 2, -headboardWidth);
    }


    // Left Bedpost
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight, 0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight, 0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight, 0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight, 0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,-0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,+0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,-0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,+postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,+postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,-0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(-3,-postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,-postHeight,+0.5);
    //glEnd();


    // Right Bedpost
    //  Front
    //glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight, 0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight, 0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight, 0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight, 0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,-0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,+0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,-0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,+postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,+postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,-0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(+4,-postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,-postHeight,+0.5);


    float length = 10.0;
    // Footboard
    for (float x = -3.0; x < 3.0; x += 0.25)
    {
        float x2 = x + 0.25;
        float y = -0.1 * (x * x);
        float y2 = -0.1 * (x2 * x2);
        glNormal3f(0, 0, -1);
        glTexCoord2f(x, 0); glVertex3f(x, y + 2, length - headboardWidth);
        glTexCoord2f(x2, 0); glVertex3f(x2, y2 + 2, length-headboardWidth);
        glTexCoord2f(x2, 0.35); glVertex3f(x2, y2 - 1, length-headboardWidth);
        glTexCoord2f(x, 0.35); glVertex3f(x, y - 1, length-headboardWidth);

        glNormal3f(0, -1, 0);
        glTexCoord2f(x, 0.35); glVertex3f(x, y - 1, length-headboardWidth);
        glTexCoord2f(x2, 0.35); glVertex3f(x2, y2 - 1, length-headboardWidth);
        glTexCoord2f(x2, 0.5); glVertex3f(x2, y2 - 1, length+headboardWidth);
        glTexCoord2f(x, 0.5); glVertex3f(x, y - 1, length+headboardWidth);

        glNormal3f(0, 0, +1);
        glTexCoord2f(x, 0.5); glVertex3f(x, y - 1, length+headboardWidth);
        glTexCoord2f(x2, 0.5); glVertex3f(x2, y2 - 1, length+headboardWidth);
        glTexCoord2f(x2, 0.85); glVertex3f(x2, y2 + 2, length+headboardWidth);
        glTexCoord2f(x, 0.85); glVertex3f(x, y + 2, length+headboardWidth);

        glNormal3f(0, +1, 0);
        glTexCoord2f(x, 0.85); glVertex3f(x, y + 2, length+headboardWidth);
        glTexCoord2f(x2, 0.85); glVertex3f(x2, y2 + 2, length+headboardWidth);
        glTexCoord2f(x2, 1); glVertex3f(x2, y2 + 2, length-headboardWidth);
        glTexCoord2f(x, 1); glVertex3f(x, y + 2, length-headboardWidth);
    }


    // Left Bedpost
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight, length+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight, length+0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight, length+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight, length+0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,length-0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,length+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,length+0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,length+0.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,length+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,length-0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,+postHeight,length+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,+postHeight,length+0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,length-0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(-3,-postHeight,length+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,-postHeight,length+0.5);
    //glEnd();


    // Right Bedpost
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight, length+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight, length+0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight, length+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight, length+0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,length-0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,length+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,length+0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,length+0.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,length+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,length-0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,+postHeight,length+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,+postHeight,length+0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,length-0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(+4,-postHeight,length+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,-postHeight,length+0.5);


    // Side rail
    //  Front (connected to footboard at length - 0.5)
    //        (connected to headboard at 0.5)
    float sideRailHeight = -1.0;
    postHeight = 0.4;
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-3.75,sideRailHeight-postHeight, length-0.5);
    glTexCoord2f(1,0); glVertex3f(-3.25,sideRailHeight-postHeight, length-0.5);
    glTexCoord2f(1,1); glVertex3f(-3.25,sideRailHeight+postHeight, length-0.5);
    glTexCoord2f(0,1); glVertex3f(-3.75,sideRailHeight+postHeight, length-0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(-3.25,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,0); glVertex3f(-3.75,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,1); glVertex3f(-3.75,sideRailHeight+postHeight,0.5);
    glTexCoord2f(0,1); glVertex3f(-3.25,sideRailHeight+postHeight,0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-3.25,sideRailHeight-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(-3.25,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,1); glVertex3f(-3.25,sideRailHeight+postHeight,0.5);
    glTexCoord2f(0,1); glVertex3f(-3.25,sideRailHeight+postHeight,length-0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-3.75,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,0); glVertex3f(-3.75,sideRailHeight-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(-3.75,sideRailHeight+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(-3.75,sideRailHeight+postHeight,0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(-3.75,sideRailHeight+postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(-3.25,sideRailHeight+postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(-3.25,sideRailHeight+postHeight,0.5);
    glTexCoord2f(0,1); glVertex3f(-3.75,sideRailHeight+postHeight,0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-3.75,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,0); glVertex3f(-3.25,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,1); glVertex3f(-3.25,sideRailHeight-postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(-3.75,sideRailHeight-postHeight,length-0.5);


    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+3.25,sideRailHeight-postHeight, length-0.5);
    glTexCoord2f(1,0); glVertex3f(+3.75,sideRailHeight-postHeight, length-0.5);
    glTexCoord2f(1,1); glVertex3f(+3.75,sideRailHeight+postHeight, length-0.5);
    glTexCoord2f(0,1); glVertex3f(+3.25,sideRailHeight+postHeight, length-0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(+3.75,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,0); glVertex3f(+3.25,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,1); glVertex3f(+3.25,sideRailHeight+postHeight,0.5);
    glTexCoord2f(0,1); glVertex3f(+3.75,sideRailHeight+postHeight,0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+3.75,sideRailHeight-postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(+3.75,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,1); glVertex3f(+3.75,sideRailHeight+postHeight,0.5);
    glTexCoord2f(0,1); glVertex3f(+3.75,sideRailHeight+postHeight,length-0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+3.25,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,0); glVertex3f(+3.25,sideRailHeight-postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(+3.25,sideRailHeight+postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(+3.25,sideRailHeight+postHeight,0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(+3.25,sideRailHeight+postHeight,length-0.5);
    glTexCoord2f(1,0); glVertex3f(+3.75,sideRailHeight+postHeight,length-0.5);
    glTexCoord2f(1,1); glVertex3f(+3.75,sideRailHeight+postHeight,0.5);
    glTexCoord2f(0,1); glVertex3f(+3.25,sideRailHeight+postHeight,0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(+3.25,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,0); glVertex3f(+3.75,sideRailHeight-postHeight,0.5);
    glTexCoord2f(1,1); glVertex3f(+3.75,sideRailHeight-postHeight,length-0.5);
    glTexCoord2f(0,1); glVertex3f(+3.25,sideRailHeight-postHeight,length-0.5);


    // Frame slats
    postHeight = 0.1;
    for (float z = 0.75; z < length - 0.5; z++)
    {
        //  Front (faces footboard)
        glNormal3f(0, 0, 1);
        glTexCoord2f(0,0); glVertex3f(-3.25,sideRailHeight-postHeight, z+0.5);
        glTexCoord2f(1,0); glVertex3f(+3.25,sideRailHeight-postHeight, z+0.5);
        glTexCoord2f(1,1); glVertex3f(+3.25,sideRailHeight+postHeight, z+0.5);
        glTexCoord2f(0,1); glVertex3f(-3.25,sideRailHeight+postHeight, z+0.5);
        //  Back (faces headboard)
        glNormal3f(0, 0, -1);
        glTexCoord2f(0,0); glVertex3f(+3.25,sideRailHeight-postHeight,z);
        glTexCoord2f(1,0); glVertex3f(-3.25,sideRailHeight-postHeight,z);
        glTexCoord2f(1,1); glVertex3f(-3.25,sideRailHeight+postHeight,z);
        glTexCoord2f(0,1); glVertex3f(+3.25,sideRailHeight+postHeight,z);
        //  Right
        glNormal3f(+1, 0, 0);
        glTexCoord2f(0,0); glVertex3f(+3.25,sideRailHeight-postHeight,z+0.5);
        glTexCoord2f(1,0); glVertex3f(+3.25,sideRailHeight-postHeight,z);
        glTexCoord2f(1,1); glVertex3f(+3.25,sideRailHeight+postHeight,z);
        glTexCoord2f(0,1); glVertex3f(+3.25,sideRailHeight+postHeight,z+0.5);
        //  Left
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0,0); glVertex3f(-3.25,sideRailHeight-postHeight,z);
        glTexCoord2f(1,0); glVertex3f(-3.25,sideRailHeight-postHeight,z+0.5);
        glTexCoord2f(1,1); glVertex3f(-3.25,sideRailHeight+postHeight,z+0.5);
        glTexCoord2f(0,1); glVertex3f(-3.25,sideRailHeight+postHeight,z);
        //  Top
        glNormal3f(0, +1, 0);
        glTexCoord2f(0,0); glVertex3f(-3.25,sideRailHeight+postHeight,z+0.5);
        glTexCoord2f(1,0); glVertex3f(+3.25,sideRailHeight+postHeight,z+0.5);
        glTexCoord2f(1,1); glVertex3f(+3.25,sideRailHeight+postHeight,z);
        glTexCoord2f(0,1); glVertex3f(-3.25,sideRailHeight+postHeight,z);
        //  Bottom
        glNormal3f(0, -1, 0);
        glTexCoord2f(0,0); glVertex3f(-3.25,sideRailHeight-postHeight,z);
        glTexCoord2f(1,0); glVertex3f(+3.25,sideRailHeight-postHeight,z);
        glTexCoord2f(1,1); glVertex3f(+3.25,sideRailHeight-postHeight,z+0.5);
        glTexCoord2f(0,1); glVertex3f(-3.25,sideRailHeight-postHeight,z+0.5);
    }
    
    glEnd();

    glPopMatrix();

    glBindVertexArray(0);
    ErrCheck("renderFrame");
}

// Outline of mattress for curved, axisymmetric generation
#define Nm 9
vec2 mattress[Nm] = {
    //{0.0, -2.0},
    //{2.0, -2.0},
    {4.7, -2.0},
    {4.8, -1.9},
    {4.9, -1.6},
    {5.0, -1.0},
    {5.0, 0.0},
    {5.0, 1.0},
    {4.9, 1.6},
    {4.8, 1.9},
    {4.7, 2.0},
    //{2.0, 2.0},
    //{0.0, 2.0},
};
void initMattress(GLfloat *vertArray, GLfloat *normArray, GLfloat *texArray)
{
    vec2 norm[Nm];
    CalcNorm(Nm, mattress, norm);
    int j = 0;
    int k = 0;

    vec2 matRotationArray[21] = {
        {1.0, 0.0},
        {1.0, 1.95},
        {0.975, 1.965},
        {0.925, 1.985},
        {0.9, 2.0},
        {0.0, 2.0},
        {-0.9, 2.0},
        {-0.925, 1.985},
        {-0.975, 1.965},
        {-1.0, 1.95},
        {-1.0, 0.0},
        {-1.0, -1.95},
        {-0.975, -1.965},
        {-0.925, -1.985},
        {-0.9, -2.0},
        {0.0, -2.0},
        {0.9, -2.0},
        {0.925, -1.985},
        {0.975, -1.965},
        {1.0, -1.95},
        {1.0, 0.0},
    };

    // Calculate normals for every quad
    for (int i = 0; i < Nm - 1; i++)
    {
        for (int th = 0; th < 21; th++)
        {
            // float c = Cos(th);
            // float s = Sin(th);
            float c = matRotationArray[th].x;
            float s = matRotationArray[th].y;

            // Each array has Nm * 3 * (360/inc) elements
            // j indexes the array by next element
            vertArray[j]   = c*mattress[i].x;
            vertArray[j+1] = mattress[i].y;
            vertArray[j+2] = s*mattress[i].x;

            vertArray[j+3] = c*mattress[i+1].x;
            vertArray[j+4] = mattress[i+1].y;
            vertArray[j+5] = s*mattress[i+1].x;

            normArray[j]   = c*norm[i].x;
            normArray[j+1] = norm[i].y;
            normArray[j+2] = s*norm[i].x;

            normArray[j+3] = c*norm[i+1].x;
            normArray[j+4] = norm[i+1].y;
            normArray[j+5] = s*norm[i+1].x;


            texArray[k]   = (float)th / 21.0;
            texArray[k+1] = mattress[i].y;
            texArray[k+2] = (float)th / 21.0;
            texArray[k+3] = mattress[i+1].y;

            j += 6;
            k += 4;
        }
    }

    for (float y = -2.0; y < 5.0; y+=4.0)
    {
        // Quad vertices
        vertArray[j]    = -4.7;
        vertArray[j+1]  = y-0.001;
        vertArray[j+2]  = -9.4;

        vertArray[j+3]  = -4.7;
        vertArray[j+4]  = y-0.001;
        vertArray[j+5]  = 9.4;

        vertArray[j+6]  = 4.7;
        vertArray[j+7]  = y-0.001;
        vertArray[j+8]  = 9.4;

        vertArray[j+9]  = 4.7;
        vertArray[j+10] = y-0.001;
        vertArray[j+11] = -9.4;

        // Quad norms (straight up or straight down)
        normArray[j]    = 0.0;
        normArray[j+1]  = y / 2;
        normArray[j+2]  = 0.0;

        normArray[j+3]  = 0.0;
        normArray[j+4]  = y / 2;
        normArray[j+5]  = 0.0;

        normArray[j+6]  = 0.0;
        normArray[j+7]  = y / 2;
        normArray[j+8]  = 0.0;

        normArray[j+9]  = 0.0;
        normArray[j+10] = y / 2;
        normArray[j+11] = 0.0;

        // Quad textures
        texArray[k] = 0.0;
        texArray[k+1] = 0.0;
        
        texArray[k+2] = 1.0;
        texArray[k+3] = 0.0;

        texArray[k+4] = 0.0;
        texArray[k+5] = 1.0;
        
        texArray[k+6] = 1.0;
        texArray[k+7] = 1.0;

        j+=12;
        k+=8;
    }

    ErrCheck("initMattress");
}
void renderMattress(GLfloat *vertArray, GLfloat *normArray, GLfloat *texArray, float x, float y, float z, float th, float s)
{
    s = s / 2;
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(th, 0, 0, 1);
    glScaled(s + 0.33 * s, s - 0.5 * s, s - 0.09 * s);

    int len3 = 1008;
    int numVertices = len3 / 3;

    // Vertices and normals
    int j = 0;
    // Texture coords
    int k = 0;

    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1}; 
    glColor4fv(white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < numVertices; i++)
    {
        glNormal3f(normArray[j], normArray[j+1], normArray[j+2]);
        glVertex3f(vertArray[j], vertArray[j+1], vertArray[j+2]);
        glTexCoord2f(texArray[k], texArray[k+1]);
        j += 3;
        k += 2;
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    for (int i = 0; i < 8; i++)
    {
        glNormal3f(normArray[j], normArray[j+1], normArray[j+2]);
        glVertex3f(vertArray[j], vertArray[j+1], vertArray[j+2]);
        glTexCoord2f(texArray[k], texArray[k+1]);
        j += 3;
        k += 2;
    }
    glEnd();

    glPopMatrix();

    ErrCheck("renderMattress");
}

void initDesk(GLuint *vao, GLuint *vbo);

// Render desk
void renderDesk(GLuint *vao, float x, float y, float z, float th, float s)
{
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(th, 0, 0, 1);
    glScaled(s, s, s);

    int postHeight = 5;

    glBegin(GL_QUADS);
    // Left back leg
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight, 0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight, 0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight, 0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight, 0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,-0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,+0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,-0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,+postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(-3,+postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,-0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(-3,-postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(-4,-postHeight,+0.5);


    // Left front leg
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight, 0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight, 0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight, 0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight, 0.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,-0.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,+0.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,-0.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,+postHeight,+0.5);
    glTexCoord2f(1,0); glVertex3f(+4,+postHeight,+0.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,-0.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,-0.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,-0.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,-0.5);
    glTexCoord2f(1,1); glVertex3f(+4,-postHeight,+0.5);
    glTexCoord2f(0,1); glVertex3f(+3,-postHeight,+0.5);


    // Right back leg
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight, 14.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight, 14.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight, 14.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight, 14.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,13.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,13.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,13.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,13.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-3,-postHeight,14.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,13.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,13.5);
    glTexCoord2f(0,1); glVertex3f(-3,+postHeight,14.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,13.5);
    glTexCoord2f(1,0); glVertex3f(-4,-postHeight,14.5);
    glTexCoord2f(1,1); glVertex3f(-4,+postHeight,14.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,13.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,+postHeight,14.5);
    glTexCoord2f(1,0); glVertex3f(-3,+postHeight,14.5);
    glTexCoord2f(1,1); glVertex3f(-3,+postHeight,13.5);
    glTexCoord2f(0,1); glVertex3f(-4,+postHeight,13.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-4,-postHeight,13.5);
    glTexCoord2f(1,0); glVertex3f(-3,-postHeight,13.5);
    glTexCoord2f(1,1); glVertex3f(-3,-postHeight,14.5);
    glTexCoord2f(0,1); glVertex3f(-4,-postHeight,14.5);


    // Right front leg
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight, 14.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight, 14.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight, 14.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight, 14.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,13.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,13.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,13.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,13.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+4,-postHeight,14.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,13.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,13.5);
    glTexCoord2f(0,1); glVertex3f(+4,+postHeight,14.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,13.5);
    glTexCoord2f(1,0); glVertex3f(+3,-postHeight,14.5);
    glTexCoord2f(1,1); glVertex3f(+3,+postHeight,14.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,13.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,+postHeight,14.5);
    glTexCoord2f(1,0); glVertex3f(+4,+postHeight,14.5);
    glTexCoord2f(1,1); glVertex3f(+4,+postHeight,13.5);
    glTexCoord2f(0,1); glVertex3f(+3,+postHeight,13.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(+3,-postHeight,13.5);
    glTexCoord2f(1,0); glVertex3f(+4,-postHeight,13.5);
    glTexCoord2f(1,1); glVertex3f(+4,-postHeight,14.5);
    glTexCoord2f(0,1); glVertex3f(+3,-postHeight,14.5);


    // Desk Surface
    //  Front
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-5,postHeight, 15.5);
    glTexCoord2f(1,0); glVertex3f(+5,postHeight, 15.5);
    glTexCoord2f(1,1); glVertex3f(+5,postHeight+1, 15.5);
    glTexCoord2f(0,1); glVertex3f(-5,postHeight+1, 15.5);
    //  Back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(+5,postHeight,-1.5);
    glTexCoord2f(1,0); glVertex3f(-5,postHeight,-1.5);
    glTexCoord2f(1,1); glVertex3f(-5,postHeight+1,-1.5);
    glTexCoord2f(0,1); glVertex3f(+5,postHeight+1,-1.5);
    //  Right
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+5,postHeight,15.5);
    glTexCoord2f(1,0); glVertex3f(+5,postHeight,-1.5);
    glTexCoord2f(1,1); glVertex3f(+5,postHeight+1,-1.5);
    glTexCoord2f(0,1); glVertex3f(+5,postHeight+1,15.5);
    //  Left
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-5,postHeight,-1.5);
    glTexCoord2f(1,0); glVertex3f(-5,postHeight,15.5);
    glTexCoord2f(1,1); glVertex3f(-5,postHeight+1,15.5);
    glTexCoord2f(0,1); glVertex3f(-5,postHeight+1,-1.5);
    //  Top
    glNormal3f(0, +1, 0);
    glTexCoord2f(0,0); glVertex3f(-5,postHeight+1,15.5);
    glTexCoord2f(1,0); glVertex3f(+5,postHeight+1,15.5);
    glTexCoord2f(1,1); glVertex3f(+5,postHeight+1,-1.5);
    glTexCoord2f(0,1); glVertex3f(-5,postHeight+1,-1.5);
    //  Bottom
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-5,postHeight,-1.5);
    glTexCoord2f(1,0); glVertex3f(+5,postHeight,-1.5);
    glTexCoord2f(1,1); glVertex3f(+5,postHeight,15.5);
    glTexCoord2f(0,1); glVertex3f(-5,postHeight,15.5);


    glEnd();
    glPopMatrix();

    ErrCheck("renderDesk");
}

void initComputer(GLuint *vao, GLuint *vbo);
void renderComputer(GLuint *vao, float x, float y, float z, float th, float s);

void initKeyboard(GLuint *vao, GLuint *vbo);
void renderKeyboard(GLuint *vao, float x, float y, float z, float th, float s);