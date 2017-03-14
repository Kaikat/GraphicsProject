/*#version 330 core
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
	// Output the diffuse texture color of the object
    color = texture(texture_diffuse1, TexCoords);
}*/

#version 330 core
layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;
layout (location = 2) out vec4 albedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    position = FragPos;
    // Also store the per-fragment normals into the gbuffer
    normal = normalize(Normal);
    // And the diffuse per-fragment color
    albedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // Store specular intensity in albedoSpec's alpha component
    albedoSpec.a = texture(texture_specular1, TexCoords).r;
}