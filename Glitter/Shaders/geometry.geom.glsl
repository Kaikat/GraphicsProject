#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in VS_OUT
{
    vec3 VertexPos; //in view space
    //vec2 TexCoords; //in NDC (0 to 1)
} gs_in[];

void main()
{
    gl_Position = vec4(gs_in[0].VertexPos, 1.0);
    EmitVertex();
    gl_Position = vec4(gs_in[1].VertexPos, 1.0);
    EmitVertex();
    gl_Position = vec4(gs_in[2].VertexPos, 1.0);
    EmitVertex();

    EndPrimitive();
}