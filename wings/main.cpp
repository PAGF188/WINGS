/*VIDEOJUEGO WINGS
AUTOR: Pablo Garcia Fernnadez
*/
/*NOTAS DE DISEÑO
-1m equivalente a 1 float
-hecho en plano X-Z
*/
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
//Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "juego.h"

//TAMAÑO VENTANA
int ANCHO = 768;
int ALTO = 768;

//varaibles para el calculo de deltaTime
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//funciones
void framebuffer_size_callback(GLFWwindow  *window, int width, int height);
extern GLuint setShaders(const char *nVertx, const char *nFrag);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

juego Wings(ANCHO, ALTO);
GLuint shaderProgram;
GLFWwindow* window;


void framebuffer_size_callback(GLFWwindow  *window, int ancho, int alto) {
	Wings.ANCHO = ancho;
	Wings.ALTO = alto;
}

//borra esta fucion
GLuint VAO;
void Suelo() {
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


int main() {

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creo la ventana											
	window = glfwCreateWindow(ANCHO, ALTO, "WINGS", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//borrar
	Suelo();
	Wings.Init();

	//generarShader();
	shaderProgram = setShaders("shader.vert", "shader.frag");
	glUseProgram(shaderProgram);
	Wings.setShaderProgram(shaderProgram);

	
	// Lazo de la ventana mientras no la cierre
	while (!glfwWindowShouldClose(window))
	{
		//calculamos deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Wings.ProcessInput(deltaTime,window);

		Wings.Update(deltaTime);

		Wings.Render();

		//borrar de aqui
		glm::mat4 transform;
		unsigned int transformLocK = glGetUniformLocation(shaderProgram, "transform");
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(VAO);
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

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//control de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Wings.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Wings.Keys[key] = GL_FALSE;
	}
}

//control de raton
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			Wings.click = GL_TRUE;
		}
	}		
}


