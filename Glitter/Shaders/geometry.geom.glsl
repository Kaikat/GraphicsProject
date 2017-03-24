#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in VS_OUT
{
    vec4 VertexPos; //in view space
    //vec2 TexCoords; //in NDC (0 to 1)
} gs_in[];

void main()
{
    gl_Position = gs_in[0].VertexPos;
    EmitVertex();
    gl_Position = gs_in[1].VertexPos;
    EmitVertex();
    gl_Position = gs_in[2].VertexPos;
    EmitVertex();

    EndPrimitive();
}