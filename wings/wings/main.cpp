/*VIDEOJUEGO WINGS
AUTOR: Pablo Garcia Fernnadez
*/
/*NOTAS DE DISEÑO
-1m equivalente a 1 float
-hecho en plano X-Z
*/

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
GLuint shaderCubeMap;
GLFWwindow* window;


void framebuffer_size_callback(GLFWwindow  *window, int ancho, int alto) {
	Wings.ANCHO = ancho;
	Wings.ALTO = alto;
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
	
	Wings.Init();

	shaderCubeMap = setShaders("shaderMap.vert", "shaderMap.frag");
	Wings.setShaderCubeMap(shaderCubeMap);

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

		Wings.Update(deltaTime, window);

		Wings.Render();

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


