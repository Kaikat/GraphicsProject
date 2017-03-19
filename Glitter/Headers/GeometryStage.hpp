
#pragma once
#include <glad\glad.h>
#include "GLFW\glfw3.h"

#include <memory>
#include "shader.hpp"
#include "model.hpp"

class GeometryStage
{
	private:
		GLuint gBufferID;
		GLuint positionTextureID, colorTextureID, normalTextureID;
		Shader gBufferShader;

		void CreateGBufferTextures();
		void AssignUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	public:
		GeometryStage();
		void Pass(Model model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
		GLuint GetGBufferID() { return gBufferID; };
		GLuint GetPositionTextureID() { return positionTextureID; };
		GLuint GetColorTextureID() { return colorTextureID; };
		GLuint GetNormalTextureID() { return normalTextureID; };
};
