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

#include "juego.h"
#include "Jugador.h"

#define PI 3.1415
//variables globales
Jugador j;

float fov = INITIAL_FOV;
double horizontalAngle=180*PI/180, verticalAngle=0;


juego::juego(unsigned int ancho, unsigned int alto) 
	: estado(MENU), ANCHO(ancho), ALTO(alto) 
{ 
}

juego::~juego()
{

}
void juego::setShaderProgram(GLuint sh) {
	this->shaderProgram = sh;
}

void juego::Init(){
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	
	j.generarVAO();
	this->estado = JUEGO;
}

void juego::ProcessInput(float deltaTime, GLFWwindow* window){
	//para estado de juego JUEGO
	if (this->estado == JUEGO) {
		/*******************movimiento de raton***********/
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, ANCHO / 2, ALTO / 2+40);  //cuidado con este 200
		horizontalAngle += MOUSE_SPEED * deltaTime * float((float)ANCHO / 2 - xpos);
		verticalAngle += MOUSE_SPEED * deltaTime * float((float)ALTO / 2+40 - ypos);		

		direction=glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		up = glm::cross(right, direction);

		//rotacion izquierda
		if (this->Keys[GLFW_KEY_A]) {
			j.setAnguloZ(j.getAnguloZ() + j.getvRotacion() * deltaTime);
			j.setX(j.getX() - j.getvRotacion() *6 * deltaTime);
		}
		//rotacion derecha
		if (this->Keys[GLFW_KEY_D]) {
			j.setAnguloZ(j.getAnguloZ() - j.getvRotacion() * deltaTime);
			j.setX(j.getX() + j.getvRotacion() *6 * deltaTime);
		}
		//mas velocidad
		if (this->Keys[GLFW_KEY_W] && j.getvMovimiento()<=MAX_VELOCIDAD_MOVIMINETO) {
			j.setVmovimiento(j.getvMovimiento() + INCREMENTO_VELOCIDAD);
			fov = fov - INCREMENTO_FOV;
		}
		//al dejas de pulsar W menos velocidad
		if (this->Keys[GLFW_KEY_W]==false && j.getvMovimiento()>V_MOVIMINETO) {
			j.setVmovimiento(j.getvMovimiento() - INCREMENTO_VELOCIDAD);
			fov = fov + INCREMENTO_FOV;
		}
		//disparar
		if (this->click == true) {
			this->proyectiles_jugador.push_back(new proyectil(glm::vec3(j.getX(),j.getY(),j.getZ()),direction,20.0));
			this->click = false;
		}
	}

	//se los pasamos a jugador
	j.setAnguloX(atan(direction[1]/direction[2]));
	j.setAnguloY(atan(direction[0]/direction[2])); //arcotangente de 
	
	
}

void juego::Update(float deltaTime){
	//incrementamos posicion del avion
	j.setX(j.getX() + direction[0] * j.getvMovimiento() * deltaTime);
	j.setY(j.getY() + direction[1] * j.getvMovimiento() * deltaTime);
	j.setZ(j.getZ() + direction[2] * j.getvMovimiento() * deltaTime);


	for (std::vector<proyectil*>::iterator aux = this->proyectiles_jugador.begin(); aux != this->proyectiles_jugador.end(); ++aux) {
		(*aux)->actualizar(deltaTime);
	}

}



void juego::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->Camara();
	j.draw(this->shaderProgram);

	//dibujamos los proyectiles
	for (std::vector<proyectil*>::iterator aux = this->proyectiles_jugador.begin(); aux != this->proyectiles_jugador.end(); ++aux) {
		(*aux)->draw(this->shaderProgram);
	}
}


/*FUNCIONES DE LA CAMARA*/
void juego::Camara() {
	glViewport(0, 0, ANCHO, ALTO);

	//proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(fov), (float)ANCHO / (float)ALTO, 0.0001f, 1000.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//view
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(
		glm::vec3(j.getX(), j.getY() + 2, j.getZ()+2),           // posicion
		glm::vec3(j.getX(), j.getY() + 2, j.getZ()+2) + direction, //mira (vector)
		up
	);

	
	
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void juego::cargarOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals) {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		exit(-1);
	}

	while (1) {
		char lineHeader[128];
		// Lee la primera palabra de la línea
		int res = fscanf_s(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				exit(-1);
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

}