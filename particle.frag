// Particle fragment shader
#version 150 compatibility

in float vtime;
flat in int render;

void main()
{
   // If particle is dead, discard associated fragments.
   // Otherwise, color from yellow to red based on life.
   float fractTime = fract(vtime);
   if (render == 0)
      discard;
   else
      gl_FragColor = vec4(1.0, 1.0 - fractTime * 2, 1.0 - fractTime * 4, 1.0);
}