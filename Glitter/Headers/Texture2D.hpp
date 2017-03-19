
#pragma once
#include <glad/glad.h>

class Texture2D
{
	private:
		GLuint textureID;

	public:
		Texture2D();
		GLuint GetTextureID() { return textureID; };
		void CreateTexture(int width, int height, GLuint internalFormat, GLenum format, GLenum type, GLenum paramName1, GLint param1, GLenum paramName2, GLint param2);
};