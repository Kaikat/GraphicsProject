#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D brdfResult;
uniform sampler2D ssaoResult;

void main()
{
    FragColor = texture(brdfResult, TexCoords) * texture(ssaoResult, TexCoords);
}