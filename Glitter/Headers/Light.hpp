
#pragma once 

#include <glm/glm.hpp>
#include "ShadowMap.hpp"

class Light
{
	private:
		glm::vec3 position;
		glm::vec3 color;
		float intensity;
		ShadowMap shadowMap;

	public:
		Light() {};
		Light(glm::vec3 lightPosition, glm::vec3 lightColor, float lightIntensity);
		glm::vec3 Position() { return position; };
		glm::vec3 Color() { return color; };
		float Intensity() { return intensity; };
		GLuint GetShadowMapTextureID() { return shadowMap.cubeMap.GetTextureID(); };
		GLuint GetShadowMapFrameBufferID() { return shadowMap.cubeMap.GetFrameBufferID(); };
		void Init();

		void CreateShadowMap(Shader shadowMapShader, Model model, glm::mat4 modelMatrix);
};