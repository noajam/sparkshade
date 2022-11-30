// Particle vertex shader
#version 150 compatibility

in float life;
in float time;
in vec3 velocity;

uniform int renderParticles[200];

out float vtime;
flat out int render;

void main()
{
    // Decide whether to render particles and send to fragment shader
    render = renderParticles[gl_VertexID];
    vtime = time;

    // Back points are odd vertices, returns 1
    // Front points are even vertices, returns 0
    int isBackPoint = int(mod(gl_VertexID, 2));

    float fractTime = fract(time);

    // front point = velocity * fractTime;
    // back point = velocity * (fractTime - 0.01)
    // (velocity * fractTime) - (velocity * (fractTime - 0.01))
    // velocity * (fractTime - (fractTime - 0.01));
    // velocity * (0.01) = difference in distance between front and back point
    // Take a percentage of this based on fractTime and add to back point
    //vec3 backPointRecovery = isBackPoint * velocity * 0.001 * fractTime * 2;
    // Change vertex from position based on velocity and time
    vec4 kinematicTranslation = vec4((velocity * fractTime), 1.0);

    // Acceleration from gravity
    //kinematicTranslation.y -= 2.0 * fractTime * fractTime;
    //kinematicTranslation.y += 2.5;
    //gl_PointSize = 2.0;

    //  Set vertex position
    gl_Position = gl_ModelViewProjectionMatrix * kinematicTranslation;
    gl_Position.x = clamp(gl_Position.x, -5.0, 5.0);
    gl_Position.y = clamp(gl_Position.y, -5.0, 5.0);
    gl_Position.z = clamp(gl_Position.z, -5.0, 5.0);
}