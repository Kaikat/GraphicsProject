#version 330 core
layout (location = 0) out vec3 position;
layout (location = 1) out vec3 normal;

//gl cullface for front and back

//in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

void main()
{    
    vec3 yellow = vec3(1.0, 1.0, 0.0);
    vec3 red = vec3(1.0, 0.0, 0.0);

    // Store the fragment position vector in the first gbuffer texture
    position = FragPos;
    // Also store the per-fragment normals into the gbuffer
    normal = normalize(Normal);
    // And the diffuse per-fragment color
   // albedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // Store specular intensity in albedoSpec's alpha component
    //albedoSpec.a = texture(texture_specular1, TexCoords).r;
}