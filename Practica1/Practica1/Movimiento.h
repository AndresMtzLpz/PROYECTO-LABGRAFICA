#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>

class Movimiento
{
public:
	Movimiento();
	glm::vec3 movBlackHawk(float movOffset);
	glm::vec3 movKit(float movOffset);
	glm::vec3 movBall(float movOffset);
	float giroBlackHawk();
	float giroKitY();
	float giroKitZ();
	GLfloat  time();
	~Movimiento();

private:
	GLfloat deltaTime;
	GLfloat lastTime;
	float toRadians;

	float giro(float p_ini, float p_final, float p_actual);

	//Posicion de BH
	float posX_BH;
	float posY_BH;
	float posZ_BH;
	float dirY_BH;
	float giroY_BH;
	int	  ruta_BH;

	float posX_KK;
	float posY_KK;
	float posZ_KK;
	float giroZ_KK;
	float giroY_KK;
	int	  ruta_KK;

	float posX_BB;
	float posY_BB;
	float posZ_BB;
	int	  ruta_BB;
	float maxY_BB;
	float t_BB;
};