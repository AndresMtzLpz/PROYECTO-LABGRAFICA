#include "Movimiento.h"
#include <glfw3.h>
#define PI 3.14159265

Movimiento::Movimiento()
{
	deltaTime = 0.0f;
	lastTime = 0.0f;
	toRadians = 3.14159265f / 180.0f;
	/*Aqui se declaran las posiciones iniciales para cada objeto
	Para que se visualize correctamente desde el t=0 verificar que
	la posición dada esté dentro del recorrido y entre en la primera condcional*/
	posX_BH = 0.0f;
	posY_BH = 10.0f;
	posZ_BH = 5.0f;
	giroY_BH = 0.0f;
	ruta_BH = 0;
	dirY_BH = 1;
	giroHelice = 0.0;

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

	contHora = 0.0f;
	cont = 0.0f;

	posX_luzSP = 0.0f;
	posZ_luzSP = 0.0f;
	DirX_luzSP = 1;
}

glm::vec3 Movimiento::movBlackHawk(float movOffset, bool despegue_BH)
{

	if (despegue_BH) {
		giroHelice += 15.0f * deltaTime;


		if (giroHelice == 200.0f || posY_BH < 20.0f) {
			posY_BH += 0.6f * deltaTime;
		}
		else {

			switch (ruta_BH)
			{
			case 1:
				if (posZ_BH < 5) {
					posZ_BH += sin(0.3) * 2.5 * deltaTime;
					posY_BH += sin(1.5) * deltaTime;
				}
				else if (posZ_BH >= 5 && giroY_BH < 90) {
					giroY_BH += 1.0;
				}
				else
					ruta_BH += 1;
				break;

			case 2:
				if (posX_BH > -5) {
					posX_BH -= sin(0.7) * 2.5 * deltaTime;
					posY_BH += sin(giroHelice) * deltaTime;
				}
				else if (posX_BH <= -5 && giroY_BH < 180) {
					giroY_BH += 1.0;
				}
				else {
					ruta_BH += 1;

				}

				break;
			case 3:
				if (posZ_BH > -5) {
					posZ_BH -= sin(0.4) * 2.5 * deltaTime;
					posY_BH += sin(giroHelice) * deltaTime;
				}
				else if (posZ_BH <= 5 && giroY_BH < 270) {
					giroY_BH += 1.0;
				}
				else
					ruta_BH += 1;
				break;
			case 4:
				if (posX_BH < 5) {
					posX_BH += sin(0.3) * 2.5 * deltaTime;
					posY_BH += sin(giroHelice) * deltaTime;
				}
				else if (posX_BH >= 5 && giroY_BH < 360) {
					giroY_BH += 1.0;
				}
				else
					ruta_BH += 1;
				break;
			default:
				ruta_BH = 1;
				giroY_BH = 0;

			}
		}
	}

	return glm::vec3(posX_BH - 1.85,posY_BH,posZ_BH*2+2);
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

float Movimiento::getGiroHelice() {
	return giroHelice * toRadians;
}

float Movimiento::giroKitY()
{
	return giroY_KK * toRadians;
}

float Movimiento::giroKitZ()
{
	return giroZ_KK * toRadians;
}

glm::vec3 Movimiento::getMovCir()
{	
	float tetha = 2 * PI * cont;
	float x = cos(tetha);
	float z = sin(tetha);
	//printf("Theta %f \n",);
	return glm::vec3(x, 1.0f,z); //X & Z pequeños para circulo pequeño, Y aumenta el tamaño, pero no inf a 1
}

glm::vec3 Movimiento::getMovLin()
{
	if (abs(posX_luzSP) >= 4) {
		DirX_luzSP *= -1;
		posX_luzSP += deltaTime * 3.0 * DirX_luzSP; //Evita rebotes
	}
	posX_luzSP += deltaTime * 2.0 * DirX_luzSP;
	posZ_luzSP = posX_luzSP / 2;
	return glm::vec3(posX_luzSP,4, posZ_luzSP);
}

glm::vec3 Movimiento::getMovLin_2()
{
	return glm::vec3(-posX_luzSP, 4, -posZ_luzSP);
}

float Movimiento::giro(float p_ini, float p_final, float p_actual)
{
	float mag_giro = p_final - p_ini;
	return (p_actual-p_ini)/mag_giro;
}

float Movimiento::horaDia() {
	contHora += deltaTime * 40;
	//printf("%d \n\n", (int)contHora % 24);
	return (int)contHora % 2400;
}


GLfloat Movimiento::time()
{
	GLfloat now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;
	cont += getDeltaTime() * 0.5;
	return GLfloat();

}

float Movimiento::getDeltaTime() {
	return deltaTime;
}

Movimiento::~Movimiento()
{
}


