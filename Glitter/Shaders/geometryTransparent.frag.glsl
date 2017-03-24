#version 330 core
out vec4 color;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_normal1;
//uniform sampler2D texture_specular1;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{
	// Output the diffuse texture color of the object
    color = vec4(247.0/255.0, 194.0/255.0, 175.0/255.0, 0.2);//texture(texture_diffuse1, TexCoords);
}