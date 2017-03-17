#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform mat4 view;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

const int NR_LIGHTS = 3;

#define AMBIENT_STRENGTH 0.1

struct Light {
    vec3 Position;
    vec3 Color;
    float Intensity;
};
uniform Light lights[NR_LIGHTS];
uniform samplerCube shadowMap[NR_LIGHTS];
uniform float farPlane;

float isShadow(int index)
{
    float bias = 0.1;
    vec3 FragPosViewSpace = (inverse(view) * vec4(texture(gPosition, TexCoords).rgb, 1.0)).xyz;
    float shadowDepth = bias + (texture(shadowMap[index], FragPosViewSpace.xyz - lights[index].Position).r * farPlane);
    float currentDepth = length(FragPosViewSpace.xyz - lights[index].Position);
    return currentDepth > shadowDepth ? 0.0 : 1.0;
}

vec3 calculateAmbientLight(vec3 lightColor)
{
    return AMBIENT_STRENGTH * lightColor;
}

vec3 calculateDiffuseLight(vec3 lightColor, vec3 lightDirection, vec3 normal)
{
    float angle = max(dot(lightDirection, normal), 0.0);
    return angle * lightColor;
}

void main()
{                 
    // Retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // Then calculate lighting as usual
    vec3 viewDir = normalize(-FragPos);
    vec3 lighting = vec3(0.0); 

    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        vec3 lightPos = (view * vec4(lights[i].Position, 1.0)).xyz;
        float lightFallOff = lights[i].Intensity / length(lightPos - FragPos);
        vec3 lightDir = normalize(lightPos - FragPos);
        //view direction would be -FragPos because this is already in view space
        vec3 halfWay = normalize(lightDir - viewDir);
        
        float spec = pow(max(dot(Normal, halfWay), 0.0), 32.0 * Specular);
        vec3 specular = lights[i].Color * spec;

        vec3 ambient = calculateAmbientLight(lights[i].Color);
        vec3 diffuse = calculateDiffuseLight(lights[i].Color, lightDir, Normal);
        lighting += (ambient + diffuse + specular) * Albedo * isShadow(i) * lightFallOff;
    }

    FragColor = vec4(lighting, 1.0);
}


