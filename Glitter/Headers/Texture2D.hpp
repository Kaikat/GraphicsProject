
#pragma once
#include <glad/glad.h>

class Texture2D
{
	private:
		GLuint textureID;

	public:
		Texture2D();
		GLuint GetTextureID() { return textureID; };

		//TODO
		void CreateTexture();
};