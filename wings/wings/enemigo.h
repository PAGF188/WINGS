#pragma once
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
#include "Jugador.h"
#include "Sphere.h"

class enemigo
{

public:
	enemigo(glm::vec3 pos, glm::vec3 direction, float velocidad = 4, float anguloX = 0.0, float anguloY = 0.0, float anguloZ = 0.0, float vRotacion = 0);
	~enemigo();

	glm::vec3 getPos() const { return pos; }
	glm::vec3 getDirection() const { return direction; }
	int getVida() const { return vida; }
	void set(glm::vec3 pos, glm::vec3 direction, float velocidad, float anguloX, float anguloY, float anguloZ, float vRotacion);
	void setPos(glm::vec3 pos);
	void setDirection(glm::vec3 direction);
	void setVelocidad(float velocidad);
	void setAnguloX(float anguloX1);
	void setAnguloY(float anguloZ1);
	void setAnguloZ(float anguloX2);
	void setVrotacion(float vRotacion);
	void setTextura(GLuint textura);
	

	/*******metodos*******/
	void actualizar(float deltaTime, Jugador jugador,glm::vec3 direction); //para actualizar su posicion
	void draw(GLuint shader);
	void generarVAO(std::vector< glm::vec3 > vertices, std::vector< glm::vec3 > normales, std::vector< glm::vec2 > uv);
	void decrementarVida(int a);

private:
	//atributos
	glm::vec3 pos;
	glm::vec3 direction;
	//rotaciones en los ejes
	float anguloX;
	float anguloY;
	float anguloZ;
	float velocidad;
	float vRotacion;
	int vida;
	GLuint VAO;
	GLuint textura;
};

