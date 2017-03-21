#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 TexCoords;

//uniform mat4 view;
//uniform mat4 projection;

void main()
{  
    //mat4 noTranslationViewMatrix = mat4(mat3(view));
    //vec4 pos = projection * noTranslationViewMatrix * vec4(position, 1.0f);
    gl_Position = vec4(position, 1.0);
    TexCoords = uv;
}