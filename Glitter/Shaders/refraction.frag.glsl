#version 330 core
layout (location = 0) out vec3 p1_object;
layout (location = 1) out vec3 p2_floor;

//gl cullface for front and back

in vec3 FragPos;
in vec2 TexCoords;

//in vec3 Normal;

#define INDEX_OF_REFRACTION_AIR 1.00029
#define INDEX_OF_REFRACTION_WATER 1.33

uniform sampler2D front_depth;
uniform sampler2D back_depth;

uniform sampler2D front_position;
uniform sampler2D back_position;

uniform sampler2D front_normal;
uniform sampler2D back_normal;

void main()
{    
    //normal = normalize(Normal);
    vec3 normal = normalize(texture(front_normal, TexCoords)).xyz;
    vec3 v = -FragPos;
    float angle_i = dot(normal, v);
    vec3 dN = -normal;

    //calculate vector that is perpendicular to dN and in the same "2D plane"
    vec3 R2 = cross(cross(dN, v), dN);

    float lengthDv = texture(back_position, TexCoords).x - texture(front_position, TexCoords).x;
    //float lengthDv = texture(back_depth, TexCoords).x - texture(front_depth, TexCoords).x;

    float angleT = asin(INDEX_OF_REFRACTION_AIR * sin(angle_i) / INDEX_OF_REFRACTION_WATER);
    vec3 T1 = dN * cos(angleT) + R2 * sin(angleT);

    vec3 P1 = FragPos;
    p1_object = P1 + (lengthDv * T1); //p2 in short paper, p1 in long paper

    //TODO: calculate p2_floor
    p2_floor = p1_object;
}