#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform mat4 view;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

#define AMBIENT_STRENGTH 0.1
const int NR_LIGHTS = 3;
struct Light {
    vec3 Position;
    vec3 Color;
    float Intensity;
};
uniform Light lights[NR_LIGHTS];
uniform samplerCube shadowMap[NR_LIGHTS];
uniform float farPlane;

//wi incoming direction of light, wo outgoing direction of light

//Kd + Ks <= 1, otherwise it's emitting energy
float Ka = 1.0;
float Ks = 0.8; //amount of light that is specularly reflected (mirror-like material)
float Kd = 0.2; //amount of incoming radiance that gets diffused (diffuse-like material)
float IOR = 1.3;
float roughness = 0.2;

float opacity = 1.0;
float specularColor = 1.0;
vec3 diffuseColor = vec3(0.6, 0.6, 0.6);

#define GAUSSIAN_CONSTANT 100.0
#define PI 3.141592653589793

float isShadow(int index)
{
    float bias = 0.1;
    vec3 FragPosViewSpace = (inverse(view) * vec4(texture(gPosition, TexCoords).rgb, 1.0)).xyz;
    float shadowDepth = bias + (texture(shadowMap[index], FragPosViewSpace.xyz - lights[index].Position).r * farPlane);
    float currentDepth = length(FragPosViewSpace.xyz - lights[index].Position);
    return currentDepth > shadowDepth ? 0.0 : 1.0;
}

vec3 calculateAmbientLight(vec3 lightColor)
{
    return AMBIENT_STRENGTH * lightColor;
}

vec3 calculateFLambert(vec3 color)
{
    return color / PI;
}

vec3 calculateDiffuseLight(vec3 lightColor, vec3 lightDirection, vec3 normal)
{
    float angle = max(dot(lightDirection, normal), 0.0);
    return angle * lightColor;
}

float DistributionFunction(float angleBetweenNormal_Half, float alpha)
{
    float alpha2 = alpha * alpha;
    float angleBetweenNH2 = angleBetweenNormal_Half * angleBetweenNormal_Half;
    //float denominator = angleBetweenNH2 * alpha2 + (1.0 - angleBetweenNH2);
    float denominator = angleBetweenNH2 * (alpha2 - 1.0) + 1.0;
    return (angleBetweenNH2 > 0 ? 1.0 : 0.0) * alpha2 / (PI * denominator * denominator);
}

vec3 CalculateFresnel(float angleBetweenViewDir_Half, vec3 color)
{
    //float F0 = abs((1.0 - IOR) / (1.0 + IOR));
    vec3 F0 = vec3(.04, .04, .04);
    //F0 = F0 * F0;
    //F0 = lerp(F0, materialColor.rgb, metallic) ????
    F0 = F0 + (1.0 - F0) * pow(1.0 - angleBetweenViewDir_Half, 5.0);
    return F0;
}
//F = fresnel function
//D = distribution function: describes the statistical orientation of the micro facets at some point
//G = geometry function: describes the attenuation of light due to the microfacets shadowing each other
//    it is the probability that at a given point the microfacets are occluded by each other or that
//    light bounces on multiple microfacets, loosing energy in the process, before reaching the
//    observer's eye
float ShlickBeckmannGGX(vec3 normal, vec3 direction, float roughness)
{
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    float normalDotDirection = max(dot(normal, direction), 0.0);
    return normalDotDirection / (normalDotDirection * (1.0 - k) + k);
}

float ShlickBeckmannGGXSmith(vec3 normal, vec3 view, vec3 light, float roughness)
{
    return ShlickBeckmannGGX(normal, view, roughness) * ShlickBeckmannGGX(normal, light, roughness);
}

void main()
{
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 cook = vec3(0.0);
    for (int i = 0; i < NR_LIGHTS; i++)
    {
        vec3 viewDir = normalize(-FragPos);
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 halfVector = normalize(viewDir + lightDir);

        float incoming = max(dot(Normal, lightDir), 0.0);
        float outgoing = max(dot(Normal, viewDir), 0.0);

        vec3 Ks_fresnel = CalculateFresnel(outgoing, Albedo);
        vec3 Kd_fresnel = vec3(1.0) - Ks_fresnel;
        vec3 Flambert = Albedo / PI;

        //Cook Torrance 
        float torranceDenominator = max(4.0 * incoming * outgoing, 0.1);

        float angleBetweenNormal_Half = max(dot(Normal, halfVector), 0.0);
        float alpha = 1.0 - Specular;
        float D = DistributionFunction(angleBetweenNormal_Half, alpha);
        float G = ShlickBeckmannGGXSmith(Normal, viewDir, lightDir, roughness);

        float torranceNumerator = D * G;
        vec3 FragPosWorld = (inverse(view) * vec4(FragPos, 1.0)).xyz;
        float lightFallOff = lights[i].Intensity / pow(length(lights[i].Position - FragPosWorld), 2.0);
        cook += (Kd_fresnel * Flambert + Ks_fresnel * (D * G / torranceDenominator)) * isShadow(i) * lightFallOff * lights[i].Color;
    }

    vec4 ambient = vec4(Albedo * 0.1, 1.0);
    FragColor = vec4(cook, 1.0) + ambient;
}
/*
void main()
{
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    //float Fresnel = CalculateFresnel();     
    vec3 cook = vec3(0.0);
    vec3 Fresnel;
    float cookF = 0.0;
    for (int i = 0; i < NR_LIGHTS; i++)
    {
        vec3 viewDir = normalize(-FragPos);
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 fragDir = normalize(-viewDir); //vn
        vec3 halfVector = normalize(viewDir + lightDir);

        float angleBetweenNormal_ViewDir = max(dot(Normal, fragDir), 0.0);
        float angleBetweenNormal_Half = max(dot(Normal, halfVector), 0.0);
        float angleBetweenNormal_LightDir = max(dot(Normal, lightDir), 0.0);
        float angleBetweenViewDir_Half = max(dot(fragDir, halfVector), 0.0);

        Fresnel = CalculateFresnel(angleBetweenViewDir_Half, Albedo); //Ks
        //Kd = 1 - Fresnel   

        float G = min(1.0, 2.0 * angleBetweenNormal_Half * angleBetweenNormal_ViewDir / angleBetweenViewDir_Half);
        G = min(G, 2.0 * angleBetweenNormal_Half * angleBetweenNormal_LightDir / angleBetweenViewDir_Half);

        //What's m? The direction vector we want to check?
        //float alpha = acos(angleBetweenNormal_Half);
        float alpha = 1.0 - Specular;
        float D = DistributionFunction(angleBetweenNormal_Half, alpha);
        //float D = GAUSSIAN_CONSTANT * exp(-(alpha * alpha) / (m * m));

        //cook torrance is specular
        cook += (Fresnel * D * G) / (PI * angleBetweenNormal_ViewDir);
        cookF += (D * G) / (PI * angleBetweenNormal_ViewDir);


        //cook += Kd * calculateFLambert(Albedo) + Ks * (D * Fresnel * G) / (4.0);
        
        //vec3 ambient = calculateAmbientLight(lights[i].Color);
        //vec3 diffuse = calculateDiffuseLight(lights[i].Color, lightDir, Normal);
        //lighting += (ambient + diffuse + specular) * Albedo * isShadow(i) * lightFallOff;
     }

   // cook = cook / PI;
    float Oi = 1.0;
    //Oi = opacity; ????????????????????????????
    //Ci = (Kd * diffuseColor * diffuse(Normal) + Ks * specularColor * cook) * Oi;
   // Ci = (Kd * calculateFLambert(Albedo)) + Ks * (Specular * cook) * Oi;
    vec4 KdFlambert = Kd * vec4(calculateFLambert(Albedo), 1.0);
    vec3 KsCook = Ks * (Specular * cook) * Oi;
   // FragColor = (Kd * calculateFLambert(Albedo)) + Ks * (Specular * cook) * Oi;
   
   //FragColor = vec4(KdFlambert.x + KsCook, KdFlambert.y + KsCook, KdFlambert.z + KsCook, 1.0);
   
   //FragColor = vec4(Fresnel.x, Fresnel.y, Fresnel.z, 1.0);

   //FragColor = (1.0 - Fresnel) * fLambert + cook;
   vec3 fLambert = (Albedo / PI);
   //vec3 F0plastic = vec3(0.0);
   //vec3 newKd = 
   vec3 intermColor = (Albedo / PI) * Ks * Specular * cookF;
  // FragColor = vec4(intermColor.x, intermColor.y, intermColor.z, 1.0) ;
  // FragColor = vec4(fLambert.x, fLambert.y, fLambert.z, 1.0);
    vec3 ll = (1.0 - Fresnel) * fLambert + cook;
    FragColor = vec4(ll, 1.0);
  //FragColor = 1.0 - vec4(Fresnel.x, Fresnel.y, Fresnel.z, 1.0);
}
*/

/*
float CalculateFresnel()
{
    return 0.0;
}

float CalculateIlluminance()
{
    float NdotV = dot(Normal, viewingVector);
    vec3 lightDirection;
    vec3 viewPosition;
    //have eye to the surface vector, I, need surface to eye
    vector Vn = normalize(-I);

    vector Ln = normalize(L); 
    vector H = normalize(Vn + Ln);
    float NdotH = dot(Nn, H);
    float NdotL = dot(Nn, Ln);
    float VdotH = dot(Vn, H);

    //Geometric Attenuation, G
    float G = min(1.0, (2 * NdotH * NdotV / VdotH), (2 * NdotH * NdotL / VdotH));

    //Microfacet Slope Distribution, D
    float alpha = acos(NdotH);
    float D = gaussConstant * exp(-(alpha * alpha) / (m * m));s

    //END OF LOOP
    cook += C1 * (F * D * G) / (PI * NdotV);

    //OUT OF END OF LOOP
    cook = cook / PI;
    Oi = opacity;
    Ci = (Kd * diffuseColor * diffuse(Nn) + Ks * specularColor * cook) * Oi;
}
*/


/*
void main()
{                 
    // Retrieve data from G-buffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // Then calculate lighting as usual
    vec3 viewDir = normalize(-FragPos);
    vec3 lighting = vec3(0.0); 

    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        vec3 lightPos = (view * vec4(lights[i].Position, 1.0)).xyz;
        float lightFallOff = lights[i].Intensity / length(lightPos - FragPos);
        vec3 lightDir = normalize(lightPos - FragPos);
        //view direction would be -FragPos because this is already in view space
        vec3 halfWay = normalize(lightDir - viewDir);
        
        float spec = pow(max(dot(Normal, halfWay), 0.0), 32.0 * Specular);
        vec3 specular = lights[i].Color * spec;

        vec3 ambient = calculateAmbientLight(lights[i].Color);
        vec3 diffuse = calculateDiffuseLight(lights[i].Color, lightDir, Normal);
        lighting += (ambient + diffuse + specular) * Albedo * isShadow(i) * lightFallOff;
    }

    FragColor = vec4(lighting, 1.0);
}


*/
