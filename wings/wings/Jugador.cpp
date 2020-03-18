#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
//Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#include <iostream>
#include <iomanip>
#include <cmath>

#include "jugador.h"
#include "juego.h"

Jugador::Jugador(float x, float y, float z, float anguloX, float anguloY, float anguloZ, float vRotacion, float vMovimiento){
	this->x = x;
	this->y = y;
	this->z = z;
	this->anguloX = anguloX;
	this->anguloY = anguloY;
	this->anguloZ = anguloZ;
	this->vRotacion = vRotacion;
	this->vMovimiento = vMovimiento;
	//this->generarVAO();
}

Jugador::~Jugador(){

}

void Jugador::setX(float x) {
	this->x = x;
}
void Jugador::setY(float y) {
	this->y = y;
}
void Jugador::setZ(float z) {
	this-> z = z;
}
void Jugador::setAnguloX(float anguloX) {
	this->anguloX = anguloX;
}
void Jugador::setAnguloY(float anguloY) {
	this->anguloY = anguloY;
}
void Jugador::setAnguloZ(float anguloZ) {
	this->anguloZ = anguloZ;
}
void Jugador::setVmovimiento(float vMovimiento) {
	this->vMovimiento = vMovimiento;
}
void Jugador::setVrotacion(float vRotacion) {
	this->vRotacion = vRotacion;
}
void Jugador::setTextura(GLuint textura) {
	this->textura = textura;
}

//dibujar cuerpo y helice
void Jugador::draw(GLuint shader) {

	unsigned int transformLocK = glGetUniformLocation(shader, "transform");
	glm::mat4 transform;
	glm::mat4 temp;

	//ponemos activo el VAO
	glBindVertexArray(this->VAO);
	glBindTexture(GL_TEXTURE_2D, this->textura);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(this->x, this->y, this->z));
	transform = glm::rotate(transform, this->anguloZ, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, this->anguloX, glm::vec3(-1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, this->anguloY, glm::vec3(0.0f, 1.0f, 0.0f));
	//para colocar el avion, esta al reves
	transform = glm::rotate(transform, 3.1415f, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::scale(transform, glm::vec3(0.8f, 0.8f, 0.8f));
	
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));  //esto se usa para cargar la matriz en el shader
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 12927);
	glBindVertexArray(0);
	//resetamos matriz transofrm a identidad
	transform = glm::mat4();
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Jugador::generarVAO(std::vector< glm::vec3 > vertices, std::vector< glm::vec3 > normales, std::vector< glm::vec2 > uv, bool x) {
	//recordar cambiar la funcion de draw para que dibuje arrays
	GLuint VAO;
	unsigned int VBO, NBO, TBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//posicion
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	//normales
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normales.size() *sizeof(glm::vec3), &normales[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	//uv
	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, uv.size()* sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &TBO);

	if (x == true)
		this->VAO = VAO;
	else
		this->VAOhelice = VAO;
}