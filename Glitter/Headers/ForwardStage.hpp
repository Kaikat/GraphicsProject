#pragma once

#include "Light.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "GeometryStage.hpp"
#include "Constants.hpp"
#include "Object.hpp"
#include "Texture2D.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

class ForwardStage
{
	private:
		Shader transparentObjectShader, refractionShader;
		GLuint sceneDepthFrameBufferID;
		GLuint transparentObjectFrontFrameBufferID, transparentObjectBackFrameBufferID;
		GLuint refractionFrameBufferID;

		Texture2D sceneDepthTexture;
		Texture2D frontDepthTexture, backDepthTexture;
		Texture2D frontPositionTexture, backPositionRefractedTexture;
		Texture2D frontFaceNormalTexture, backFaceNormalTexture;
		Texture2D refractionP1Texture, refractionP2Texture;

		//vector<float> NDC_pointPattern;
		GLuint debuggerVAO;
		Shader debuggingShader;
		Texture2D debuggingTexture;
		GLuint debuggingFrameBufferID;
		GLuint NDC_pointPatternBufferID;

		Texture2D vDebugTexture, frontNormalDebugTexture;

	public:
		ForwardStage();
		void Pass(Light *lights, vector<Object> objects, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage);
};