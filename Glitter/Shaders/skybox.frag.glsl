#version 330 core
out vec4 SkyboxColor;
in vec3 SkyTexCoords;

uniform samplerCube skybox;

void main()
{                 
    SkyboxColor = texture(skybox, SkyTexCoords);
}