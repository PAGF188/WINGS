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


void Jugador::draw(GLuint shader) {

	unsigned int transformLocK = glGetUniformLocation(shader, "transform");
	glm::mat4 transform;
	glm::mat4 temp;

	//ponemos activo el VAO del cuadrado
	glBindVertexArray(this->VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(this->x, this->y, this->z));
	transform = glm::rotate(transform, this->anguloZ, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, this->anguloX, glm::vec3(-1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, this->anguloY, glm::vec3(0.0f, 1.0f, 0.0f));
	
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));  //esto se usa para cargar la matriz en el shader
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	//resetamos matriz transofrm a identidad
	transform = glm::mat4();
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
}

void Jugador::generarVAO() {
	
	GLuint VAOCuadrado;
	unsigned int VBO, EBO;

	float vertices[] = {
		-1.5f, -0.5f,  3.5f,  0.0f, 0.0f,0.0f,  //z positiva
		1.5f, -0.5f,  3.5f,  1.0f, 0.0f,0.0f,
		1.5f,  0.5f,  3.5f,  1.0f, 1.0f,0.0f,
		-1.5f,  0.5f,  3.5f,  1.0f, 1.0f,0.0f,

		-1.5f, -0.5f,  -3.5f,  0.0f, 0.0f,0.0f,  //z negativa
		1.5f, -0.5f,  -3.5f,  1.0f, 0.0f,0.0f,
		1.5f,  0.5f,  -3.5f,  1.0f, 1.0f,0.0f,
		-1.5f,  0.5f,  -3.5f,  1.0f, 1.0f,0.0f,

	};
	unsigned int indices[] = {  // empieza desde cero
								//cara frente
		0, 1, 2,
		2, 3, 0,
		//cara atras
		4,7,6,
		6,5,4,
		//cara derecha
		1,5,6,
		6,2,1,
		//cara izquierda
		0,3,7,
		7,4,0,
		//cara arriba
		2,6,7,
		7,3,2,
		//cara abajo
		1,0,4,
		4,5,1,
	};

	glGenVertexArrays(1, &VAOCuadrado);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAOCuadrado);

	//datos para los vertices VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//datos para los indices EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	this->VAO = VAOCuadrado;

}