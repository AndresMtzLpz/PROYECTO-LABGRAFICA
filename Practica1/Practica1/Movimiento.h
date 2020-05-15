#pragma once

#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>

class Movimiento
{
public:
	Movimiento();
	glm::vec3 movBlackHawk(float movOffset, bool despegue_BH);
	glm::vec3 movKit(float movOffset);
	glm::vec3 movBall(float movOffset);
	glm::vec3 movBici(float movOffset, bool arranque_Bici);
	float giroBlackHawk();
	float getGiroHelice();
	float giroBici();
	float getGiroLlantas();
	float giroKitY();
	float giroSubeBaja();
	float giroKitZ();

	glm::vec3 getMovCir();
	glm::vec3 getMovLin();

	glm::vec3 getMovLin_2();

	float horaDia();
	GLfloat  time();
	float getDeltaTime();
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
	float subebaja_BH;
	int	  ruta_BH;
	float giroHelice;

	//Posicion de Bici
	float posX_Bici;
	float posY_Bici;
	float posZ_Bici;
	float dirY_Bici;
	float giroY_Bici;
	int	  ruta_Bici;
	float giroLlantas;


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

	float contHora;
	float cont;

	float posX_luzSP;
	float posZ_luzSP;
	int DirX_luzSP;

	float giroY_SB;
	int Dir_giroSB;

};