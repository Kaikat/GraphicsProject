
#pragma once
#include "CubeMap.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "Object.hpp"
#include <glm/glm.hpp>

class ShadowMap
{
	private:
		CubeMap cubeMap;

	public:

		ShadowMap();
		void Init();
		void CreateShadowMap(Shader shadowMapShader, Object object, glm::vec3 lightPosition);
		GLuint GetTextureID() { return cubeMap.GetTextureID(); };
		GLuint GetFrameBufferID() { return cubeMap.GetFrameBufferID(); };
};