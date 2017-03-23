#version 330 core
//layout (location = 0) out vec3 result;

in vec3 fragmentColor;
out vec4 color;

void main()
{    
    color = vec4(fragmentColor, 1.0f);
    //result = color.xyz;
}