#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform mat4 projection;

#define NUMBER_OF_SAMPLES 64
#define RADIUS 0.5
uniform float randomPoints[NUMBER_OF_SAMPLES * 3];


// Screen Space Ambient Occlusion
float calculateSSAO(vec3 normal)
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;

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
    float occlu2;
    for(int i = 0; i < NUMBER_OF_SAMPLES * 3; i+=3)
    {
        //Linear Combination
        vec3 samplePoint = RADIUS * vec3(randomPoints[i], randomPoints[i + 1], randomPoints[i + 2]);
        if (samplePoint.x < 0) //the point is not within the the hemisphere with the normal
        {
            samplePoint.x = -samplePoint.x; //so flip it
        }

        vec4 pointInSphere = projection * vec4(FragPos + NormalTangentBitangentMatrix * samplePoint, 1.0);
        float thisPointDepth = vec4(FragPos + NormalTangentBitangentMatrix * samplePoint, 1.0).z;

        // Now in NDC: -1 to 1
        pointInSphere = pointInSphere/pointInSphere.w;
        // Need the points to go from 0 to 1 for indexing into the texture
        vec2 uvCoordinate = vec2((pointInSphere.x + 1.0) / 2.0, (pointInSphere.y + 1.0) / 2.0);

        // Compare the points depth 
        float depthAtTexturePosition = texture(gPosition, uvCoordinate).z;
        float radius = 0.25;

        occlu2 += (((depthAtTexturePosition < thisPointDepth) || 
            (depthAtTexturePosition - thisPointDepth >= radius)) ? 1.0 : 0.0);
    }

    //return the influence of lit points
    return occlu2 / NUMBER_OF_SAMPLES;
}

void main()
{
    float ssao = calculateSSAO(texture(gNormal, TexCoords).rgb);
    FragColor = vec4(vec3(ssao), 1.0);
}