#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 FragPos; //in view space
    vec2 TexCoords; //in NDC (0 to 1)
} gs_in[];

//in vec3 Normal;

out vec3 fragmentColor;

uniform sampler2D p0_point;
uniform sampler2D p1_point;
uniform sampler2D p2_point;

uniform sampler2D p0_normal;
uniform sampler2D p1_normal;
//uniform sampler2D p2_normal;

uniform mat4 model;
uniform mat4 cameraView;
uniform mat4 lightView;
uniform mat4 projection;

const vec3 red = vec3(1.0, 0.0, 0.0);
const vec3 orange = vec3(1.0, 153.0/255.0, 0.0);
const vec3 yellow = vec3(1.0, 1.0, 0.0);
const vec3 green = vec3(0.0, 1.0, 0.0);
const vec3 blue = vec3(0.0, 0.0, 1.0);

void main()
{
    
    vec4 point0 = projection * cameraView * inverse(lightView) * vec4(texture(p0_point, gs_in[0].TexCoords).xyz, 1.0);
    vec4 point1 = projection * cameraView * inverse(lightView) * vec4(texture(p1_point, gs_in[0].TexCoords).xyz, 1.0);
    vec4 point2 = projection * cameraView * inverse(lightView) * vec4(texture(p2_point, gs_in[0].TexCoords).xyz, 1.0);

    vec4 normal0 =  vec4(texture(p0_normal, gs_in[0].TexCoords).xyz, 0.0);

    vec4 point0Norm = texture(p0_point, gs_in[0].TexCoords) + normal0;
    point0Norm = projection * cameraView * inverse(lightView) * point0Norm;


    vec4 normal1 = projection * model * vec4(texture(p1_normal, gs_in[0].TexCoords).xyz, 1.0);
    //normal0 = vec4(normalize(normal0.xyz));
    //normals per point
    fragmentColor = red;

    gl_Position = point0;
    EmitVertex();

    gl_Position = point0Norm; //point0 + ( normal0 * 0.4);//normalize(normal0 - point0);//normal0 + (normal0 * 0.4);
    EmitVertex();
    EndPrimitive();


    //light to first object point
    fragmentColor = yellow;

    gl_Position = projection * cameraView * inverse(lightView) * vec4(0.0, 0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = point0;
    EmitVertex();

    EndPrimitive();

    //first object point to second object point
    fragmentColor = green;

    gl_Position = point0;
    EmitVertex();

    gl_Position = point1;
    EmitVertex();

    //second object point to scene object point

    EndPrimitive();
}

//C:\Users\Nataly\Downloads\apitrace-msvc-latest\x86\bin\apitrace.exe trace --api gl Glitter\Release\Glitter.exe