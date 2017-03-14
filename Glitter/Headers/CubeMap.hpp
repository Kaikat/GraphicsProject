
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
		CubeMap() {};
		CubeMap(int cubeWidth);
		void GenTextures();
		GLuint GetTextureID() { return cubeMapTextureID; };
		GLuint GetFrameBufferID() { return cubeMapFrameBufferID; };
		glm::vec3 GetCubeOrientation(int cubeFace) { return cubeOrientations[cubeFace]; };
		glm::vec3 GetCubeUpDirection(int cubeFace) { return cubeUpDirections[cubeFace]; };
		void CreateFace(int cubeFace);
};