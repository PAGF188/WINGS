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

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>


#define PI 3.1415
//variables globales
Jugador j;
//para el cubeMap
GLuint texturaCubeMap;
GLuint VAOCUbo;

//para el menu inicial
GLuint VAOcuadrado;
GLuint texturaMenu;
GLuint textura1;
GLuint textura2;
GLuint textura3;

//para cargar objeatos
std::vector< glm::vec3 > vertices2;
std::vector< glm::vec2 > uvs2;
std::vector< glm::vec3 > normals2;
GLuint textAvion;
GLuint textAvionDanado1, textAvionDanado2, textAvionDanado3;
GLuint textEnemigo;
float fov = INITIAL_FOV;
double horizontalAngle=180*PI/180, verticalAngle=0;

//textura proyectil
GLuint textPr;

//terreno
GLuint VAO;

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
void juego::setShaderCubeMap(GLuint sh) {
	this->shaderCubeMap = sh;
}

void juego::Init(){
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
	
	//cargamos VAO del menu inicial con su textura
	this->generarCuadrado();
	textura1 = this->cargarTextura((char *)"MENU1.jpg");
	textura2 = this->cargarTextura((char *)"MENU2.jpg");
	textura3 = this->cargarTextura((char *)"MENU3.jpg");

	//cargar .obj de jugador (avion)
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	this->cargarOBJ("plane.obj",vertices,uvs,normals);
	j.generarVAO(vertices, normals, uvs, true);
	textAvion=this->cargarTextura((char *)"plane.jpg");
	textAvionDanado1 = this->cargarTextura((char *) "planeDanado1.jpg");
	textAvionDanado2 = this->cargarTextura((char *) "planeDanado2.jpg");
	textAvionDanado3 = this->cargarTextura((char *) "planeDanado3.jpg");
	textEnemigo = this->cargarTextura((char *) "enemigo.jpg");
	
	j.setTextura(textAvion);
	//cargamos .obj de los enemigos (helicoptero)
	this->cargarOBJ("plane.obj", vertices2, uvs2, normals2);
	//cargamos cubemap con su textura
	this->generarCuboBox();
	std::vector<std::string> faces ={"right.jpg","left.jpg","top.jpg","bottom.jpg","front.jpg","back.jpg"};
	texturaCubeMap = this->loadCubemap(faces);
	
	/************CARGAR TEXTURA PROYECTIL**************/
	textPr = this->cargarTextura((char *) "proyectil2.jpg");

	/*****GENERACION DE TERRENO******/
	Suelo();

	this->estado = MENU;
}

void juego::ProcessInput(float deltaTime, GLFWwindow* window){
	
	double xpos = 0, ypos = 0;
	glfwGetCursorPos(window, &xpos, &ypos);

	if (this->estado == MENU) {
		if (this->click == true && (xpos > this->ANCHO / 2 - 145 && xpos < this->ANCHO / 2 + 145) && (ypos > this->ALTO / 2 - 190 && ypos < this->ALTO / 2 - 30)) {
			//si pulsa en jugar, iniciamos el juego
			this->estado = JUEGO;
		}
		else if (this->click == true && (xpos > this->ANCHO / 2 - 145 && xpos < this->ANCHO / 2 + 145) && (ypos > this->ALTO / 2 + 0 && ypos < this->ALTO / 2 + 160)) {
			exit(0);
		}
	}
	
	//para estado de juego JUEGO
	if (this->estado == JUEGO) {
		/*******************movimiento de raton***********/
		
		glfwSetCursorPos(window, ANCHO / 2, ALTO / 2+10);  //cuidado con este 10
		horizontalAngle += MOUSE_SPEED * deltaTime * float((float)ANCHO / 2 - xpos);
		verticalAngle += MOUSE_SPEED * deltaTime * float((float)ALTO / 2+10 - ypos);		

		direction=glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
		direction=glm::normalize(direction); //vector direccion ya normalizado

		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		up = glm::cross(right, direction);

		//rotacion izquierda
		if (this->Keys[GLFW_KEY_A]) {
			j.setAnguloZ(j.getAnguloZ() + j.getvRotacion() * deltaTime);
			j.setX(j.getX() - j.getvRotacion() *2 * deltaTime);
		}
		//rotacion derecha
		if (this->Keys[GLFW_KEY_D]) {
			j.setAnguloZ(j.getAnguloZ() - j.getvRotacion() * deltaTime);
			j.setX(j.getX() + j.getvRotacion() *2 * deltaTime);
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
			this->proyectiles_jugador.push_back(new proyectil(glm::vec3(j.getX(),j.getY(),j.getZ()),direction,20.0,textPr));
			this->click = false;
		}
	}

	//se los pasamos a jugador
	j.setAnguloX(atan(direction[1]/direction[2]));
	j.setAnguloY(atan(direction[0]/direction[2])); //arcotangente de 
	
}

void juego::Update(float deltaTime, GLFWwindow* window){

	if (this->estado == MENU) {
		//comprobamos posicion del raton para saber que textura mostrar
		double xpos = 0, ypos = 0;
		glfwGetCursorPos(window, &xpos, &ypos);
		//si esta en el boton 1 mostramos la segunda textura.
		if ((xpos > this->ANCHO / 2 - 145 && xpos < this->ANCHO / 2 + 145) && (ypos > this->ALTO / 2 - 190 && ypos < this->ALTO / 2 - 30)){
			texturaMenu = textura2;
		}
		//si esta en el boton 2 mostramos la 3 textura
		else if ((xpos>this->ANCHO / 2 - 145 && xpos <this->ANCHO / 2 + 145) && (ypos>this->ALTO / 2 +0 && ypos <this->ALTO / 2 + 160)) {
			texturaMenu = textura3;
		}
		//si no esta en ninguno, mostramos la textura base
		else {
			texturaMenu = textura1;
		}
	}

	//si el juego esta activo, actualizamos todo
	if (this->estado == JUEGO) {

		/******actualizacion del jugador****/
		//incrementamos posicion del avion
		j.setX(j.getX() + direction[0] * j.getvMovimiento() * deltaTime);
		j.setY(j.getY() + direction[1] * j.getvMovimiento() * deltaTime);
		j.setZ(j.getZ() + direction[2] * j.getvMovimiento() * deltaTime);

		//actualizamos proyectiles
		for (auto& pointer : this->proyectiles_jugador) {
			//recibe el jugador para poder actualizar la posicion
			(pointer)->actualizar(deltaTime);

			if (abs((pointer)->getPos()[2] - j.getZ()) > DISTANCIA_PROYECTIL) {
				//delete pointer; comporbar esta linea
				pointer = nullptr;
			}
			auto it = std::find(this->proyectiles_jugador.begin(), this->proyectiles_jugador.end(), nullptr);
			if (it != this->proyectiles_jugador.end()) {
				this->proyectiles_jugador.erase(it);
			}
		}
		/******fin actualizacion jugador*****/

		//generamos un enemigo
		if (j.getZ()<-10 && this->enemigos.size()==0) {
			enemigo *e;
			e = new enemigo(glm::vec3(j.getX(), j.getY() + 5, j.getZ() + 5), glm::normalize(glm::vec3(j.getX(), j.getY(), j.getZ() - 30) - glm::vec3(j.getX(), j.getY() + 5, j.getZ() + 5)), 15, 0, 0, 0, j.getvRotacion());
			e->generarVAO(vertices2, normals2, uvs2);
			e->setTextura(textEnemigo);
			this->enemigos.push_back(e);
		}

		/*actualizamos todos los enemigos*/
		for (auto& pointer : this->enemigos) {
			(pointer)->actualizar(deltaTime, j, direction);
		}

		//finalmente, deteccion de colisiones:
		for (auto& bala : this->proyectiles_jugador) {
			for (auto& enem : this->enemigos) {
				//si la distancia entre en centro de una bala y el centro de un avion es menor a la suma de los radios-->colision
				if (glm::distance(((bala)->getPos()), ((enem)->getPos())) <= 2.0f) {
					//colision
					//decrementamos la vida del enemigo
					(enem)->decrementarVida(1);
					//ponemos la textura de daño
					if((enem)->getVida() == 3) {
						(enem)->setTextura(textAvionDanado1);
					}
					else if ((enem)->getVida() == 2) {
						(enem)->setTextura(textAvionDanado2);
					}
					else if ((enem)->getVida() == 1) {
						(enem)->setTextura(textAvionDanado3);
					}
					//si es cero lo eliminamos
					if ((enem)->getVida() == 0) {
						enem = nullptr;
						auto it = std::find(this->enemigos.begin(), this->enemigos.end(), nullptr);
						if (it != this->enemigos.end()) {
							this->enemigos.erase(it);
						}
					}
					//eliminamos el proyectil
					bala = nullptr;
					auto it = std::find(this->proyectiles_jugador.begin(), this->proyectiles_jugador.end(), nullptr);
					if (it != this->proyectiles_jugador.end()) {
						this->proyectiles_jugador.erase(it);
					}
				}
			}
		}
	}

}



void juego::Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (this->estado == MENU) {
		glUseProgram(this->shaderProgram);
		this->CamaraInicio();

		//dibujar menu
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAOcuadrado);
		glBindTexture(GL_TEXTURE_2D,texturaMenu);
		unsigned int transformLocK = glGetUniformLocation(this->shaderProgram, "transform");
		glm::mat4 transform = glm::mat4();
		transform = glm::rotate(transform,glm::radians(-90.0f), glm::vec3(0, 0, 1));
		transform = glm::scale(transform, glm::vec3(3.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}

	if (this->estado == JUEGO) {
		
		this->Camara();
		//dibujamos el cubeBox	CON OTRO SHADER
		glDepthMask(GL_FALSE);
		glUseProgram(this->shaderCubeMap);
		//projection
		glm::mat4 projection;
		projection = glm::mat4();
		projection = glm::perspective(glm::radians(fov), (float)ANCHO / (float)ALTO, 0.0001f, 1000.0f);
		unsigned int projectionLoc = glGetUniformLocation(this->shaderCubeMap, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//cargamos view
		glm::mat4 view = glm::mat4();
		view = glm::lookAt(
			glm::vec3(j.getX(), j.getY() + 2, j.getZ() + 2.5),           // posicion
			glm::vec3(j.getX(), j.getY() + 2, j.getZ() + 2) + direction, //mira (vector)
			up
		);
		glm::mat4 view_final = glm::mat4(glm::mat3(view));
		unsigned int viewLoc = glGetUniformLocation(shaderCubeMap, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view_final));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAOCUbo);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texturaCubeMap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glDepthMask(GL_TRUE);
		glUseProgram(this->shaderProgram);
		//fin dibujo cubeBox

		//dibujamos el suelo
		//borrar de aqui
		glm::mat4 transform;
		unsigned int transformLocK = glGetUniformLocation(this->shaderProgram, "transform");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		for (int i = -50; i <= 50; i++) {
			for (int j = -50; j <= 50; j++) {
				transform = glm::mat4();
				transform = glm::translate(transform, glm::vec3(i, 0.0f, j));
				glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
		glBindVertexArray(0);
		//a aqui

		j.draw(this->shaderProgram);

		//dibujamos los proyectiles
		for (std::vector<proyectil*>::iterator aux = this->proyectiles_jugador.begin(); aux != this->proyectiles_jugador.end(); ++aux) {
			(*aux)->draw(this->shaderProgram);
		}
		//dibujamos los enemigos
		for (auto& pointer : this->enemigos) {
			(pointer)->draw(this->shaderProgram);
		}
	}

}


/*FUNCIONES DE LA CAMARA durante el juego*/
void juego::Camara() {
	glViewport(0, 0, ANCHO, ALTO);

	//proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(fov), (float)ANCHO / (float)ALTO, 0.0001f, 100.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//view
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(
		glm::vec3(j.getX(), j.getY() + 2, j.getZ()+2.5),           // posicion
		glm::vec3(j.getX(), j.getY() + 2, j.getZ()+2) + direction, //mira (vector)
		up
	);

	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void juego::CamaraInicio() {
	glViewport(0, 0, ANCHO, ALTO);
	//proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(fov), (float)ANCHO / (float)ALTO, 0.01f, 10.0f);
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//view
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(
		glm::vec3(0.0f,0.0f,5.0f),           // posicion
		glm::vec3(0.0f, 0.0f, 0.0f), //mira (vector)
		glm::vec3(0.0f,1.0f,0.0f)
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
		int res = fscanf_s(file, "%s", lineHeader, sizeof(lineHeader) / sizeof(char));
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

GLuint juego::cargarTextura(char *path) {

		GLuint textura;
		glGenTextures(1, &textura);
		glBindTexture(GL_TEXTURE_2D, textura);

		int ancho, alto, canales;
		unsigned char *data = NULL;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load(path, &ancho, &alto, &canales, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		stbi_image_free(data);
		return(textura);
}

GLuint juego::loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			exit(-1);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void juego::generarCuboBox() {

	unsigned int VBO;

	float cubeBox[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAOCUbo);
	glBindVertexArray(VAOCUbo);

	//vertices
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeBox), cubeBox, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void juego::generarCuadrado() {
	
	unsigned int VBO, NBO, TBO;

	float vertices[] = {
		1.0f,-1.0f,0.0f, // triángulo 1 : comienza
		1.0f,1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, // triángulo 1 : termina

		-1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,
		
	};

	float normales[] = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,

		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
	};

	float uv[] = {
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,

		1.0f,0.0f,
		0.0f,0.0f,
		0.0f,1.0f,
	};

	glGenVertexArrays(1, &VAOcuadrado);
	glBindVertexArray(VAOcuadrado);

	//posicion
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	//normales
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normales), normales, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	//uv
	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &TBO);
}

void juego::Suelo() {
	unsigned int VBO, EBO;

	float vertices[] = {
		0.5f,  0.0f,  0.5f,  0.0f, 1.0f,0.0f,
		-0.5f,  0.0f,  0.5f,  .0f, 1.0f,0.0f,
		0.5f,  0.0f,  -0.5f,  .0f, 1.0f,0.0f,
		-0.5f,  0.0f,  -0.5f, .0f, 1.0f,0.0f,

		0.5f,  -1.0f,  0.5f,  0.0f, 0.0f,0.0f,   //borrar estos
		-0.5f,  -1.0f,  0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  -1.0f,  -0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  -1.0f,  -0.5f,  0.0f, 0.0f,0.0f,

	};
	unsigned int indices[] = {  // empieza desde cero
								//0,2,3,
								//3,1,0,
		0,2,3,
		3,1,0,
		4,6,7,
		7,5,4,
		4,0,1,
		1,5,4,
		7,3,2,
		2,6,7,
		6,2,0,
		0,4,6,
		5,1,3,
		3,7,5
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAO);

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
}