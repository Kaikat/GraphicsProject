
#include "LightingStage.hpp"
#include "filesystem.hpp"
#include "Constants.hpp"

#define TOTAL_LIGHTS 3
#define CUBE_FACES 6

LightingStage::LightingStage()
{
	shadowMapShader = Shader(FileSystem::getPath("Shaders/shadow_map.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/shadow_map.frag.glsl").c_str());
}

void LightingStage::Pass(Light *lights, Model model, glm::mat4 modelMatrix)
{
	glEnable(GL_DEPTH_TEST);
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, mNear, mFar);

	for (int i = 0; i < TOTAL_LIGHTS; i++)
	{
		for (int cubeFace = 0; cubeFace < CUBE_FACES; cubeFace++)
		{
			glm::mat4 lightView = glm::lookAt(lights[i].Position(),
				lights[i].Position() + lights[i].shadowMap.cubeMap.GetCubeOrientation(cubeFace),
				lights[i].shadowMap.cubeMap.GetCubeUpDirection(cubeFace));
			glm::mat4 lightSpace = lightProjection * lightView;

			lights[i].shadowMap.cubeMap.CreateFace(cubeFace);
			GLuint buffID = lights[i].shadowMap.cubeMap.GetFrameBufferID();
			glBindFramebuffer(GL_FRAMEBUFFER, buffID);

			shadowMapShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpace));
			glUniformMatrix4fv(glGetUniformLocation(shadowMapShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniform1f(glGetUniformLocation(shadowMapShader.Program, "farPlane"), mFar);
			glUniform3fv(glGetUniformLocation(shadowMapShader.Program, "lightPosition"), 1, glm::value_ptr(lights[i].Position()));

			glClear(GL_DEPTH_BUFFER_BIT);
			model.Draw(shadowMapShader);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	glDisable(GL_DEPTH_TEST);
}