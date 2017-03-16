
#pragma once
#include "Light.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "GeometryStage.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#define NUMBER_OF_SAMPLES 64

class LightingStage
{
	private:
		Shader shadowMapShader;
		Shader brdfShader;
		Shader ssaoShader; //screen space ambient occlusion

		//The quad that will be drawn to for deferred rendering
		GLuint vertexArrayBufferID;

		//The buffer and texture that will hold the result of deferred rendering with a brdf
		GLuint brdfResultFrameBufferID;
		GLuint brdfResultTextureID;

		GLfloat randomPoints[NUMBER_OF_SAMPLES * 3];
		//The texture ID for the ssao 

		//GLuint quadPositionBufferID;
		//GLuint quadUVBufferID;
		//GLuint quadPositionIndexBufferID;

	public:
		LightingStage();
		void Pass(Light *lights, Model model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage);
};