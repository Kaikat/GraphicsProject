
#pragma once
#include "Light.hpp"
#include "shader.hpp"
#include "model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

class LightingStage
{
	private:
		Shader shadowMapShader;

	public:
		LightingStage();
		void Pass(Light *lights, Model model, glm::mat4 modelMatrix);
};