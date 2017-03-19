
#include "Texture2D.hpp"

Texture2D::Texture2D()
{
	glGenTextures(1, &textureID);
}

void Texture2D::CreateTexture(int width, int height, GLuint internalFormat, GLenum format, GLenum type, GLenum paramName1, GLint param1, GLenum paramName2, GLint param2)
{
	//internal format - give it a size
	//format is just rgba or so
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, paramName1, param1);
	glTexParameteri(GL_TEXTURE_2D, paramName2, param2);
}