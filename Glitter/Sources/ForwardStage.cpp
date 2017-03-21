
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

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	frontPositionTexture.PrintID("Yellow: FrontPositionTexture");
	frontFaceNormalTexture.PrintID("Red: FrontFaceNormalTexture");

	//Back of object
	/*glGenFramebuffers(1, &transparentObjectBackFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, transparentObjectBackFrameBufferID);
	backDepthTexture.CreateTexture(mWidth, mHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	backFaceNormalTexture.CreateTexture(mWidth, mHeight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	backPositionRefractedTexture.CreateTexture(mWidth, mHeight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, backDepthTexture.GetTextureID(), 0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, backFaceNormalTexture.GetTextureID(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT2);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, backPositionRefractedTexture.GetTextureID(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT3);*/

	transparentObjectShader = Shader(FileSystem::getPath("Shaders/transparentObject.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/transparentObject.frag.glsl").c_str());
}

void ForwardStage::Pass(Light *lights, vector<Object> objects, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage)
{
	glBindFramebuffer(GL_FRAMEBUFFER, sceneDepthFrameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Load the scene depth buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, geometryStage.GetGBufferID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, sceneDepthFrameBufferID);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, transparentObjectFrontFrameBufferID);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	transparentObjectShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	objects[0].Draw(transparentObjectShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}
