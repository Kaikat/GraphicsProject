
#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

class Texture2D
{
	private:
		GLuint textureID;

	public:
		Texture2D();
		GLuint GetTextureID() { return textureID; };
		void PrintID(std::string textureName) { std::cout << textureName << "ID: " << textureID << std::endl; };
		void CreateTexture(int width, int height, GLuint internalFormat, GLenum format, GLenum type, GLenum paramName1, GLint param1, GLenum paramName2, GLint param2);
};