
#include "ShadowMap.hpp"
#include "Constants.hpp"

#define CUBE_DIMENSION 1024

ShadowMap::ShadowMap()
{
	cubeMap = CubeMap(CUBE_DIMENSION);
}

void ShadowMap::Init()
{
	cubeMap.GenTextures();
}