
#pragma once
#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"

#include "Light.hpp"
#include "glitter.hpp"

#include <string>
#include <memory>
#include <map>


class Renderer
{
	private:
		std::unique_ptr<Camera> camera;
		std::unique_ptr<Model> worldModel;

		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;

		Light lights[3];
		std::unique_ptr<Shader> sampleShader;

		map<int, bool> keyPressed;
		map<int, bool> mouseButtonPressed;
		
		bool IsKeyPressed(int key);
		bool IsMouseButtonPressed(int button);

	public:
		Renderer();
		void SetWorldModel(string filename);
		void SetCamera(glm::vec3 cameraPosition);
		void Update(float deltaTime);
		void Render();

		void GeometryPass();
		void LightPass();

		//Input 
		void OnKeyPressed(int key);
		void OnKeyReleased(int key);
		void OnMouseButtonPressed(int button);
		void OnMouseButtonReleased(int button);
		void OnMouseMoved(float deltaX, float deltaY);
		void OnMouseScrolled(int amountScrolled);
};