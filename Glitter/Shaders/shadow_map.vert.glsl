#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec3 fragmentPosition;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
    fragmentPosition = (model * vec4(position, 1.0)).xyz;
} 