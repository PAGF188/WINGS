/*clase juego.
Basicamente permite las operaciones de INIT, ProcessInput, Update, Render
Una vez inicializado el entorno de trabajo, recogeremos eventos de teclado o raton del usuario,
actualizaremos los parametros de los sprites y objetos y renderizaremos por pantalla los mismos*/
#pragma once
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include "proyectil.h"
#include "enemigo.h"
#pragma warning (disable : 4996)

#define MENU 0
#define JUEGO 1
#define GANAR 2
#define PERDER 3

#define INITIAL_FOV 60
#define INCREMENTO_FOV 0.3
#define MOUSE_SPEED 0.1

class juego{

public:

	//atributos
	unsigned int estado;    //para el estado del juego (rendierizar diferentes cosas) 0-->menu incial, 1-->juego, 2-->ganar, 3 -->perder
	unsigned int ANCHO, ALTO;
	GLuint shaderProgram;
	GLuint shaderCubeMap;
	GLboolean Keys[1024];
	GLboolean click;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	std::vector<proyectil * > proyectiles_jugador;  //vector de proyectiles del jugador
	std::vector<enemigo *> enemigos;


	// Constructor/Destructor
	juego(unsigned int ANCHO, unsigned int ALTO);
	~juego();
	
	//gettes y setters
	void setShaderProgram(GLuint sh);
	void setShaderCubeMap(GLuint sh);

	//metodos publicos
	void Init();	 //inicializar componentes (fuera del bucle)
	void ProcessInput(float deltaTime, GLFWwindow* window);  //gestion de eventos. Se hace desde el main
	void Update(float deltaTime, GLFWwindow* window); //actualizacion de los parametros de los objetos
	void Render();			//renderizado de los objetos

	void cargarOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals);
	GLuint cargarTextura(char * path);
	GLuint loadCubemap(std::vector<std::string> faces);
	//void loadTerrainHeight(char * path, std::vector<GLubyte> * altura, int * width, int * height);

private:
	//metodos
	void Camara();
	void CamaraInicio();
	void generarCuboBox();
	void generarCuadrado();
	void Suelo();
	
};

