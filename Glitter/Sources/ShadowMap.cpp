
#include "ShadowMap.hpp"
#include "Constants.hpp"

#define CUBE_DIMENSION 1024
#define CUBE_FACES 6

ShadowMap::ShadowMap()
{
	cubeMap = CubeMap(CUBE_DIMENSION);
}

void ShadowMap::Init()
{
	cubeMap.GenTextures();
}

void ShadowMap::CreateShadowMap(Shader shadowMapShader, Model model, glm::vec3 lightPosition, glm::mat4 modelMatrix)
{
	glEnable(GL_DEPTH_TEST);
	glm::mat4 lightProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, mNear, mFar);

	for (int cubeFace = 0; cubeFace < CUBE_FACES; cubeFace++)
	{
		glm::mat4 lightView = glm::lookAt(lightPosition,
			lightPosition + CubeOrientations[cubeFace],
			CubeUpDirections[cubeFace]);
		glm::mat4 lightSpace = lightProjectionMatrix * lightView;

		cubeMap.CreateFace(cubeFace);

		shadowMapShader.Use();
		glBindFramebuffer(GL_FRAMEBUFFER, cubeMap.GetFrameBufferID());
		glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));
		glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniform1f(glGetUniformLocation(shadowMapShader.Program, "farPlane"), mFar);
		glUniform3fv(glGetUniformLocation(shadowMapShader.Program, "lightPosition"), 1, glm::value_ptr(lightPosition));

		glClear(GL_DEPTH_BUFFER_BIT);
		model.Draw(shadowMapShader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}