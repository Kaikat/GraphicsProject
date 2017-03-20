
#include "LightingStage.hpp"
#include "filesystem.hpp"

const string SKYBOX_PATH = "Resources/MountainSkybox/";
const string SKYBOX_FILE_EXTENSION = ".jpg";

LightingStage::LightingStage(BRDF_TYPE brdf)
{
	BRDF = brdf;

	LoadShaders();
	GenerateRandomSamplePoints();
	CreateQuadVertexArrayObject();
	SetupBRDFFramebuffer();
	SetupSSAOFramebuffer();
	SetupSkyBox();
}

void LightingStage::Pass(Light *lights, Model model, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage)
{
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, mNear, mFar);

	//Create a shadow map for every light
	for (int i = 0; i < TOTAL_LIGHTS; i++)
	{
		lights[i].CreateShadowMap(shadowMapShader, model, modelMatrix);
	}

	//Set viewport back when done because the cube map changes it
	glViewport(0, 0, mWidth, mHeight);

	//Use the shadow map results with the brdf to get lighting with shadows
	glBindFramebuffer(GL_FRAMEBUFFER, brdfResultFrameBufferID); //put the results in this framebuffer rather than on the screen
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

	LoadLightUniforms(lights, brdfShader.Program);


	// Shadow Map Stuff
	glUniform1f(glGetUniformLocation(brdfShader.Program, "farPlane"), mFar);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lights[0].GetShadowMapTextureID());
	glUniform1i(glGetUniformLocation(brdfShader.Program, "shadowMap[0]"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lights[1].GetShadowMapTextureID());
	glUniform1i(glGetUniformLocation(brdfShader.Program, "shadowMap[1]"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lights[2].GetShadowMapTextureID());
	glUniform1i(glGetUniformLocation(brdfShader.Program, "shadowMap[2]"), 5);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Use the results of the brdf with ssao
	ssaoShader.Use();
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoResultFrameBufferID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(ssaoShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniform1fv(glGetUniformLocation(ssaoShader.Program, "randomPoints"), NUMBER_OF_SAMPLES * VEC_SAMPLE_POINT, randomPoints);

	glUniform1i(glGetUniformLocation(ssaoShader.Program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(ssaoShader.Program, "gNormal"), 2);
	//glUniform1i(glGetUniformLocation(ssaoShader.Program, "gAlbedoSpec"), 6);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Combine the results of the BRDF and the SSAO Shaders
	lightingResultsShader.Use();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brdfTexture.GetTextureID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ssaoTexture.GetTextureID());

	glUniform1i(glGetUniformLocation(lightingResultsShader.Program, "brdfResult"), 0);
	glUniform1i(glGetUniformLocation(lightingResultsShader.Program, "ssaoResult"), 1);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	//Skybox
	// bind the source framebuffer and select a color attachment to copy from
	glBindFramebuffer(GL_READ_FRAMEBUFFER, geometryStage.GetGBufferID());

	// bind the destination framebuffer and select the color attachments to copy to
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// copy
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	skyboxShader.Use();
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void LightingStage::LoadShaders()
{
	shadowMapShader = Shader(FileSystem::getPath("Shaders/shadow_map.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/shadow_map.frag.glsl").c_str());

	string brdfType = BRDF == BRDF_TYPE::Blinn_Phong ? "Shaders/blinn_phong" : "Shaders/cook_torrance";
	brdfShader = Shader(FileSystem::getPath(brdfType + ".vert.glsl").c_str(),
		FileSystem::getPath(brdfType + ".frag.glsl").c_str());

	ssaoShader = Shader(FileSystem::getPath("Shaders/ssao.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/ssao.frag.glsl").c_str());
	lightingResultsShader = Shader(FileSystem::getPath("Shaders/combineLightingResults.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/combineLightingResults.frag.glsl").c_str());
	skyboxShader = Shader(FileSystem::getPath("Shaders/skybox.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/skybox.frag.glsl").c_str());
}


void LightingStage::LoadLightUniforms(Light *lights, GLuint shaderID)
{
	for (int i = 0; i < TOTAL_LIGHTS; i++)
	{
		string str_i = std::to_string(i);
		lights[i].LoadDataToShader(brdfShader.Program, "lights[" + str_i + "].Position", "lights[" + str_i + "].Color", "lights[" + str_i + "].Intensity");
	}
}

void LightingStage::GenerateRandomSamplePoints()
{
	srand(14);
	for (int i = 0; i < NUMBER_OF_SAMPLES * VEC_SAMPLE_POINT; i++)
	{
		//diameter 2, radius 1 so that it is within a unit sphere
		randomPoints[i] = 2.0f * rand() / (float)RAND_MAX - 1.0f;
	}
}

void LightingStage::CreateQuadVertexArrayObject()
{
	GLuint quadPositionBufferID, quadUVBufferID, quadPositionIndexBufferID;

	//put positions, uvs, indeces in vertex array object
	glGenVertexArrays(1, &vertexArrayBufferID);
	glBindVertexArray(vertexArrayBufferID);

	glGenBuffers(1, &quadPositionBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadPositionBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadPositions), QuadPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //vertex shader
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &quadUVBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadUVs), QuadUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //vertex shader
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &quadPositionIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadPositionIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadTriangleIndeces), QuadTriangleIndeces, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void LightingStage::SetupBRDFFramebuffer()
{
	//put the brdf's results in this framebuffer's color attachment rather than on the screen
	glGenFramebuffers(1, &brdfResultFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, brdfResultFrameBufferID); 
	brdfTexture.CreateTexture(mWidth, mHeight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfTexture.GetTextureID(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void LightingStage::SetupSSAOFramebuffer()
{
	//put ssao results in this framebuffer's color attachment rather than on the screen
	glGenFramebuffers(1, &ssaoResultFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoResultFrameBufferID);
	ssaoTexture.CreateTexture(mWidth, mHeight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture.GetTextureID(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void LightingStage::SetupSkyBox()
{
	skybox.Init(SKYBOX_PATH, SKYBOX_FILE_EXTENSION);

	GLuint cubePositionBufferID, quadUVBufferID, quadPositionIndexBufferID;

	//put positions, uvs, indeces in vertex array object
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);

	glGenBuffers(1, &cubePositionBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, cubePositionBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //vertex shader
	glEnableVertexAttribArray(0);
}