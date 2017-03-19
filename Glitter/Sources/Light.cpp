#include "Light.hpp"

Light::Light(glm::vec3 lightPosition, glm::vec3 lightColor, float lightIntensity)
{
	position = lightPosition;
	color = lightColor;
	intensity = lightIntensity;
}

void Light::Init()
{
	shadowMap.Init();
}

void Light::CreateShadowMap(Shader shadowMapShader, Model model, glm::mat4 modelMatrix)
{
	shadowMap.CreateShadowMap(shadowMapShader, model, position, modelMatrix);
}