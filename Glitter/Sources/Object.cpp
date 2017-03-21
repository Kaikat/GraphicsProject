
#include "Object.hpp"

Object::Object(Model objectModel, float scale, glm::vec3 translation)
{
	model = objectModel;
	Position = translation;

	modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, translation);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
}

void Object::Draw(Shader shader)
{
	model.Draw(shader);
}