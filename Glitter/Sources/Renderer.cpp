
#include "Renderer.hpp"
#include "GLFW\glfw3.h"
#include <filesystem.hpp>

Renderer::Renderer()
{
	lights[0] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0);
	lights[1] = Light(glm::vec3(0.0f, 20.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0);
	lights[2] = Light(glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0);
}

void Renderer::Init(glm::vec3 cameraPosition, string worldModelFilename, float worldModelScale)
{
	SetCamera(cameraPosition);
	SetWorldModel(worldModelFilename, worldModelScale);
	InitLights();

	//Initialize Stages
	geometryStage = unique_ptr<GeometryStage>(new GeometryStage());
	lightingStage = unique_ptr<LightingStage>(new LightingStage(BRDF_TYPE::Blinn_Phong));
}

void Renderer::SetCamera(glm::vec3 cameraPosition)
{
	camera = unique_ptr<Camera>(new Camera(cameraPosition));
	projection = glm::perspective(glm::radians(camera->Zoom), (GLfloat)mWidth / (GLfloat)mHeight, mNear, mFar);
	view = camera->GetViewMatrix();
}

void Renderer::SetWorldModel(string filename, float scale)
{
	// The world doesn't move or change so this only has to be set once
	worldModel = unique_ptr<Model>(new Model(filename));
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(scale));    // The sponza model is too big, scale it first
	model = glm::translate(model, glm::vec3(-1, 0, 0));
}

void Renderer::InitLights()
{
	for (int i = 0; i < TOTAL_LIGHTS; i++)
	{
		lights[i].Init();
	}
}

void Renderer::Render()
{
	// Background Fill Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, mWidth, mHeight);

	geometryStage->Pass(*worldModel, model, view, projection);
	lightingStage->Pass(lights, *worldModel, model, view, projection, *geometryStage);
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