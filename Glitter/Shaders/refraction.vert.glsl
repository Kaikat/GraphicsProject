#version 330 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;

out vec3 FragPos;
out vec2 TexCoords;
//out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 viewPos = view * model * vec4(position, 1.0f);
    gl_Position = projection * viewPos;
    FragPos = viewPos.xyz;
    vec4 clipSpace = projection * viewPos;
    TexCoords = ((clipSpace / clipSpace.w).xy + 1.0) / 2.0;

    //mat3 normalMatrix = transpose(inverse(mat3(view * model)));
   // Normal = normalMatrix * normal;
}