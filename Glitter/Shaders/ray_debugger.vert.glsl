#version 330 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;

out VS_OUT
{
    vec3 FragPos; //in view space
    vec2 TexCoords; //in NDC (0 to 1)
} vs_out;

//out vec3 FragPos;
//out vec2 TexCoords;
//out vec3 Normal;

uniform mat4 model;
uniform mat4 cameraView;
uniform mat4 lightView;
uniform mat4 projection;

void main()
{
    vec4 viewPos = cameraView * model * vec4(position, 1.0f);
    gl_Position = projection * viewPos;
    vs_out.FragPos = viewPos.xyz;
    vec4 clipSpace = projection * viewPos;
    vs_out.TexCoords = (clipSpace / clipSpace.w).xy;

    //mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    //Normal = normalMatrix * normal;
}