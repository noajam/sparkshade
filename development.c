/*
 *  Shadow Map with Shaders
 *    with Frame Buffer Depth Texture by Jay Kominek
 *
 *  Demonstate shadows using the shadow map algorithm using a shader.
 *
 *  Key bindings:
 *  m/M        Show/hide shadow map
 *  o/O        Cycle through objects
 *  +/-        Change light elevation
 *  []         Change light position
 *  s/S        Start/stop light movement
 *  l/L        Toggle teapot lid stretch
 *  <>         Decrease/increase number of slices in objects
 *  b/B        Toggle room box
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
#include "objects.h"
#include "particle.h"

typedef struct {float x,y,z;} Point;
//  Global variables
int          fp=0;      // First Person mode
int          mode=0;    // Display mode
int          obj=15;    // Display objects (bitmap)
int          move=1;    // Light movement
int          axes=1;    // Display axes
int          box=1;     // Display enclosing box
int          n=8;       // Number of slices
int          th=180;    // Azimuth of view angle
int          ph=+30;    // Elevation of view angle
int          tex2d[4];  // Textures (names)
int          dt=50;     // Timer period (ms)
double       asp=1;     // Aspect ratio
double       dim=3;     // Size of world
int          zh=0;      // Light azimuth
float        Ylight=2.5;  // Elevation of light
float        Lpos[4];   // Light position
unsigned int framebuf=0;// Frame buffer id
double       Svec[4];   // Texture planes S
double       Tvec[4];   // Texture planes T
double       Rvec[4];   // Texture planes R
double       Qvec[4];   // Texture planes Q
double       gEyeP[3];  // Global eye position for FP
int          Width;     // Window width
int          Height;    // Window height
int          shadowdim; // Size of shadow map textures
int          shader;    // Shadow shader
int          particleShader; // Particle shader
const char* text[]={"Shadows","Shadow Map"};

GLuint bulbVAO;
GLuint bulbVBO[3];

GLuint matressVAO;
GLuint matressVBO[3];

#define Mlen3 1032
#define Mlen2 688
GLfloat mattressVertices[Mlen3];
GLfloat mattressNormals[Mlen3];
GLfloat mattressTexCoords[Mlen2];

ParticleSystem pSystem;
GLuint pSystemVAO;
GLuint pSystemVBO[3];
int pSystemState = 0;
int pSystemTransition = 0;


#define MAXN 64    // Maximum number of slices (n) and points in a polygon

/*
 * Set Color and Material Properties
 */
static void SetColor(float R,float G,float B,float S)
{
   glColor3f(R,G,B);
   float color[] = {R,G,B,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,color);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,&S);
}


/*
 *  Draw a wall
 */
static void Wall(float x,float y,float z, float th,float ph , float Sx,float Sy,float Sz , float St)
{
   float s=1.0/n;
   float t=0.5*St/n;

   //  Transform
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScaled(Sx,Sy,Sz);

   //  Draw walls
   glNormal3f(0,0,1);
   for (int j=-n;j<n;j++)
   {
      glBegin(GL_QUAD_STRIP);
      for (int i=-n;i<=n;i++)
      {
         glTexCoord2f((i+n)*t,(j  +n)*t); glVertex3f(i*s,    j*s,-1);
         glTexCoord2f((i+n)*t,(j+1+n)*t); glVertex3f(i*s,(j+1)*s,-1);
      }
      glEnd();
   }

   //  Restore
   glPopMatrix();
}

/*
 *  Set light
 *    light>0 bright
 *    light<0 dim
 *    light=0 off
 */
static void Light(int light)
{
   //  Set light position
   Lpos[0] = 0.5*Cos(zh);
   Lpos[1] = Ylight;
   Lpos[2] = 0.5*Sin(zh);
   Lpos[3] = 1;

   //  Enable lighting
   if (light)
   {
      float Med[]  = {0.2,0.2,0.1,1.0};
      float High[] = {0.9,0.9,0.8,1.0};
      //  Enable lighting with normalization
      glEnable(GL_LIGHTING);
      glEnable(GL_NORMALIZE);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_POSITION,Lpos);
      glLightfv(GL_LIGHT0,GL_AMBIENT,Med);
      glLightfv(GL_LIGHT0,GL_DIFFUSE,High);
   }
   else
   {
      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_NORMALIZE);
   }
}

/*
 *  Draw scene
 *    light (true enables lighting)
 */
void Scene(int light)
{
   //  Set light position and properties
   Light(light);
 
   //  Enable textures if lit
   if (light)
   {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,tex2d[2]);
   }

   // Bed goes to (+x, +z) corner of room
   // Desk goes to (-x, +z) corner of room

   float scale = 0.85;
   SetColor(1,1,1,32);
   renderFrame(0, 4.2, -4.3, -2.1, 0, scale);
   renderDesk(0, -4, -4.5, 2, 0, 0.3);
   SetColor(1,1,1,2);
   glBindTexture(GL_TEXTURE_2D, tex2d[3]);
   renderMattress(mattressVertices, mattressNormals, mattressTexCoords, 4.3, -4.4, 2.22, 0, scale);


   //  Disable textures
   if (light) glDisable(GL_TEXTURE_2D);

   //  The floor, ceiling and walls don't cast a shadow, so bail here
   if (!light) return;

   //  Enable textures for floor, ceiling and walls
   glEnable(GL_TEXTURE_2D);

   //  Wood texture for floor and ceiling
   glBindTexture(GL_TEXTURE_2D,tex2d[0]);
   if(box != 0)
   {
      SetColor(0.85,0.85,0.85,4);
      for (int k=-1;k<=box;k+=2)
         Wall(0,0,0, 0,90*k , 8,8,box?6:2 , 4);
      //  Concrete texture for walls
      glBindTexture(GL_TEXTURE_2D,tex2d[1]);
      SetColor(0.3,0.3,0.3,1);
      for (int k=0;k<4*box;k++)
         Wall(0,0,0, 90*k,0 , 8,box?6:2,8 , 4);
   }
   //  Disable textures
   glDisable(GL_TEXTURE_2D);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{

   //  Erase the window and the depth buffers
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Disable lighting
   glDisable(GL_LIGHTING);

   //
   //  Draw the scene with shadows
   //
   //  Set perspective view
   if (fp)
   {
      // //  First person
      // float Ex = gEyeP[0];
      // float Ey = gEyeP[1];
      // float Ez = gEyeP[2];
      // float Cx = Ex + Cos(th) * Cos(ph);
      // float Cy = Ey + Sin(ph);
      // float Cz = Ez + Sin(th) * Cos(ph);
      // gluLookAt(Ex,Ey,Ez , Cx,Cy,Cz, 0,Cos(ph),0);
      // //  Full width
      // //Project(60,asp,dim);
      // //glViewport(0,0,RES*Width,RES*Height);
   }
   else
   {
      //  Eye position
      float Ex = -2*dim*Sin(th)*Cos(ph);
      float Ey = +2*dim        *Sin(ph);
      float Ez = +2*dim*Cos(th)*Cos(ph);
      if (mode)
      {
         //  Half width for shadow map display
         Project(60,asp/2,dim);
         glViewport(0,0,RES*Width/2,RES*Height);
      }
      else
      {
         //  Full width
         Project(60,asp,dim);
         glViewport(0,0,RES*Width,RES*Height);
      }
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }

   //  Enable shdaow shader program
   glUseProgram(shader);
   int id = glGetUniformLocation(shader,"tex");
   if (id>=0) glUniform1i(id,0);
   id = glGetUniformLocation(shader,"depth");
   if (id>=0) glUniform1i(id,1);

   // Set up the eye plane for projecting the shadow map on the scene
   glActiveTexture(GL_TEXTURE1);
   glTexGendv(GL_S,GL_EYE_PLANE,Svec);
   glTexGendv(GL_T,GL_EYE_PLANE,Tvec);
   glTexGendv(GL_R,GL_EYE_PLANE,Rvec);
   glTexGendv(GL_Q,GL_EYE_PLANE,Qvec);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);
   glActiveTexture(GL_TEXTURE0);

   // Draw objects in the scene (including walls)
   Scene(1);

   //  Disable shadow shader program
   glUseProgram(0);

   // Enable particle shader and display particle system
   glUseProgram(particleShader);
   id = glGetUniformLocation(particleShader, "renderParticles");
   if (id>=0) glUniform1iv(id, 2*numParticles, pSystem.active);
   

   renderParticleSystem(&pSystemVAO, numParticles, Lpos[0], Lpos[1] + 0.5, Lpos[2]);
   bufferUpdateParticleSystem(&pSystem, numParticles, pSystemVBO, pSystemState, &pSystemTransition);

   // Disable particle shader program
   glUseProgram(0);
   glDisable(GL_LIGHTING);
   renderBulb(&bulbVAO, Lpos[0], Lpos[1], Lpos[2], 180 + (7.5*Cos(zh)), 7.5*Sin(zh), 0.2);

   //  Draw axes (white)
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0;
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //
   //  Show the shadow map
   //
   if (mode)
   {
      //  Orthogonal view (right half)
      Project(0,asp/2,1);
      glViewport(RES*Width/2+1,0,RES*Width/2,RES*Height);
      //  Disable any manipulation of textures
      glActiveTexture(GL_TEXTURE1);
      glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_NONE);
      //  Display texture by drawing quad
      glEnable(GL_TEXTURE_2D);
      glColor3f(1,1,1);
      glBegin(GL_QUADS);
      glMultiTexCoord2f(GL_TEXTURE1,0,0);glVertex2f(-1,-1);
      glMultiTexCoord2f(GL_TEXTURE1,1,0);glVertex2f(+1,-1);
      glMultiTexCoord2f(GL_TEXTURE1,1,1);glVertex2f(+1,+1);
      glMultiTexCoord2f(GL_TEXTURE1,0,1);glVertex2f(-1,+1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
      //  Switch back to default texture unit
      glActiveTexture(GL_TEXTURE0);
      //  Show buffer info
      int n,ix=Width/2+5,iy=Height-5;
      glColor3f(1,0,0);
      glWindowPos2i(ix,iy-=20);
      glGetIntegerv(GL_MAX_TEXTURE_UNITS,&n);
      Print("Maximum Texture Units %d\n",n);
      glGetIntegerv(GL_MAX_TEXTURE_SIZE,&n);
      glWindowPos2i(ix,iy-=20);
      Print("Maximum Texture Size %d\n",n);
      glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&n);
      glWindowPos2i(ix,iy-=20);
      Print("Maximum Buffer Size  %d\n",n);
      glWindowPos2i(ix,iy-=20);
      Print("Shadow Texture Size %dx%d\n",shadowdim,shadowdim);
   }

   //  Display parameters
   glColor3f(1,1,1);
   glWindowPos2i(5,5);
   Print("Ylight=%.1f Angle=%d,%d,%d  Dim=%.1f Slices=%d Mode=%s",
     Ylight,th,ph,zh,dim,n,text[mode]);

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  Build Shadow Map
 */
void ShadowMap(void)
{
   double Lmodel[16];  //  Light modelview matrix
   double Lproj[16];   //  Light projection matrix
   double Tproj[16];   //  Texture projection matrix
   double Dim=5.0;     //  Bounding radius of scene
   double Ldist;       //  Distance from light to scene center

   //  Save transforms and modes
   glPushMatrix();
   glPushAttrib(GL_TRANSFORM_BIT|GL_ENABLE_BIT);
   //  No write to color buffer and no smoothing
   glShadeModel(GL_FLAT);
   glColorMask(0,0,0,0);
   // Overcome imprecision
   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Turn off lighting and set light position
   Light(0);

   //  Light distance
   Ldist = sqrt(Lpos[0]*Lpos[0] + Lpos[1]*Lpos[1] + Lpos[2]*Lpos[2]);
   if (Ldist<1.1*Dim) Ldist = 1.1*Dim;

   //  Set perspective view from light position
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(300.0*atan(Dim/Ldist),1,Ldist-Dim,Ldist+Dim);
   //gluPerspective(114.6*atan(Dim/Ldist),1,Ldist-Dim,Ldist+Dim);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(Lpos[0],Lpos[1],Lpos[2] , 0,0,0 , 0,1,0);
   //  Size viewport to desired dimensions
   glViewport(0,0,shadowdim,shadowdim);

   // Redirect traffic to the frame buffer
   glBindFramebuffer(GL_FRAMEBUFFER,framebuf);

   // Clear the depth buffer
   glClear(GL_DEPTH_BUFFER_BIT);
   // Draw all objects that can cast a shadow
   Scene(0);

   //  Retrieve light projection and modelview matrices
   glGetDoublev(GL_PROJECTION_MATRIX,Lproj);
   glGetDoublev(GL_MODELVIEW_MATRIX,Lmodel);

   // Set up texture matrix for shadow map projection,
   // which will be rolled into the eye linear
   // texture coordinate generation plane equations
   glLoadIdentity();
   glTranslated(0.5,0.5,0.5);
   glScaled(0.5,0.5,0.5);
   glMultMatrixd(Lproj);
   glMultMatrixd(Lmodel);

   // Retrieve result and transpose to get the s, t, r, and q rows for plane equations
   glGetDoublev(GL_MODELVIEW_MATRIX,Tproj);
   Svec[0] = Tproj[0];    Tvec[0] = Tproj[1];    Rvec[0] = Tproj[2];    Qvec[0] = Tproj[3];
   Svec[1] = Tproj[4];    Tvec[1] = Tproj[5];    Rvec[1] = Tproj[6];    Qvec[1] = Tproj[7];
   Svec[2] = Tproj[8];    Tvec[2] = Tproj[9];    Rvec[2] = Tproj[10];   Qvec[2] = Tproj[11];
   Svec[3] = Tproj[12];   Tvec[3] = Tproj[13];   Rvec[3] = Tproj[14];   Qvec[3] = Tproj[15];

   // Restore normal drawing state
   glShadeModel(GL_SMOOTH);
   glColorMask(1,1,1,1);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glPopAttrib();
   glPopMatrix();
   glBindFramebuffer(GL_FRAMEBUFFER,0);

   //  Check if something went wrong
   ErrCheck("ShadowMap");
}

/*
 *
 */
void InitMap()
{
   unsigned int shadowtex; //  Shadow buffer texture id
   int n;

   //  Make sure multi-textures are supported
   glGetIntegerv(GL_MAX_TEXTURE_UNITS,&n);
   if (n<2) Fatal("Multiple textures not supported\n");

   //  Get maximum texture buffer size
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&shadowdim);
   //  Limit texture size to maximum buffer size
   glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&n);
   if (shadowdim>n) shadowdim = n;
   //  Limit texture size to 2048 for performance
   if (shadowdim>2048) shadowdim = 2048;
   if (shadowdim<512) Fatal("Shadow map dimensions too small %d\n",shadowdim);

   //  Do Shadow textures in MultiTexture 1
   glActiveTexture(GL_TEXTURE1);

   //  Allocate and bind shadow texture
   glGenTextures(1,&shadowtex);
   glBindTexture(GL_TEXTURE_2D,shadowtex);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowdim, shadowdim, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

   //  Map single depth value to RGBA (this is called intensity)
   glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY);

   //  Set texture mapping to clamp and linear interpolation
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

   //  Set automatic texture generation mode to Eye Linear
   glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
   glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
   glTexGeni(GL_R,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);
   glTexGeni(GL_Q,GL_TEXTURE_GEN_MODE,GL_EYE_LINEAR);

   // Switch back to default textures
   glActiveTexture(GL_TEXTURE0);

   // Attach shadow texture to frame buffer
   glGenFramebuffers(1,&framebuf);
   glBindFramebuffer(GL_FRAMEBUFFER,framebuf);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowtex, 0);
   //  Don't write or read to visible color buffer
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   //  Make sure this all worked
   if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Fatal("Error setting up frame buffer\n");
   glBindFramebuffer(GL_FRAMEBUFFER,0);

   //  Check if something went wrong
   ErrCheck("InitMap");

   //  Create shadow map
   ShadowMap();
}

/*
 *  GLUT calls this routine when nothing else is going on
 */
void idle(int k)
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(60*t,1440.0);
   //  Update shadow map
   ShadowMap();
   //bufferUpdateParticleSystem(&pSystem, numParticles, pSystemVBO, pSystemState, &pSystemTransition);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
   //  Schedule update
   if (move) glutTimerFunc(dt,idle,0);
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 1;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 1;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 1;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 1;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void timer(int t)
{
   if (fp)
   {
      //glutPostRedisplay();
      glutWarpPointer(Width/2,Height/2);
      glutTimerFunc(1000/60.0, timer, 0);
   }
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'k' || ch == 'K')
      axes = 1-axes;
   //  Toggle display modes
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'l' || ch == 'L')
      move = 1-move;
   //  Toggle box
   else if (ch == 'b' || ch == 'B')
      box = 1-box;
   // Toggle particle effects
   else if (ch=='t' || ch=='T')
   {
      pSystemTransition = 1;
      pSystemState = 1 - pSystemState;
   }
   // Toggle first person (currently bugged, do not use)
   else if (ch=='f' || ch=='F')
   {
      fp = 1-fp;
      if (fp)
      {
         gEyeP[0] = 0.0;
         gEyeP[1] = 0.0;
         gEyeP[2] = 0.0;
         th = 0;
         ph = 0;
         glutTimerFunc(0, timer, 0);
         glutSetCursor(GLUT_CURSOR_NONE);
         //glViewport(0,0,RES*Width,RES*Height);
         //Project(60, asp, dim);
      }
      else
      {
         th=-45;
         ph=45;
         glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
      }
   }
   //  Moving forwards in first person
   else if (ch == 'w' || ch == 'W')
   {
      gEyeP[0] += Cos(th);
      gEyeP[2] += Sin(th);
   }
   //  Moving left in first person
   else if (ch == 'a' || ch == 'A')
   {
      gEyeP[0] += Cos(th+270);
      gEyeP[2] += Sin(th+270);
   }
   //  Moving backwards in first person
   else if (ch == 's' || ch == 'S')
   {
      gEyeP[0] += Cos(th+180);
      gEyeP[2] += Sin(th+180);
   }
   //  Moving right in first person
   else if (ch == 'd' || ch == 'D')
   {
      gEyeP[0] += Cos(th+90);
      gEyeP[2] += Sin(th+90);
   }
   else if (ch=='q' || ch=='Q')
      dim += 0.1;
   else if (ch=='e' || ch=='E')
      dim -= 0.1;
   //  Restart animation
   if ((ch =='l' || ch == 'L') && move) glutTimerFunc(dt,idle,0);
   //  Update screen size when mode changes
   if (ch == 'm' || ch == 'M') glutReshapeWindow(mode?2*Width:Width/2,Height);
   //  Update shadow map if light position or objects changed
   if (strchr("<>oO-+[]",ch)) ShadowMap();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Store window dimensions
   Width  = width;
   Height = height;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(60,asp,dim);
}

//
//  Read text file
//
static char* ReadText(const char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rb");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);

   //printf("%i\n", n);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   //printf(buffer);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   // size_t blah = fread(buffer,1,n,f);
   // buffer[n] = 0;
   // printf("%zu\n", blah);
   // printf(buffer);
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

//
//  Print Shader Log
//
static void PrintShaderLog(int obj,const char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

//
//  Print Program Log
//
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

//
//  Create Shader
//
void CreateShader(int prog,const GLenum type,const char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Attach to shader program
   glAttachShader(prog,shader);
}

//
//  Create Shader Program
//
int CreateShaderProg(const char* VertFile,const char* FragFile, int shaderType)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   if (VertFile) CreateShader(prog,GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   if (FragFile) CreateShader(prog,GL_FRAGMENT_SHADER,FragFile);

   if (shaderType)
   {
      glBindVertexArray(pSystemVAO);

      glBindAttribLocation(prog, 0, "time");
      glBindAttribLocation(prog, 1, "velocity");

      glBindVertexArray(0);
   }
   
   
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

// /*
//  *  GLUT calls this routine to detect passive mouse motion
//  */
// void passive(int x, int y)
// {
//    if (fp)
//    {
//       int deltaX = (Width/2) - x;
//       int deltaY = (Height/2) - y;

//       th -= (float)deltaX/50.0;
//       ph += (float)deltaY/50.0;
//       th %= 360;
//       ph %= 360;
//       // Project(200,asp,dim);
//       glutPostRedisplay();
//    }
// }

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Shadow Map Shader");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //glutPassiveMotionFunc(passive);
   glutTimerFunc(dt,idle,0);
   //  Load textures
   tex2d[0] = LoadTexBMP("wood_floor.bmp");
   tex2d[1] = LoadTexBMP("concrete_blocks.bmp");
   tex2d[2] = LoadTexBMP("wood.bmp");
   tex2d[3] = LoadTexBMP("blanket.bmp");
   // Enable Z-buffer
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);
   glPolygonOffset(4,0);
   //  Initialize texture map
   shader = CreateShaderProg("shadow.vert","shadow.frag", 0);

   // Set up particle system
   initParticleSystem(&pSystem, numParticles, &pSystemVAO, pSystemVBO, particleShader);
   glEnable(GL_PROGRAM_POINT_SIZE);
   particleShader = CreateShaderProg("particle.vert", "particle.frag", 1);
   initBulb(&bulbVAO, bulbVBO);
   initMattress(mattressVertices, mattressNormals, mattressTexCoords);
   initParticleSystem(&pSystem, numParticles, &pSystemVAO, pSystemVBO, particleShader);

   //  Initialize texture map
   InitMap();
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
