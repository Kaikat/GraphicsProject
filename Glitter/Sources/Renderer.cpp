
#include "Renderer.hpp"
#include "GLFW\glfw3.h"
#include <filesystem.hpp>

Renderer::Renderer()
{
	lights[0] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0);
	lights[1] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0);
	lights[2] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0);
}

void Renderer::SetCamera(glm::vec3 cameraPosition)
{
	camera = unique_ptr<Camera>(new Camera(cameraPosition));
	projection = glm::perspective(glm::radians(camera->Zoom), (GLfloat)mWidth / (GLfloat)mHeight, mNear, mFar);
	view = camera->GetViewMatrix();

	for (int i = 0; i < 3; i++)
		lights[i].Init();
}

void Renderer::SetWorldModel(string filename)
{
	// The world doesn't move or change so this only has to be set once
	worldModel = unique_ptr<Model>(new Model(filename));
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.05f));    // The sponza model is too big, scale it first
	model = glm::translate(model, glm::vec3(-1, 0, 0));

	//sampleShader = unique_ptr<Shader>(new Shader(FileSystem::getPath("Shaders/geometry.vert.glsl").c_str(), FileSystem::getPath("Shaders/geometry.frag.glsl").c_str()));
	geometryStage = unique_ptr<GeometryStage>(new GeometryStage());
	lightingStage = unique_ptr<LightingStage>(new LightingStage());
	//glEnable(GL_DEPTH_TEST);
}

void Renderer::Render()
{
	// Background Fill Color
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, mWidth, mHeight);

	/*sampleShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(sampleShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(sampleShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sampleShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	worldModel->Draw(*sampleShader);*/

	geometryStage->Pass(*worldModel, model, view, projection);
	lightingStage->Pass(lights, *worldModel, model, view, *geometryStage);
}

void Renderer::Update(float deltaTime)
{
	view = camera->GetViewMatrix();

	if (IsKeyPressed(GLFW_KEY_W))
	{
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (IsKeyPressed(GLFW_KEY_S))
	{
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (IsKeyPressed(GLFW_KEY_A))
	{
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (IsKeyPressed(GLFW_KEY_D))
	{
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
}

////////////////////////////////////////////
////////////// Keyboard Input //////////////
////////////////////////////////////////////

bool Renderer::IsKeyPressed(int key)
{
	auto keyState = keyPressed.find(key);
	
	//If the key is not in the map return false otherwise return if it is pressed or not
	return keyState == keyPressed.end() ? false : keyState->second;
}

void Renderer::OnKeyPressed(int key)
{
	keyPressed[key] = true;
	switch (key)
	{
		default:
			return;
	}
}

void Renderer::OnKeyReleased(int key)
{
	keyPressed[key] = false;
	switch (key)
	{
		default:
			return;
	}
}

/////////////////////////////////////////
////////////// Mouse Input //////////////
/////////////////////////////////////////

bool Renderer::IsMouseButtonPressed(int button)
{
	auto state = mouseButtonPressed.find(button);
	return state == mouseButtonPressed.end() ? false : state->second;
}

void Renderer::OnMouseButtonPressed(int button)
{
	mouseButtonPressed[button] = true;
}

void Renderer::OnMouseButtonReleased(int button)
{
	mouseButtonPressed[button] = false;
}

void Renderer::OnMouseMoved(float deltaX, float deltaY)
{
	if (IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		camera->ProcessMouseMovement(deltaX, deltaY);
	}
}

void Renderer::OnMouseScrolled(int amountScrolled)
{
	camera->ProcessMouseScroll(amountScrolled);
}