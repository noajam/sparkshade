//  Shadow Fragment shader
#version 150 compatibility

in vec3 View;
in vec3 Light;
in vec3 Normal;
in vec4 Ambient;
uniform sampler2D tex;
uniform sampler2DShadow depth;

vec4 phong()
{
   //  Emission and ambient color
   vec4 color = 0.2 * Ambient;

   //  Do lighting if not in the shadow
   if (shadow2DProj(depth,gl_TexCoord[1]).a==1.0)
   {
      //  N is the object normal
      vec3 N = normalize(Normal);
      //  L is the light vector
      vec3 L = normalize(Light);

      //  Diffuse light is cosine of light and normal vectors
      float Id = dot(L,N);
      if (Id>0.0)
      {
         //  Add diffuse
         color += Id*gl_FrontLightProduct[0].diffuse;
         //  R is the reflected light vector R = 2(L.N)N - L
         vec3 R = reflect(-L,N);
         //  V is the view vector (eye vector)
         vec3 V = normalize(View);
         //  Specular is cosine of reflected and view vectors
         float Is = dot(R,V);
         if (Is>0.0) color += pow(Is,gl_FrontMaterial.shininess)*gl_FrontLightProduct[0].specular;
      }
   }
   
   //  Return result
   return color;
}

void main()
{
   //  Compute pixel lighting modulated by texture
   gl_FragColor = phong() * texture2D(tex,gl_TexCoord[0].st);
}
