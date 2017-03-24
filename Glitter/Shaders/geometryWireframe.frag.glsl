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
    //color = vec4(97.0/255.0, 255.0/255.0, 160.0/255.0, 1.0);//texture(texture_diffuse1, TexCoords);
    color = vec4(1.0, 1.0, 1.0, 0.1);//texture(texture_diffuse1, TexCoords);
}