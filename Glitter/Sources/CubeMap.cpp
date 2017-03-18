
#include "CubeMap.hpp"

CubeMap::CubeMap(int cubeWidth)
{
	width = cubeWidth;
}

void CubeMap::GenTextures()
{
	//Create IDs
	glGenTextures(1, &cubeMapTextureID);
	glGenFramebuffers(1, &cubeMapFrameBufferID);

	for (int cubeFace = 0; cubeFace < 6; cubeFace++)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFace, 0, GL_DEPTH_COMPONENT, width, width, 0,
			GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}

void CubeMap::CreateFace(int cubeFace)
{
	glBindFramebuffer(GL_FRAMEBUFFER, cubeMapFrameBufferID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFace, cubeMapTextureID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, width, width);
}