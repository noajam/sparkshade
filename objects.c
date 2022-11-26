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
#define Np 102
vec2 bulb[Np] = {
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

vec2 norm[Np];
//  Calculate normals for bulb
void CalcNorm()
{
   //  Calculate normals for each facet
   //  y increases with point index so dy>0
   for (int i=0;i<Np-2;i++)
   {
      // Vector in the plane of the facet
      float dx = bulb[i+1].x - bulb[i].x;
      float dy = bulb[i+1].y - bulb[i].y;
      // Normal is perpendicular
      // dy>0 so normal faces out
      norm[i] = normalize(dy,-dx);
   }
   //  The normal at the last point on the bulb is straight up
   //  This is NOT true in general
   norm[Np-1] = (vec2){0,1};

   //  Average normals of adjacent facets
   //  First and last normal unchanged
   vec2 N2 = norm[0];
   for (int i=1;i<Np-2;i++)
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
    //    (Np * (Nth + 1)) + ((Np - 2) * (Nth + 1))
    //    Np * (Nth + 1) + (Np - 2) * (Nth + 1)
    //    (Nth + 1)(2Np - 2)
    //    2(Np - 1)(Nth + 1)
    // Length of arrays:
    //    4 * 2(Np - 1)(Nth + 1)
    //        8 * 25 * (Np - 1)
    //    3 * 2(Np - 1)(Nth + 1)
    //        6 * 25 * (Np - 1)
    //    2 * 2(Np - 1)(Nth + 1)
    //        4 * 25 * (Np - 1)
    int len4 = 200 * (Np - 1);
    int len3 = 150 * (Np - 1);
    int len2 = 100 * (Np - 1);

    GLfloat vertArray[len3];
    GLfloat normArray[len3];
    GLfloat colorArray[len4];
    GLfloat texArray[len2];
    CalcNorm();
    int inc = 15;
    int j = 0;
    int k = 0;
    int m = 0;

    printf("Made it after init");
    // Calculate normals for every quad
    for (int i = 0; i < Np - 1; i++)
    {
        for (int th = 0; th <= 360; th += inc)
        {
            float c = Cos(th);
            float s = Sin(th);

            // Each array has Np * 3 * (360/inc) elements
            // j indexes the array by next element
            vertArray[j]   = c*bulb[i].x;
            vertArray[j+1] = bulb[i].y;
            vertArray[j+2] = s*bulb[i].x;

            vertArray[j+3] = c*bulb[i+1].x;
            vertArray[j+4] = bulb[i+1].y;
            vertArray[j+5] = s*bulb[i+1].x;

            if (i < 2)
            {
                colorArray[m]   = 0.1;
                colorArray[m+1] = 0.1;
                colorArray[m+2] = 0.1;
                colorArray[m+3] = 1.0;

                colorArray[m+4] = 0.1;
                colorArray[m+5] = 0.1;
                colorArray[m+6] = 0.1;
                colorArray[m+7] = 1.0;
            }
            else if (i < 9)
            {
                colorArray[m]   = 0.5;
                colorArray[m+1] = 0.5;
                colorArray[m+2] = 0.5;
                colorArray[m+3] = 1.0;

                colorArray[m+4] = 0.1;
                colorArray[m+5] = 0.1;
                colorArray[m+6] = 0.1;
                colorArray[m+7] = 1.0;
            }
            else
            {
                colorArray[m]   = 1.0;
                colorArray[m+1] = 1.0;
                colorArray[m+2] = 1.0;
                colorArray[m+3] = 0.3;

                colorArray[m+4] = 1.0;
                colorArray[m+5] = 1.0;
                colorArray[m+6] = 1.0;
                colorArray[m+7] = 0.3;   
            } 

            normArray[j]   = c*norm[i].x;
            normArray[j+1] = norm[i].y;
            normArray[j+2] = s*norm[i].x;

            normArray[j+3] = c*norm[i+1].x;
            normArray[j+4] = norm[i+1].y;
            normArray[j+5] = s*norm[i+1].x;


            texArray[k]   = th / 360.0;
            texArray[k+1] = bulb[i].y;
            texArray[k+2] = th / 360.0;
            texArray[k+3] = bulb[i+1].y;

            m += 8;
            j += 6;
            k += 4;
        }
    }

    // reverse(vertArray, len3);
    // reverse(normArray, len3);
    //reverse(colorArray, len4);

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

    // // Bind normal buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    // glBufferData(GL_ARRAY_BUFFER, len3 * sizeof(GLfloat), normArray, GL_STATIC_DRAW);

    // // Assign normal buffer to second attribute array
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // // Bind color buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // glBufferData(GL_ARRAY_BUFFER, len4 * sizeof(GLfloat), colorArray, GL_STATIC_DRAW);

    // // Assign color buffer to third attribute array
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // // Bind texture buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // glBufferData(GL_ARRAY_BUFFER, len2 * sizeof(GLfloat), texArray, GL_STATIC_DRAW);

    // // Assign texture buffer to third attribute array
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    ErrCheck("initBulb");  
}

void renderBulb(GLuint *vao, float x, float y, float z, float th, float ph, float s)
{
    glBindVertexArray(*vao);

    float white[] = {1,1,1,0.9};
    float black[] = {0.0, 0.0, 0.0, 1.0};
    float achrome[] = {0.25,0.25,0.25,1.0};
    float dchrome[] = {0.4,0.4,0.4,1.0};
    float schrome[] = {0.774597,0.774597,0.774597,1.0};
    float Emission[]  = {0.2,0.2,0.05,1.0};

    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(th, 0, 0, 1);
    glRotated(ph, 1, 0, 0);
    glScaled(s, s, s);

    int numVertices = 50 * (Np - 1);

    // Draw bulb
    glColor4fv(white);

    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    glDrawArrays(GL_QUAD_STRIP, 450, numVertices - 450);

    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128*0.6);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,achrome);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,dchrome);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,schrome);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);

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
    int postHeight = 5;

    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(th, 0, 1, 0);
    glScaled(s, s, s);

    // Headboard: 2 rectangular prisms and arched prism in between


    glBegin(GL_QUADS);
    // glVertex3f(-3.0, -0.1 * -3.0 * -3.0 + 2, -0.5);
    // glVertex3f(-3.0, -0.1 * -3.0 * -3.0 + 2, 0.5);
    for (float x = -3.0; x < 3.0; x += 0.25)
    {
        float x2 = x + 0.25;
        float y = -0.1 * (x * x);
        float y2 = -0.1 * (x2 * x2);
        glNormal3f(0, 0, -1);
        glVertex3f(x, y + 2, -0.5);
        glVertex3f(x2, y2 + 2, -0.5);

        glVertex3f(x2, y2 - 1, -0.5);
        glVertex3f(x, y - 1, -0.5);

        // Normal vector is perpendicular to tangent vector
        // y = -0.1x^2
        // tangent vector = <1, -0.2x>
        // tangent = <x, -0.2x)
        // tangent = -0.2 * x
        // x = -3.0 -> (-3.0, 0.6)
        // normal = normalize(0.6, 3.0)
        // normal vector = normalize(dy, -dx) = normalize(-0.2x, -1)
        vec2 downNormal1 = normalize(-0.2 * x, -x);
        vec2 downNormal2 = normalize(-0.2 * x2, -x2);

        glNormal3f(0, -1, 0);
        glVertex3f(x, y - 1, -0.5);
        //glNormal3f(-downNormal2.x, -downNormal2.y, 0);
        glVertex3f(x2, y2 - 1, -0.5);

        //glNormal3f(-downNormal1.x, -downNormal1.y, 0);
        glVertex3f(x2, y2 - 1, 0.5);
        //glNormal3f(-downNormal2.x, -downNormal2.y, 0);
        glVertex3f(x, y - 1, 0.5);

        glNormal3f(0, 0, +1);
        glVertex3f(x, y - 1, 0.5);
        glVertex3f(x2, y2 - 1, 0.5);
        glVertex3f(x2, y2 + 2, 0.5);
        glVertex3f(x, y + 2, 0.5);

        glNormal3f(0, 1, 0);
        glVertex3f(x, y + 2, 0.5);
        //glNormal3f(-downNormal2.x, -downNormal2.y, 0);
        glVertex3f(x2, y2 + 2, 0.5);

        //glNormal3f(-downNormal1.x, -downNormal1.y, 0);
        glVertex3f(x2, y2 + 2, -0.5);
        //glNormal3f(-downNormal2.x, -downNormal2.y, 0);
        glVertex3f(x, y + 2, -0.5);
    }
    
    glEnd();



    // //  Front
    // //SetColor(1,0,0,32);
    // glNormal3f( 0, 0, 1);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0); glVertex3f(-1,-postHeight, 1);
    // glTexCoord2f(1,0); glVertex3f(+1,-postHeight, 1);
    // glTexCoord2f(1,1); glVertex3f(+1,+postHeight, 1);
    // glTexCoord2f(0,1); glVertex3f(-1,+postHeight, 1);
    // glEnd();
    // //  Back
    // //SetColor(0,0,1,32);
    // glNormal3f( 0, 0,-1);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0); glVertex3f(+1,-postHeight,-1);
    // glTexCoord2f(1,0); glVertex3f(-1,-postHeight,-1);
    // glTexCoord2f(1,1); glVertex3f(-1,+postHeight,-1);
    // glTexCoord2f(0,1); glVertex3f(+1,+postHeight,-1);
    // glEnd();
    // //  Right
    // //SetColor(1,1,0,32);
    // glNormal3f(+1, 0, 0);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0); glVertex3f(+1,-postHeight,+1);
    // glTexCoord2f(1,0); glVertex3f(+1,-postHeight,-1);
    // glTexCoord2f(1,1); glVertex3f(+1,+postHeight,-1);
    // glTexCoord2f(0,1); glVertex3f(+1,+postHeight,+1);
    // glEnd();
    // //  Left
    // //SetColor(0,1,0,32);
    // glNormal3f(-1, 0, 0);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0); glVertex3f(-1,-postHeight,-1);
    // glTexCoord2f(1,0); glVertex3f(-1,-postHeight,+1);
    // glTexCoord2f(1,1); glVertex3f(-1,+postHeight,+1);
    // glTexCoord2f(0,1); glVertex3f(-1,+postHeight,-1);
    // glEnd();
    // //  Top
    // //SetColor(0,1,1,32);
    // glNormal3f( 0,+1, 0);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0); glVertex3f(-1,+postHeight,+1);
    // glTexCoord2f(1,0); glVertex3f(+1,+postHeight,+1);
    // glTexCoord2f(1,1); glVertex3f(+1,+postHeight,-1);
    // glTexCoord2f(0,1); glVertex3f(-1,+postHeight,-1);
    // glEnd();
    // //  Bottom
    // //SetColor(1,0,1,32);
    // glNormal3f( 0,-1, 0);
    // glBegin(GL_QUADS);
    // glTexCoord2f(0,0); glVertex3f(-1,-postHeight,-1);
    // glTexCoord2f(1,0); glVertex3f(+1,-postHeight,-1);
    // glTexCoord2f(1,1); glVertex3f(+1,-postHeight,+1);
    // glTexCoord2f(0,1); glVertex3f(-1,-postHeight,+1);
    // glEnd();

    glPopMatrix();

    glBindVertexArray(0);
    ErrCheck("renderFrame");
}

void initMatress(GLuint *vao, GLuint *vbo);
void renderMatress(GLuint *vao, float x, float y, float z, float th, float s);

void initDesk(GLuint *vao, GLuint *vbo);
void renderDesk(GLuint *vao, float x, float y, float z, float th, float s);

void initComputer(GLuint *vao, GLuint *vbo);
void renderComputer(GLuint *vao, float x, float y, float z, float th, float s);

void initKeyboard(GLuint *vao, GLuint *vbo);
void renderKeyboard(GLuint *vao, float x, float y, float z, float th, float s);