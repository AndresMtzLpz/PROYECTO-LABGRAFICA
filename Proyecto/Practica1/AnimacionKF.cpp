#include "AnimacionKF.h"
#include <stdio.h>
#include <glfw3.h>
#include "Movimiento.h"

AnimacionKF::AnimacionKF()
{
	contador = 0;

	movAvion_x = 0.0f; 
	movAvion_y = 0.0f;
	movAvion_z = 0.0f;
	giroAvionX = 0;
	giroAvionZ = 0;
	vueltaAvion = 0;

	i_max_steps = 90;
	play = false;
	playIndex = 0;

	playDespegue = false;
	animacion = false;

	/*Iniciaización de arreglos individuales*/
	animaAvion();
}

void AnimacionKF::saveFrame(void)
{

	printf("frameindex %d: ", FrameIndex);



	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	KeyFrame[FrameIndex].giroAvionX;
	KeyFrame[FrameIndex].giroAvionZ;
	KeyFrame[FrameIndex].vueltaAvion;

	FrameIndex++;
}

void AnimacionKF::resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvionX = KeyFrame[0].giroAvionZ;
	giroAvionZ = KeyFrame[0].giroAvionX;
	vueltaAvion = KeyFrame[0].vueltaAvion;
}

void AnimacionKF::interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionXInc = (KeyFrame[playIndex + 1].giroAvionX - KeyFrame[playIndex].giroAvionX) / i_max_steps;
	KeyFrame[playIndex].giroAvionZInc = (KeyFrame[playIndex + 1].giroAvionZ - KeyFrame[playIndex].giroAvionZ) / i_max_steps;
	KeyFrame[playIndex].vueltaAvionInc = (KeyFrame[playIndex + 1].vueltaAvion - KeyFrame[playIndex].vueltaAvion) / i_max_steps;
}


void AnimacionKF::animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			printf("movAvion_x=%f ,movAvion_y=%f,,movAvion_z=%f ,giroAvion= %f\n", KeyFrame[playIndex].movAvion_x, KeyFrame[playIndex].movAvion_y, KeyFrame[playIndex].movAvion_z, KeyFrame[playIndex].giroAvionX , KeyFrame[playIndex].giroAvionZ, KeyFrame[playIndex].vueltaAvion);
			printf("\n");
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroAvionX += KeyFrame[playIndex].giroAvionXInc;
			giroAvionZ += KeyFrame[playIndex].giroAvionZInc;
			vueltaAvion += KeyFrame[playIndex].vueltaAvionInc;
			i_curr_steps++;
		}

	}

	//Movimiento del Helicoptero Despegue
	if (playDespegue)
	{


	}


}

void AnimacionKF::animaAvion() {
	posXavion = 80.0;
	posYavion = 30.0;
	posZavion = 100.0;

	i_curr_steps = 29;
	FrameIndex = 29;

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].movAvion_z = 0.0f;
	KeyFrame[0].giroAvionX = 0;
	KeyFrame[0].giroAvionZ = 0;
	KeyFrame[0].vueltaAvion = 0;


	KeyFrame[1].movAvion_x = 0.0f;
	KeyFrame[1].movAvion_y = 0.0f;
	KeyFrame[1].movAvion_z = -145.0f;
	KeyFrame[1].giroAvionX = 0;
	KeyFrame[1].giroAvionZ = 360;
	KeyFrame[1].vueltaAvion = 0;


	KeyFrame[2].movAvion_x = 0.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].movAvion_z = -175.0f;
	KeyFrame[2].giroAvionX = 0;
	KeyFrame[2].giroAvionZ = 360;
	KeyFrame[2].vueltaAvion = 90;

	KeyFrame[3].movAvion_x = -150.0f;
	KeyFrame[3].movAvion_y = 0.0f;
	KeyFrame[3].movAvion_z = -175.0f;
	KeyFrame[3].giroAvionX = 360;
	KeyFrame[3].giroAvionZ = 360;
	KeyFrame[3].vueltaAvion = 90;

	KeyFrame[4].movAvion_x = -175.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].movAvion_z = -175.0f;
	KeyFrame[4].giroAvionX = 360;
	KeyFrame[4].giroAvionZ = 360;
	KeyFrame[4].vueltaAvion = 180;

	KeyFrame[5].movAvion_x = -175.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].movAvion_z = -25.0f;
	KeyFrame[5].giroAvionX = 360;
	KeyFrame[5].giroAvionZ = 0;
	KeyFrame[5].vueltaAvion = 180;

	KeyFrame[6].movAvion_x = -175.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].movAvion_z = 0.0f;
	KeyFrame[6].giroAvionX = 360;
	KeyFrame[6].giroAvionZ = 0;
	KeyFrame[6].vueltaAvion = 270;

	KeyFrame[7].movAvion_x = -25.0f;
	KeyFrame[7].movAvion_y = 0.0f;
	KeyFrame[7].movAvion_z = 0.0f;
	KeyFrame[7].giroAvionX = 0;
	KeyFrame[7].giroAvionZ = 0;
	KeyFrame[7].vueltaAvion = 270;

	KeyFrame[8].movAvion_x = 0.0f;
	KeyFrame[8].movAvion_y = 0.0f;
	KeyFrame[8].movAvion_z = 0.0f;
	KeyFrame[8].giroAvionX = 0;
	KeyFrame[8].giroAvionZ = 0;
	KeyFrame[8].vueltaAvion = 360;
}

glm::vec3 AnimacionKF::movAvion() {
	return glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion + movAvion_z);
}
float AnimacionKF::rGiroAvionX() {
	return giroAvionX;
}
float AnimacionKF::rGiroAvionZ() {
	return giroAvionZ;
}
float AnimacionKF::rVueltaAvion() {
	return vueltaAvion;
}

void AnimacionKF::inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}

AnimacionKF::~AnimacionKF()
{
}
