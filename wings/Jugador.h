#include <glad.h>
#include <glfw3.h>

#define INCREMENTO_VELOCIDAD 1
#define MAX_VELOCIDAD_MOVIMINETO 10
#define V_MOVIMINETO 2

class Jugador
{
public:
	//constructor y destructor
	Jugador(float x = 0.0f, float y = 3.0, float z = 0.0, float anguloX = 0.0, float anguloY = 0.0, float anguloZ = 0.0, float vRotacion = 3, float vMovimiento = V_MOVIMINETO);
	~Jugador();

	/********getters y setters*********/
	float getX() const { return x; }
	float getY() const { return y; }
	float getZ() const { return z; }
	float getAnguloX() const { return anguloX; }
	float getAnguloY() const { return anguloY; }
	float getAnguloZ() const { return anguloZ; }
	float getvRotacion() const { return vRotacion; }
	float getvMovimiento() const { return vMovimiento; }

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setAnguloX(float anguloX1);
	void setAnguloY(float anguloZ1);
	void setAnguloZ(float anguloX2);
	void setVrotacion(float vRotacion);
	void setVmovimiento(float vMovimineto);

	/******metodos publicos********/
	void generarVAO();
	void draw(GLuint shader);

private:
	/********atributos***********/
	//posicion
	float x;
	float y;
	float z;
	//rotaciones en los ejes
	float anguloX;
	float anguloY;
	float anguloZ;
	//velocidades
	float vRotacion;
	float vMovimiento;
	//VAO
	GLuint VAO;

	/*********metodos privados************/
	
};
