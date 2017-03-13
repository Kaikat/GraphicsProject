
#include "Renderer.hpp"

Renderer::Renderer()
{
	lights[0] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0);
	lights[1] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0);
	lights[2] = Light(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0);
}

void Renderer::LightPass()
{

}

void Renderer::GeometryPass()
{

}

void Renderer::OnKeyPressed(int key)
{
	switch (key)
	{
		default:
			return;
	}
}

void Renderer::OnKeyReleased(int key)
{
	switch (key)
	{
		default:
			return;
	}
}