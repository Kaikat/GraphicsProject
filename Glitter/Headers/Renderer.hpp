
#pragma once
#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"

#include "Light.hpp"
#include "glitter.hpp"

#include <string>
#include <memory>
#include <map>

#include "GeometryStage.hpp"
#include "LightingStage.hpp"

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

		////////////// Passes | Stages //////////////
		std::unique_ptr<GeometryStage> geometryStage;
		std::unique_ptr<LightingStage> lightingStage;

		////////////// Input //////////////
		map<int, bool> keyPressed;
		map<int, bool> mouseButtonPressed;
		bool IsKeyPressed(int key);
		bool IsMouseButtonPressed(int button);

		////////////// Other //////////////
		void SetWorldModel(string filename, float scale);
		void SetCamera(glm::vec3 cameraPosition);
		void InitLights();

	public:
		Renderer();
		void Init(glm::vec3 cameraPosition, string worldModelFilename, float worldModelScale = 1.0f);
		void Update(float deltaTime);
		void Render();

		///////////// Input ////////////// 
		void OnKeyPressed(int key);
		void OnKeyReleased(int key);

		void OnMouseButtonPressed(int button);
		void OnMouseButtonReleased(int button);
		void OnMouseMoved(float deltaX, float deltaY);
		void OnMouseScrolled(int amountScrolled);
};