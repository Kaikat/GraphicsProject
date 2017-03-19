
#include "SkyBox.hpp"
#include <soil/soil.h>
#include "filesystem.hpp"

#define CUBE_FACES 6

using namespace std;

SkyBox::SkyBox()
{
}

void SkyBox::Init(string skyboxFolderPath, string imageExtension)
{
	int width, height;
	unsigned char* image[6];
	for (GLuint i = 0; i < CUBE_FACES; i++)
	{
		string path = FileSystem::getPath(skyboxFolderPath + CubeFileNames[i] + imageExtension);
		image[i] = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	}

	cubeMap = CubeMap(width);
	cubeMap.Init(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, image);
}