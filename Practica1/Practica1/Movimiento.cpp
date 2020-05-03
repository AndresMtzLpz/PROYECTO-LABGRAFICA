#include "Movimiento.h"
#include <glfw3.h>

Movimiento::Movimiento()
{
	deltaTime = 0.0f;
	lastTime = 0.0f;
	toRadians = 3.14159265f / 180.0f;
	/*Aqui se declaran las posiciones iniciales para cada objeto
	Para que se visualize correctamente desde el t=0 verificar que
	la posición dada esté dentro del recorrido y entre en la primera condcional*/
	posX_BH = 0.0f;
	posY_BH = 0.0f;
	posZ_BH = 5.0f;
	giroY_BH = 0.0f;
	ruta_BH = 0;
	dirY_BH = 1;

	posX_KK = -10.0f;
	posY_KK = 0.0f;
	posZ_KK = 3.0f;
	giroZ_KK = 0.0f;
	giroY_KK = 0.0f;
	ruta_KK = 0;

	posX_BB = 0.0f;
	posY_BB = 55.1f;
	posZ_BB = 0.0f;
	maxY_BB = 55.0f;
	ruta_BB = -1;
	t_BB = 0;
}

glm::vec3 Movimiento::movBlackHawk(float movOffset)
{
	float x;

	switch (ruta_BH)
	{
	case 0:
		ruta_BH = (posX_BH > 4.0f) ? 1 : 0;
		posX_BH += movOffset * deltaTime;
		giroY_BH = 0;
		break;

	case 1:
		ruta_BH = (posX_BH >= 9.0f) ? 2 : 1;
		x = posX_BH - 4.0;
		posZ_BH = sqrt(25 - x * x);
		posX_BH += movOffset * deltaTime / 5;
		giroY_BH = 90 * giro(4.0, 9.0, posX_BH);
		break;

	case 2:
		ruta_BH = (posX_BH <= 4.0f) ? 3 : 2;
		posX_BH -= movOffset * deltaTime / 5;
		x = posX_BH - 4.0;
		posZ_BH = -sqrt(25 - x * x);
		giroY_BH = 90 + 90 * giro(9.0, 4.0, posX_BH);
		break;

	case 3:
		ruta_BH = (posX_BH < -4.0f) ? 4 : 3;
		posX_BH -= movOffset * deltaTime;
		giroY_BH = 180;
		break;

	case 4:
		ruta_BH = (posX_BH < -9.0f) ? 5 : 4;
		x = posX_BH + 4.0;
		posZ_BH = -sqrt(25 - x * x);
		posX_BH -= movOffset * deltaTime /5;
		giroY_BH = 180 + 90 * giro(4.0, 9.0, -posX_BH);
		break;
	
	case 5:
		ruta_BH = (posX_BH > -4) ? 0 : 5;
		posX_BH += movOffset * deltaTime/5;
		x = posX_BH + 4.0;
		posZ_BH = sqrt(25 - x * x);
		giroY_BH = 270 + 90 * giro(9.0, 4.0, -posX_BH);
		break;

	}
	dirY_BH += deltaTime;
	posY_BH = 2*sin(dirY_BH) + 0.5;
	return glm::vec3(posX_BH,posY_BH,posZ_BH);
}

glm::vec3 Movimiento::movKit(float movOffset)
{
	int x;
	switch (ruta_KK)
	{
	case 0:
		ruta_KK = (posX_KK > 10.0f) ? 2 : 0;
		posX_KK += movOffset * deltaTime;
		posY_KK = posX_KK >= -2.0f && posX_KK <= 0.0f ? (posX_KK + 2)/2 : posY_KK;
		posY_KK = posX_KK >= 0.0f && posX_KK <= 2.0f ? 1 + (posX_KK) / 2 : posY_KK;
		giroY_KK = 0.0;
		break;

	case 2:
		ruta_KK = (posX_KK >= 11.0f) ? 3 : 2;
		posX_KK += movOffset * deltaTime / 5;
		x = posX_KK - 10.0;
		posZ_KK = sqrt(1 - x * x) + 2.0;
		giroY_KK = 90 * giro(10, 11, posX_KK);
		break;

	case 3:
		ruta_KK = (posZ_KK < -2.0f) ? 4 : 3;
		posZ_KK -= movOffset * deltaTime;
		giroY_KK = 90;
		break;

	case 4:
		ruta_KK = (posX_KK <= 10.0f) ? 5 : 4;
		posX_KK -= movOffset * deltaTime / 5;
		x = posX_KK - 10.0;
		posZ_KK = -sqrt(abs(1 - x * x)) - 2;
		giroY_KK = 90 + 90 * giro(11.0, 10.0, posX_KK);
		break;

	case 5:
		ruta_KK = (posX_KK < -10) ? 6 : 5;
		posX_KK -= movOffset * deltaTime ;
		posY_KK = posX_KK <= 2.0f && posX_KK >= 0.0f ? (posX_KK + 2) / 2 : posY_KK;
		posY_KK = posX_KK <= 0.0f && posX_KK >= -2.0f ? (posX_KK +2) / 2 : posY_KK;
		giroY_KK = 180;
		break;

	case 6:
		ruta_KK = (posX_KK <= -11) ? 7 : 6; //Se pasó del 11, el otro regresó a 2
		posX_KK -= movOffset * deltaTime / 5;
		x = posX_KK + 10.0;
		posZ_KK = -sqrt(abs(1 - x * x)) - 2;
		giroY_KK = 180 + 90 * giro(10, 11, -posX_KK);
		//giroZ_KK = 270 + 90 * giro(9.0, 4.0, -posX_KK);
		break;
	case 7:
		ruta_KK = (posZ_KK >= 2.0f) ? 8 : 7;
		posZ_KK += movOffset * deltaTime;
		giroY_KK = 270;
		break;
	case 8:
		ruta_KK = (posX_KK >= -10.0f) ? 0 : 8;
		posX_KK += movOffset * deltaTime / 5;
		x = posX_KK + 10.0;
		posZ_KK = sqrt(abs(1 - x * x)) + 2;
		giroY_KK = 360 - 90 * giro(10, 11, -posX_KK);
		break;
		

	}
	return glm::vec3(posX_KK, posY_KK -1.5, posZ_KK);
}

glm::vec3 Movimiento::movBall(float movOffset)
{	
	if (maxY_BB >= 0.001 )
	{
		if ((posY_BB > maxY_BB&& ruta_BB == 1) || (posY_BB < 0 && ruta_BB == -1))
		{
			ruta_BB *= -1;
			maxY_BB *= 0.7;
			t_BB = 0;
		}
		t_BB += movOffset * deltaTime;
		posY_BB += 0.5 * (9.81) * (t_BB * t_BB) * ruta_BB;
	}
	else
	{
		posY_BB = 0;
	}

	return glm::vec3(posX_BB,posY_BB - 1,posZ_BB);
}

float Movimiento::giroBlackHawk()
{
	return giroY_BH * toRadians;
}

float Movimiento::giroKitY()
{
	return giroY_KK * toRadians;
}

float Movimiento::giroKitZ()
{
	return giroZ_KK * toRadians;
}

float Movimiento::giro(float p_ini, float p_final, float p_actual)
{
	float mag_giro = p_final - p_ini;
	return (p_actual-p_ini)/mag_giro;
}


GLfloat Movimiento::time()
{
	GLfloat now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;
	return GLfloat();
}

Movimiento::~Movimiento()
{
}


