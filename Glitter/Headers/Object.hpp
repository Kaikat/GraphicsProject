#pragma once

#include "model.hpp"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

class Object
{
	private:
		Model model;
		glm::mat4 modelMatrix;

	public:
		Object() {};
		Object(Model objectModel, float scale, glm::vec3 translation);
		glm::mat4 GetModelMatrix() { return modelMatrix; };
		void Draw(Shader shader);
};