#version 330 core
in vec3 fragmentPosition;

uniform vec3 lightPosition;
uniform float farPlane;
void main()
{             
    // gl_FragDepth = gl_FragCoord.z;

    //distance / farPlane means don't need to worry about w later
    gl_FragDepth = length(fragmentPosition - lightPosition) / farPlane;
}