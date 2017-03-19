#version 330 core
layout (location = 0) in vec3 position;

out vec3 SkyTexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{  
    mat4 noTranslationViewMatrix = mat4(mat3(view));
    vec4 pos = projection * noTranslationViewMatrix * vec4(position, 1.0f);
    gl_Position = pos.xyww;
    SkyTexCoords = position;
}