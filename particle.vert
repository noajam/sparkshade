// Particle vertex shader
#version 120

varying float lifespan;
varying float life;

void main()
{
    /*
        Possibility:
        When a particle dies, reset the velocity and life, but everything else stays the same.
        In this vertex shader, take a time attribute as input and move the vertex based on the
        particle's initial position along the velocity + gravity vectors. Actually nix this.

        Instead, the life variable is the time. As life goes down, time goes up. Time will be
        represented as the delta between initial life and current life. Scale it as necessary
        to increase velocity. Actually, haha, this isn't necessary. Might as well just make
        an extra float variable called time and call it good. Just increment the time and
        decrement the life.
    */

    //  Set vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}