#version 330 core
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoords;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

//out vec3 FragPos;
out vec2 TexCoords;
//out vec3 Normal;
//out vec4 FragPosViewSpace;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform mat4 lightSpace;

void main()
{
   // vec4 viewPos = view * model * vec4(position, 1.0f);
    
    gl_Position = vec4(position, 1.0f); //projection * viewPos;
    TexCoords = uv;
    //FragPos = viewPos.xyz;
    // FragPosLightSpace = lightSpace * vec4(FragPos, 1.0f);
    //FragPosViewSpace = inverse(view) * vec4(FragPos, 1.0);

   // mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    //Normal = normalMatrix * normal;
}