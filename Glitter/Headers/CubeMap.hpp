
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class CubeMap
{
	private:
		int width;
		GLuint cubeMapTextureID;
		GLuint cubeMapFrameBufferID;

	public:
		CubeMap() {};
		CubeMap(int cubeWidth);
		void GenTextures();
		GLuint GetTextureID() { return cubeMapTextureID; };
		GLuint GetFrameBufferID() { return cubeMapFrameBufferID; };
		void CreateFace(int cubeFace);
};