
#pragma once
#include <glm/glm.hpp>
#include "glitter.hpp"

#define TOTAL_LIGHTS 3

const glm::vec3 QuadPositions[4] = { 
	glm::vec3(-1.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f)
};

const float QuadUVs[8] = { 
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f 
};

const GLuint QuadTriangleIndeces[6] = { 0, 1, 2, 2, 3, 0 };

////////////// ==== CUBE MAPS ==== //////////////
//For changing the cube map positions
const glm::vec3 CubeOrientations[6] = {
	glm::vec3(1.0f, 0.0f, 0.0f), //right x+
	glm::vec3(-1.0f, 0.0f, 0.0f), //left x-
	glm::vec3(0.0f, 1.0f, 0.0f), //top y+
	glm::vec3(0.0f, -1.0f, 0.0f), //bottom y-
	glm::vec3(0.0f, 0.0f, 1.0f), //back z+
	glm::vec3(0.0f, 0.0f, -1.0f) //front z-
};

const glm::vec3 CubeUpDirections[6] = {
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(0.0f, -1.0f, 0.0f),
};

const GLfloat cubeVertices[] = {
	// Positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};