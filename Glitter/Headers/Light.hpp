
#pragma once 

#include <glm/glm.hpp>
#include "ShadowMap.hpp"

class Light
{
	private:
		glm::vec3 position;
		glm::vec3 color;
		float intensity;
		
	public:
		ShadowMap shadowMap;

		Light() {};
		Light(glm::vec3 lightPosition, glm::vec3 lightColor, float lightIntensity);
		void Init();
		glm::vec3 Position() { return position; };
		glm::vec3 Color() { return color; };
		float Intensity() { return intensity; };
};