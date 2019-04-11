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
#include "Sphere.h"

class proyectil
{
private:
	glm::vec3 pos;
	glm::vec3 direction;
	float velocidad;
	GLuint VAO;

public:

	proyectil(glm::vec3 pos, glm::vec3 direction, float velocidad=4);
	~proyectil();

	glm::vec3 getPos() const { return pos; }
	void setPos(glm::vec3 pos);
	void setDirection(glm::vec3 direction);
	void setVelocidad(float velocidad);

	/*******metodos*******/
	void actualizar(float deltaTime); //para actualizar su posicion
	void draw(GLuint shader);
	void cargarVao();
};


