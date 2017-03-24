#version 330 core
layout (location = 0) out vec3 p1_object;
layout (location = 1) out vec3 p2_floor;

//gl cullface for front and back

in vec3 FragPos;
in vec2 TexCoords;

//in vec3 Normal;

#define INDEX_OF_REFRACTION_AIR 1.00029
#define INDEX_OF_REFRACTION_WATER 1.33

#define PI 3.14159265358979323

uniform sampler2D front_depth; //
uniform sampler2D back_depth; //

uniform sampler2D front_normal; //
uniform sampler2D back_normal;

uniform sampler2D front_position;
uniform sampler2D back_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec4 refraction(vec3 incident, vec3 normal, float ni_nt, float ni_nt_sqr )
{
// vec4 returnVal;
incident = normalize(incident);
normal = normalize(normal);

float IdotN = dot( -incident, normal );
float cosSqr = 1.0 - ni_nt_sqr*(1.0 - IdotN*IdotN);
return ( cosSqr <= 0.0 ? 
vec4( reflect( incident, normal ).xyz, -1 ) : 
vec4( normalize( ni_nt * incident + (ni_nt * IdotN - sqrt( cosSqr )) * normal ).xyz, 1) 
  ); 

  /*
  vec4 returnVal;
float IdotN = dot( -incident, normal );
float cosSqr = 1.0 - ni_nt_sqr*(1.0 - IdotN*IdotN);
return vec4(normalize( ni_nt * incident + (ni_nt * IdotN - sqrt( cosSqr )) * normal ).xyz, 1.0);

*/
//( cosSqr <= 0.0 ? 
//vec4( reflect( incident, normal ).xyz, -1 ) : 
//vec4( normalize( ni_nt * incident + (ni_nt * IdotN - sqrt( cosSqr )) * normal ).xyz, 1) 
 // ); 
 /* float cosI = dot(normal, -incident);
  float sinT2 = ni_nt_sqr * (1.0 - cosI * cosI);
  float cosT = sqrt(1.0 - sinT2);
  return vec4(ni_nt * incident + (ni_nt * cosI - cosT) * normal, 1.0);*/
}

void main()
{    
   // vec3 normal = normalize(Normal);
    vec3 normal = normalize(texture(front_normal, TexCoords).xyz);
    vec3 v = -FragPos;
    float angle_i = dot(normal, v);
    vec3 dN = -normal;

    //calculate vector that is perpendicular to dN and in the same "2D plane"
    vec3 temp = cross(dN, v);
    vec3 R2 = cross(temp, dN);


    

   /* if(temp == vec3(0.0))
    {
         R2 = normalize(vec3(10.0,10.0, 0.0));
    }*/

//    R2 = normalize(vec3(10.0,10.0, 0.0));
//    vec3 R2 = cross(cross(v, dN), dN);

   

    float lengthDv = abs(texture(back_position, TexCoords).z - texture(front_position, TexCoords).z);
    //float lengthDv = texture(back_depth, TexCoords).x - texture(front_depth, TexCoords).x;

    float angleT = asin(INDEX_OF_REFRACTION_AIR * sin(angle_i) / INDEX_OF_REFRACTION_WATER);
    
    //angleT = 60.0 * PI / 180.0;
    vec3 T1 = dN * cos(angleT) + R2 * sin(angleT); //or opp ---------------------------
       //  T1 = R2 * cos(angleT) + dN * sin(angleT);

    //T1 = normalize(vec3(10.0, 10.0, 0.0));

    vec3 P1 = FragPos; /////////////////////////////////////
    //vec3 P1 = texture(front_position, TexCoords).xyz;

    //p1_object = P1 + (lengthDv * T1); //p2 in short paper, p1 in long paper

    //TODO: calculate p2_floor
   // p2_floor = p1_object;

   vec3 t1 = texture(front_position, TexCoords).xyz;
   vec3 t2 = texture(back_position, TexCoords).xyz;
   vec3 t3 = texture(back_normal, TexCoords).xyz;
   vec3 t4 = texture(front_depth, TexCoords).xyz;
   vec3 t5 = texture(back_depth, TexCoords).xyz;


    float refractiveRatio = INDEX_OF_REFRACTION_AIR / INDEX_OF_REFRACTION_WATER;
    vec3 refractionDir = refraction(-FragPos, normal, refractiveRatio, refractiveRatio * refractiveRatio).xyz;

    if (acos(dot(refractionDir, dN)) < 0.0)
    {
        refractionDir = -refractionDir;
    }

    const float STEP = 0.1;
    //vec3 startPoint = FragPos;
    vec3 startPoint = texture(front_position, TexCoords).xyz;
    vec3 direction = normalize(refractionDir);

    float currentDepth = texture(front_depth, TexCoords).x;
    float calculatedBackDepth = texture(back_depth, TexCoords).x;

    /*startPoint = startPoint + (STEP * direction);
        //Convert this new point to depth
        vec4 calcDepth = projection * vec4(startPoint, 1.0);
        calcDepth = calcDepth / calcDepth.w;
        calcDepth = (calcDepth + 1.0) / 2.0;
        currentDepth = calcDepth.z;

        p1_object = vec3(currentDepth);
        p2_floor = texture(front_depth,TexCoords).xyz;*/
    
    for (float i = 0; i < 2.5 && currentDepth < calculatedBackDepth; i+= STEP)
    {
        startPoint = startPoint + (STEP * direction);
        //Convert this new point to depth
        vec4 calcDepth = projection * vec4(startPoint, 1.0);
        calcDepth = calcDepth / calcDepth.w;
        calcDepth = (calcDepth + 1.0) / 2.0;
        currentDepth = calcDepth.z;

        calculatedBackDepth = texture(back_depth, calcDepth.xy).x;
    }
    //if (currentDepth > calculatedBackDepth)
    //{
        //p1_object = vec3(0.0); p2_floor = p1_object;
    //}
//else
//{
    //p1_object = FragPos + refractionDir;
    p1_object = startPoint;
    p2_floor = p1_object;
//}

   // p1_object =t4;//texture(front_depth, TexCoords).xyz;
   // p2_floor = t5;//texture(back_depth, TexCoords).xyz;

    //p1_object = texture(front_position, TexCoords).xyz;
    //p1_object = FragPos.xyz;
    //p2_floor = texture(back_position, TexCoords).xyz;

    //p1_object = normal;
    //p2_floor = v;
}