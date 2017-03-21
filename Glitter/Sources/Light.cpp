#include "Light.hpp"

using namespace std;

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

void Light::CreateShadowMap(Shader shadowMapShader, Object object)
{
	shadowMap.CreateShadowMap(shadowMapShader, object, position);
}

void Light::LoadDataToShader(GLuint shaderID, string lightPosition, string lightColor, string lightIntensity)
{
	glUniform3fv(glGetUniformLocation(shaderID, lightPosition.c_str()), 1, glm::value_ptr(position));
	glUniform3fv(glGetUniformLocation(shaderID, lightColor.c_str()), 1, glm::value_ptr(color));
	glUniform1f(glGetUniformLocation(shaderID, lightIntensity.c_str()), intensity);
}
