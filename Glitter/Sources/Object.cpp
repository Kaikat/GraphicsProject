
#include "Object.hpp"

Object::Object(Model objectModel, float scale, glm::vec3 translation)
{
	model = objectModel;
	modelMatrix = glm::mat4();
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale)); 
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0, 0));
}

void Object::Draw(Shader shader)
{
	model.Draw(shader);
}