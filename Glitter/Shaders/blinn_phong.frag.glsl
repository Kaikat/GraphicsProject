#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform mat4 view;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

const int NR_LIGHTS = 3;

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
void main()
{                 
    // Retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // Then calculate lighting as usual
    vec3 lighting = Albedo * 0.1; // hard-coded ambient component
    vec3 viewDir = normalize(-FragPos);

//TTTTTTTTTTTTTTTTTTTTEMP CHANGE BACK TO NR_LIGHTS
    for(int i = 0; i < 3; ++i)
    {
        // Diffuse
        float distanceFromLight = length(lights[i].Position - FragPos);
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
        
        lighting += 3 * diffuse * isShadow(i) / distanceFromLight;
    }
    FragColor = vec4(lighting, 1.0);
}



/*#version 330 core
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_specular1;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosViewSpace;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

#define NUMBER_OF_LIGHTS 3
uniform vec3 lightPosition[NUMBER_OF_LIGHTS]; 
uniform vec4 lightColor[NUMBER_OF_LIGHTS];
uniform float shiny;

uniform int colorMode;
//uniform vec4  viewPosition;

#define NUMBER_OF_SAMPLES 64
uniform sampler2D depthMap;
uniform float randomPoints[NUMBER_OF_SAMPLES * 3];
uniform float halfRadius;

//One ShadowMap per light
uniform samplerCube shadowMap[NUMBER_OF_LIGHTS];
//uniform samplerCube shadowMap1;
//uniform samplerCube shadowMap2;
uniform float farPlane;

float isShadow(int index)
{
    float bias = 0.1;
    vec3 perspectiveDivCoords = FragPosViewSpace.xyz / FragPosViewSpace.w;
    perspectiveDivCoords = perspectiveDivCoords * 0.5 + 0.5;
    float shadowDepth = bias + (texture(shadowMap[index], FragPosViewSpace.xyz - lightPosition[index]).r * farPlane);
    float currentDepth = length(FragPosViewSpace.xyz - lightPosition[index]);
    return currentDepth > shadowDepth ? 1.0 : 0.0;
}

// Screen Space Ambient Occlusion
float calculateSSAO(vec3 normal)
{
    // calculate the axis for the space the sphere is in
    // we have the normal axis, we need 2 more perpendicular axis
    // make a vector to cross the normal with to get a perpendicular axis
    vec3 r = normal + vec3(1.0, 0.0, 0.0);
    float dotP = dot(normal, normalize(r));

    // it's in the same direction/parallel to N
    if (dotP == -1 || dotP == 1)
    {
        r += vec3(0.0, 1.0, 0.0);
    }

    // calculate the other 2 perpendicular axis
    vec3 tangent = cross(normal, normalize(r));
    vec3 biTangent = cross(tangent, normal);
    mat3 NormalTangentBitangentMatrix = mat3(normal, tangent, biTangent);

    float sumAllPoints = 0.0;
    float sumVisiblePoints = 0.0;
    for(int i = 0; i < NUMBER_OF_SAMPLES * 3; i+=3)
    {
        //Linear Combination
        vec3 samplePoint = vec3(randomPoints[i], randomPoints[i + 1], randomPoints[i + 2]);
        if (samplePoint.x < 0) //the point is not within the the hemisphere with the normal
        {
            samplePoint.x = -samplePoint.x; //so flip it
        }

        vec4 pointInSphere = projection * vec4(FragPos + NormalTangentBitangentMatrix * samplePoint, 1.0);
        vec4 limit = projection * vec4(FragPos + vec3(0, 0, halfRadius), 1.0);
        limit /= limit.w;
        // Now in NDC: -1 to 1
        pointInSphere = pointInSphere/pointInSphere.w;
        // Need the points to go from 0 to 1 for indexing into the texture
        vec2 uvCoordinate = vec2((pointInSphere.x + 1.0) / 2.0, (pointInSphere.y + 1.0) / 2.0);
        // Compare the points depth to the depth in the texture (but divide by w)
        float depth = texture(depthMap, uvCoordinate).x; //x,y,z,w are all the same for depth
        float pointDepth = (pointInSphere.z + 1.0) / 2.0;
        if (pointDepth < depth || depth < (limit.z + 1.0) / 2.0) // the point is visible
        {
            sumVisiblePoints += samplePoint.x;
        }
        sumAllPoints += samplePoint.x;
    }

    // return the influence of lit points
    return sumVisiblePoints / sumAllPoints;
}

void main()
{
    vec3 viewPosition = vec3(0.0f);
    vec3 normalizedNormal = normalize(Normal);

    // Ambient Color
    vec4 texture_color = texture(texture_diffuse1, TexCoords);
    vec4 ambient_color = 0.5 * texture_color;
    
    for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
    {
        vec3 lightPosition_viewSpace = (view * vec4(lightPosition[i], 1.0)).xyz;
        vec3 lightDirection = normalize(lightPosition_viewSpace - FragPos);
        vec3 viewDirection = normalize(viewPosition - FragPos);
        vec3 halfwayDirection = normalize(lightDirection + viewDirection);

        float lightDistance = length(lightPosition_viewSpace - FragPos) / 5.0;
        // blinn_phong calculation
        float specular = pow(max(dot(normalizedNormal, halfwayDirection), 0.0), shiny);
	    vec4 specular_color = lightColor[i] * specular;

        // Diffuse Color
        float diff = max(dot(lightDirection, normalizedNormal), 0.0);
        vec4 diffuse_color = diff * texture_color;

        // Output the diffuse texture color of the object
        
        if (colorMode == 1)
        {
            color += diffuse_color;
            color /= lightDistance;
        }
        if (colorMode == 2)
        {
            color += ambient_color + diffuse_color;
            color /= lightDistance;
        }   
        if (colorMode == 3)
        {
            color += ambient_color + diffuse_color + specular_color;
            color /= lightDistance;
        } 
        if (colorMode == 4)
        {
            color += (ambient_color + (3.0 - isShadow(i)) * (diffuse_color + specular_color));
            color /= lightDistance;
        }
        if (colorMode == 5)
        {
            float ssao = calculateSSAO(normalizedNormal);
            color += ((ambient_color * ssao) + (3.0 - isShadow(i)) * (diffuse_color + specular_color));
            color /= lightDistance;
        }
        if (colorMode == 6)
        {
            float ssao = calculateSSAO(normalizedNormal);
            color += vec4(vec3(ssao), 1.0);
            color /= lightDistance;
        }
    }

    if (colorMode == 0)
    {
        color = ambient_color;
    }
}
*/