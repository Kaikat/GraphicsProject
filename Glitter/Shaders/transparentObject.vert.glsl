#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 viewPos = view * model * vec4(position, 1.0f);
    gl_Position = projection * viewPos;

    //if(position == vec3(0.0))
    //{
     //   FragPos = vec3(0.0);
    //}
    //else
    //{
    FragPos = viewPos.xyz;
    //}
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * normal;
}