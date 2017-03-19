
#pragma once
#include "CubeMap.hpp"
#include "shader.hpp"
#include "model.hpp"
#include <glm/glm.hpp>

class ShadowMap
{
	private:
		CubeMap cubeMap;

	public:

		ShadowMap();
		void Init();
		void CreateShadowMap(Shader shadowMapShader, Model model, glm::vec3 lightPosition, glm::mat4 modelMatrix);
		GLuint GetTextureID() { return cubeMap.GetTextureID(); };
		GLuint GetFrameBufferID() { return cubeMap.GetFrameBufferID(); };
};