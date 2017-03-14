
#include "LightingStage.hpp"
#include "filesystem.hpp"
#include "Constants.hpp"

#define TOTAL_LIGHTS 3
#define CUBE_FACES 6

LightingStage::LightingStage()
{
	shadowMapShader = Shader(FileSystem::getPath("Shaders/shadow_map.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/shadow_map.frag.glsl").c_str());
	brdfShader = Shader(FileSystem::getPath("Shaders/blinn_phong.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/blinn_phong.frag.glsl").c_str());

	// Create the quad
	glm::vec3 quadPositions[4];
	quadPositions[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	quadPositions[1] = glm::vec3(-1.0f, 1.0f, 0.0f);
	quadPositions[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	quadPositions[3] = glm::vec3(1.0f, -1.0f, 0.0f);

	float quadUVs[8] = { 0.0f, 0.0f,
						 0.0f, 1.0f,
						1.0f, 1.0f,
						 1.0f, 0.0f };

	GLuint quadTriangleIndeces[6] = { 0, 1, 2, 2, 3, 0};
	
	//put positions, uvs, indeces in vertex array object
	glGenVertexArrays(1, &vertexArrayBufferID);
	glBindVertexArray(vertexArrayBufferID);

	glGenBuffers(1, &quadPositionBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadPositionBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositions), quadPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //vertex shader
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &quadUVBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadUVs), quadUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //vertex shader
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &quadPositionIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadPositionIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadTriangleIndeces), quadTriangleIndeces, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void LightingStage::Pass(Light *lights, Model model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, GeometryStage geometryStage)
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

	glViewport(0, 0, mWidth, mHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	brdfShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(brdfShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glBindVertexArray(vertexArrayBufferID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, geometryStage.GetPositionTextureID()); 
	glUniform1i(glGetUniformLocation(brdfShader.Program, "gPosition"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, geometryStage.GetColorTextureID());
	glUniform1i(glGetUniformLocation(brdfShader.Program, "gAlbedoSpec"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, geometryStage.GetNormalTextureID());
	glUniform1i(glGetUniformLocation(brdfShader.Program, "gNormal"), 2);

	glUniform3fv(glGetUniformLocation(brdfShader.Program, "lights[0].Position"), 1, glm::value_ptr(lights[0].Position()));
	glUniform3fv(glGetUniformLocation(brdfShader.Program, "lights[1].Position"), 1, glm::value_ptr(lights[1].Position()));
	glUniform3fv(glGetUniformLocation(brdfShader.Program, "lights[2].Position"), 1, glm::value_ptr(lights[2].Position()));
	glUniform3fv(glGetUniformLocation(brdfShader.Program, "lights[0].Color"), 1, glm::value_ptr(lights[0].Color()));
	glUniform3fv(glGetUniformLocation(brdfShader.Program, "lights[1].Color"), 1, glm::value_ptr(lights[1].Color()));
	glUniform3fv(glGetUniformLocation(brdfShader.Program, "lights[2].Color"), 1, glm::value_ptr(lights[2].Color()));


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	// Shadow Map Stuff

	//model.Draw(brdfShader);

	glDisable(GL_DEPTH_TEST);
}