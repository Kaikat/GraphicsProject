
#pragma once
#include "Light.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "GeometryStage.hpp"
#include "Constants.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "Texture2D.hpp"
#include "SkyBox.hpp"

#define NUMBER_OF_SAMPLES 64
#define VEC_SAMPLE_POINT 3

class LightingStage
{
	private:
		Shader shadowMapShader;
		Shader brdfShader;
		Shader ssaoShader; //screen space ambient occlusion
		Shader lightingResultsShader;
		Shader skyboxShader;

		//The quad that will be drawn to for deferred rendering
		GLuint vertexArrayBufferID;

		//The buffer and texture that will hold the result of deferred rendering with a brdf
		GLuint brdfResultFrameBufferID;
		Texture2D brdfTexture;

		//The buffer and texture that will hold the result of screen space ambient occlusion (ssao)
		GLuint ssaoResultFrameBufferID;
		Texture2D ssaoTexture;

		GLfloat randomPoints[NUMBER_OF_SAMPLES * VEC_SAMPLE_POINT];

		//Skybox
		GLuint skyboxVAO;
		SkyBox skybox;

	public:
		LightingStage();
		void Pass(Light *lights, Model model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage);

	private:
		void LoadShaders();
		void GenerateRandomSamplePoints();
		void CreateQuadVertexArrayObject();
		void SetupBRDFFramebuffer();
		void SetupSSAOFramebuffer();
		void SetupSkyBox();
};