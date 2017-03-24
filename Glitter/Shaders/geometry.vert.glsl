#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec4 VertexPos; //in view space
   // vec2 TexCoords; //in NDC (0 to 1)
} vs_out;

void main()
{
    vec4 viewPos = view * model * vec4(position, 1.0f);
    gl_Position = projection * viewPos;
    TexCoords = texCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * normal;

    vs_out.VertexPos = projection * viewPos;
}