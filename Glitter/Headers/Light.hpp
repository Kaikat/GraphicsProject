
#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "ShadowMap.hpp"
#include "Object.hpp"

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
		GLuint GetShadowMapTextureID() { return shadowMap.GetTextureID(); };
		GLuint GetShadowMapFrameBufferID() { return shadowMap.GetFrameBufferID(); };
		void Init();

		void CreateShadowMap(Shader shadowMapShader, Object object);
		void LoadDataToShader(GLuint shaderID, string lightPosition, string lightColor, string lightIntensity);
};