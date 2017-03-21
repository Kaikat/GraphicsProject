#version 330 core
out vec4 ldrColor;
in vec2 TexCoords;

uniform sampler2D hdrTexture;

void main()
{             
    //0.2, 3 looks good with 0.4 albedo
    const float gamma = 0.85;
    const float exposure = 2.0;

    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;
    vec3 toneMapped = vec3(1.0) - exp(-hdrColor * exposure);
    //Gamma Correction
    toneMapped = pow(toneMapped, vec3(1.0 / gamma));
    ldrColor = vec4(toneMapped, 1.0);
    //ldrColor = vec4(hdrColor, 1.0);
}