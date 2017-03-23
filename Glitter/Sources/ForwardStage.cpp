
#include "ForwardStage.hpp"
#include "filesystem.hpp"
#include "glitter.hpp"

ForwardStage::ForwardStage()
{
	//Scene Depth
	glGenFramebuffers(1, &sceneDepthFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneDepthFrameBufferID);
	sceneDepthTexture.CreateTexture(mWidth, mHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTexture.GetTextureID(), 0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);
	sceneDepthTexture.PrintID("sceneDepthTexture");

	//Front of object
	glGenFramebuffers(1, &transparentObjectFrontFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, transparentObjectFrontFrameBufferID);

	frontDepthTexture.CreateTexture(mWidth, mHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frontDepthTexture.GetTextureID(), 0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);

	frontPositionTexture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frontPositionTexture.GetTextureID(), 0);

	frontFaceNormalTexture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, frontFaceNormalTexture.GetTextureID(), 0);

	GLuint frontAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, frontAttachments);

	frontPositionTexture.PrintID("Yellow: FrontPositionTexture");
	frontFaceNormalTexture.PrintID("Red: FrontFaceNormalTexture");

	//Back of object
	glGenFramebuffers(1, &transparentObjectBackFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, transparentObjectBackFrameBufferID);

	backDepthTexture.CreateTexture(mWidth, mHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, backDepthTexture.GetTextureID(), 0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);

	backPositionRefractedTexture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, backPositionRefractedTexture.GetTextureID(), 0);

	backFaceNormalTexture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, backFaceNormalTexture.GetTextureID(), 0);

	GLuint backAttachments[2] = { GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(2, backAttachments);

	backPositionRefractedTexture.PrintID("Yellow: backPositionTexture");
	backFaceNormalTexture.PrintID("Red: backFaceNormalTexture");

	//Refraction calculations
	glGenFramebuffers(1, &refractionFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBufferID);

	refractionP1Texture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionP1Texture.GetTextureID(), 0);

	refractionP2Texture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, refractionP2Texture.GetTextureID(), 0);

	GLuint refractionAttachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, refractionAttachments);

	refractionP1Texture.PrintID("RefractionP1Texture");
	refractionP2Texture.PrintID("RefractionP2Texture");

	transparentObjectShader = Shader(FileSystem::getPath("Shaders/transparentObject.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/transparentObject.frag.glsl").c_str());
	refractionShader = Shader(FileSystem::getPath("Shaders/refraction.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/refraction.frag.glsl").c_str());
}

void ForwardStage::Pass(Light *lights, vector<Object> objects, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage)
{
	glm::mat4 view;
	view = glm::lookAt(lights[0].Position(), objects[0].Position, glm::vec3(0.0, 1.0, 0.0));

	glBindFramebuffer(GL_FRAMEBUFFER, sceneDepthFrameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Load the scene depth buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, geometryStage.GetGBufferID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sceneDepthFrameBufferID);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Front facing
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, transparentObjectFrontFrameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	transparentObjectShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	objects[0].Draw(transparentObjectShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Back facing
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glBindFramebuffer(GL_FRAMEBUFFER, transparentObjectBackFrameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	transparentObjectShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	objects[0].Draw(transparentObjectShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Refraction
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	refractionShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(refractionShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(refractionShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(refractionShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frontDepthTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "front_depth"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backDepthTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "back_depth"), 1);




	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, frontPositionTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "front_postion"), 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, backPositionRefractedTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "back_position"), 5);





	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, frontFaceNormalTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "front_normal"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, backFaceNormalTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "back_normal"), 3);
	objects[0].Draw(refractionShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_DEPTH_TEST);
}
