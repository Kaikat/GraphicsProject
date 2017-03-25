
#include "ForwardStage.hpp"
#include "filesystem.hpp"
#include "glitter.hpp"
#include <glm/gtc/type_ptr.hpp>


#define STEP 1.0/50.0

ForwardStage::ForwardStage()
{
	//Scene Depth
	glGenFramebuffers(1, &sceneDepthFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneDepthFrameBufferID);


	
	/*
	glGenRenderbuffers(1, &depthBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
	*/
	sceneDepthTexture.CreateTexture(mWidth, mHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sceneDepthTexture.GetTextureID(), 0);
	glDrawBuffer(GL_DEPTH_ATTACHMENT);

	scenePositionTexture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scenePositionTexture.GetTextureID(), 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	sceneDepthTexture.PrintID("sceneDepthTexture");
	scenePositionTexture.PrintID("scenePositionTexture");

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

	//Refraction Debugger
	glGenFramebuffers(1, &debuggingFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, debuggingFrameBufferID);
	glGenVertexArrays(1, &debuggerVAO);
	glBindVertexArray(debuggerVAO);

	//debuggingTexture.CreateTexture(mWidth, mHeight, GL_RGB16F, GL_RGB, GL_FLOAT, GL_TEXTURE_MIN_FILTER, GL_NEAREST, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, debuggingTexture.GetTextureID(), 0);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//debuggingTexture.PrintID("DebuggingTextureID");

	vector<float> NDC_pointPattern;
	for (float i = 0.0; i < 1.0; i += STEP)
	{
		for (float k = 0.0; k < 1.0; k += STEP)
		{
			NDC_pointPattern.push_back(i);
			NDC_pointPattern.push_back(k);
		}
	}

	glGenBuffers(1, &NDC_pointPatternBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, NDC_pointPatternBufferID);
	glBufferData(GL_ARRAY_BUFFER, NDC_pointPattern.size() * sizeof(float), &NDC_pointPattern[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); //vertex shader

	transparentObjectShader = Shader(FileSystem::getPath("Shaders/transparentObject.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/transparentObject.frag.glsl").c_str());
	refractionShader = Shader(FileSystem::getPath("Shaders/refraction.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/refraction.frag.glsl").c_str());

	debuggingShader = Shader(FileSystem::getPath("Shaders/ray_debugger.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/ray_debugger.frag.glsl").c_str(), FileSystem::getPath("Shaders/ray_debugger.geom.glsl").c_str());


	dragonWireframeShader = Shader(FileSystem::getPath("Shaders/geometry.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/geometryWireframe.frag.glsl").c_str(),
		FileSystem::getPath("Shaders/geometry.geom.glsl").c_str());

	dragonTransparentShader = Shader(FileSystem::getPath("Shaders/geometry.vert.glsl").c_str(),
		FileSystem::getPath("Shaders/geometryTransparent.frag.glsl").c_str());

	//sceneDepthShader = Shader(FileSystem::getPath("Shaders/geometryStage.vert.glsl").c_str(),
		//FileSystem::getPath("Shaders/geometryStage.frag.glsl").c_str());
}

void ForwardStage::Pass(Light *lights, vector<Object> objects, Object scene, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, GeometryStage geometryStage)
{
	glm::mat4 view;
	view = glm::lookAt(lights[0].Position(), objects[0].Position, glm::vec3(0.0, 1.0, 0.0));

	//scene depth
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, sceneDepthFrameBufferID);
	glClear(GL_DEPTH_BUFFER_BIT);
	transparentObjectShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(scene.GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(transparentObjectShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	scene.Draw(transparentObjectShader);
	glDisable(GL_DEPTH_TEST);




	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	dragonTransparentShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(dragonTransparentShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(dragonTransparentShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(dragonTransparentShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_FRONT);
	objects[0].Draw(dragonTransparentShader);

	glCullFace(GL_BACK);
	objects[0].Draw(dragonTransparentShader);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);


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

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, frontFaceNormalTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "front_normal"), 2);

	//
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, backFaceNormalTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "back_normal"), 3);

	//
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, frontPositionTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "front_position"), 4);
	//
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, backPositionRefractedTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "back_position"), 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, sceneDepthTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "scene_depth"), 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, scenePositionTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(refractionShader.Program, "scene_position"), 7);
	
	objects[0].Draw(refractionShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Debugging Shader - Draw to screen for now
	//glBindFramebuffer(GL_FRAMEBUFFER, debuggingFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	debuggingShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(debuggingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(debuggingShader.Program, "sceneModel"), 1, GL_FALSE, glm::value_ptr(scene.GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(debuggingShader.Program, "lightView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(debuggingShader.Program, "cameraView"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(debuggingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frontPositionTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(debuggingShader.Program, "p0_point"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionP1Texture.GetTextureID());
	glUniform1i(glGetUniformLocation(debuggingShader.Program, "p1_point"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, refractionP2Texture.GetTextureID());
	glUniform1i(glGetUniformLocation(debuggingShader.Program, "p2_point"), 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, frontFaceNormalTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(debuggingShader.Program, "p0_normal"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, backFaceNormalTexture.GetTextureID());
	glUniform1i(glGetUniformLocation(debuggingShader.Program, "p1_normal"), 4);

	glBindVertexArray(debuggerVAO);
	glDrawArrays(GL_POINTS, 0, 50*50);




	glEnable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	dragonWireframeShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(dragonWireframeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(objects[0].GetModelMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(dragonWireframeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(dragonWireframeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	objects[0].Draw(dragonWireframeShader);
	glDisable(GL_BLEND);

	glDisable(GL_DEPTH_TEST);
}
