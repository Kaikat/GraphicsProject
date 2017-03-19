
#pragma once
#include "CubeMap.hpp"
#include "shader.hpp"
#include "model.hpp"
#include <glm/glm.hpp>

class ShadowMap
{
	private:
	public:
		CubeMap cubeMap;

		ShadowMap();
		void Init();
		void CreateShadowMap(Shader shadowMapShader, Model model, glm::vec3 lightPosition, glm::mat4 modelMatrix);
};