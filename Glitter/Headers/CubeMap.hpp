
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class CubeMap
{
	private:
		int width;
		GLuint cubeMapTextureID;
		GLuint cubeMapFrameBufferID;

		glm::vec3 cubeOrientations[6];
		glm::vec3 cubeUpDirections[6];

	public:
		CubeMap(int cubeWidth);
		GLuint GetTextureID() { return cubeMapTextureID; };
		GLuint GetFrameBufferID() { return cubeMapFrameBufferID; };

		void CreateFace(int cubeFace);
};