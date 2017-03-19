
#pragma once
#include "CubeMap.hpp"
#include "Constants.hpp"
#include <string>

class SkyBox
{
	private:
		CubeMap cubeMap;

	public:
		SkyBox();
		void Init(std::string skyboxFolderPath, std::string imageExtension);
		GLuint GetTextureID() { return cubeMap.GetTextureID(); };
		GLuint GetFrameBufferID() { return cubeMap.GetFrameBufferID(); };
};