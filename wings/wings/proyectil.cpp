#include "proyectil.h"



proyectil::proyectil(glm::vec3 pos, glm::vec3 direction,float velocidad,GLuint textura)
{
	this->pos = pos;
	this->direction = direction;
	this->velocidad = velocidad;
	this->textura = textura;
}


proyectil::~proyectil()
{
}

void proyectil::setPos(glm::vec3 pos) {
	this->pos = pos;
}
void proyectil::setDirection(glm::vec3 dir) {
	this->direction = dir;
}
void proyectil::setVelocidad(float v) {
	this->velocidad = v;
}

void proyectil::actualizar(float deltaTime) {

	this->pos = this->pos + direction*this->velocidad*deltaTime;

}
void proyectil::setTextura(GLuint textura) {
	this->textura = textura;
}
void proyectil::draw(GLuint shader) {
	Sphere esfera(1,12,6,true);

	unsigned int transformLocK = glGetUniformLocation(shader, "transform");
	glm::mat4 transform;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, this->textura);

	transform = glm::mat4();
	transform = glm::translate(transform, this->pos);
	transform = glm::scale(transform, glm::vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));

	esfera.draw();

}

void proyectil::cargarVao() {

}
