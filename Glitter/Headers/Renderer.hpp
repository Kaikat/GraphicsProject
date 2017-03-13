
#pragma once
#include "Light.hpp"

class Renderer
{
	private:
		Light lights[3];

	public:
		Renderer();
		void GeometryPass();
		void LightPass();

		//Input 
		void OnKeyPressed(int key);
		void OnKeyReleased(int key);
};