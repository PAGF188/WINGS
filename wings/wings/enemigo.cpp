#include "enemigo.h"
int rota = 1.0;
enemigo::enemigo(glm::vec3 pos, glm::vec3 direction, float velocidad, float anguloX, float anguloY, float anguloZ, float vRotacion)
{
	this->pos = pos;
	this->direction = direction;
	this->velocidad = velocidad;
	this->anguloX = anguloX;
	this->anguloY = anguloY;
	this->anguloZ = anguloZ;
	this->vRotacion = vRotacion;
	this->vida = 4;
}


enemigo::~enemigo()
{
}

void enemigo::set(glm::vec3 pos, glm::vec3 direction, float velocidad, float anguloX, float anguloY, float anguloZ, float vRotacion) {
	this->pos = pos;
	this->direction = direction;
	this->velocidad = velocidad;
	this->anguloX = anguloX;
	this->anguloZ = anguloZ;
	this->vRotacion = vRotacion;
}
void enemigo::setPos(glm::vec3 pos) {
	this->pos = pos;
}
void enemigo::setDirection(glm::vec3 dir) {
	this->direction = dir;
}
void enemigo::setVelocidad(float v) {
	this->velocidad = v;
}
void enemigo::setAnguloX(float anguloX) {
	this->anguloX = anguloX;
}
void enemigo::setAnguloY(float anguloY) {
	this->anguloY = anguloY;
}
void enemigo::setAnguloZ(float anguloZ) {
	this->anguloZ = anguloZ;
}
void enemigo::setVrotacion(float vRotacion) {
	this->vRotacion = vRotacion;
}
void enemigo::setTextura(GLuint textura) {
	this->textura = textura;
}
void enemigo::decrementarVida(int a) {
	this->vida = this->vida - a;
}
void enemigo::actualizar(float deltaTime, Jugador jugador,glm::vec3 direction) {

	this->pos = this->pos + this->direction * this->velocidad*deltaTime;
	if (this->pos[1] <= jugador.getY() + 0.1 && this->pos[1] >= jugador.getY() - 0.1) {
		this->velocidad = jugador.getvMovimiento();
		this->direction = glm::normalize(direction);
	}

	//movimientos pseudoaleatorios rotacion eje z
	this->anguloZ += (rota * deltaTime)*0.2;
	if (this->anguloZ >= glm::radians(15.0f)) {
		rota = -rota;
		//this->pos = this->pos - glm::vec3(40.0f, 0.0f, 0.0f)*glm::vec3(deltaTime, deltaTime, deltaTime);
	}
	if (this->anguloZ <= glm::radians(-15.0f)) {
		rota = -rota;
		//this->pos = this->pos + glm::vec3(40.0f, 0.0f, 0.0f)*glm::vec3(deltaTime, deltaTime, deltaTime);
	}
	//cambion en vector director

	
}

void enemigo::draw(GLuint shader) {
	Sphere esfera;
	unsigned int transformLocK = glGetUniformLocation(shader, "transform");
	glm::mat4 transform;
	glm::mat4 temp;

	//ponemos activo el VAO del cuadrado
	glBindVertexArray(this->VAO);
	glBindTexture(GL_TEXTURE_2D, this->textura);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	transform = glm::mat4();
	transform = glm::translate(transform, pos);
	transform = glm::rotate(transform, this->anguloZ, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, this->anguloX, glm::vec3(-1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, this->anguloY, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::scale(transform,glm::vec3(1.5f, 1.5f, 1.5f));

	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));  //esto se usa para cargar la matriz en el shader
	glDrawArrays(GL_TRIANGLES, 0, 12927);
	glBindVertexArray(0);
	//resetamos matriz transofrm a identidad
	transform = glm::mat4();
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, 0);
}

void enemigo::generarVAO(std::vector< glm::vec3 > vertices, std::vector< glm::vec3 > normales, std::vector< glm::vec2 > uv) {
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
	glBufferData(GL_ARRAY_BUFFER, normales.size() * sizeof(glm::vec3), &normales[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	//uv
	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &TBO);

	this->VAO = VAO;

}
