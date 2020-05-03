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
	giroAvion = 0;

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
	KeyFrame[FrameIndex].giroAvion;

	FrameIndex++;
}

void AnimacionKF::resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroAvion = KeyFrame[0].giroAvion;
}

void AnimacionKF::interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

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
			printf("movAvion_x=%f ,movAvion_y=%f,,movAvion_z=%f ,giroAvion= %f\n", KeyFrame[playIndex].movAvion_x, KeyFrame[playIndex].movAvion_y, KeyFrame[playIndex].movAvion_z, KeyFrame[playIndex].giroAvion);
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
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}

	//Movimiento del Helicoptero Despegue
	if (playDespegue)
	{


	}


}

void AnimacionKF::animaAvion() {
	posXavion = 2.0;
	posYavion = 2.0;
	posZavion = 0.0;

	i_curr_steps = 29;
	FrameIndex = 29;

	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].movAvion_z = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].movAvion_z = 0.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].movAvion_z = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].movAvion_z = 0.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].movAvion_z = 0.0f;
	KeyFrame[4].giroAvion = 0.0f;

	KeyFrame[5].movAvion_x = 5.0f;
	KeyFrame[5].movAvion_y = 2.0f;
	KeyFrame[5].movAvion_z = 0.0f;
	KeyFrame[5].giroAvion = 0;

	KeyFrame[6].movAvion_x = 6.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].movAvion_z = 0.0f;
	KeyFrame[6].giroAvion = 0;

	KeyFrame[7].movAvion_x = 7.0f;
	KeyFrame[7].movAvion_y = -2.0f;
	KeyFrame[7].movAvion_z = 0.0f;
	KeyFrame[7].giroAvion = 0;

	KeyFrame[8].movAvion_x = 8.0f;
	KeyFrame[8].movAvion_y = 0.0f;
	KeyFrame[8].movAvion_z = 0.0f;
	KeyFrame[8].giroAvion = 0;

	KeyFrame[9].movAvion_x = 9.0f;
	KeyFrame[9].movAvion_y = 2.0f;
	KeyFrame[9].movAvion_z = 0.0f;
	KeyFrame[9].giroAvion = 0.0f;

	KeyFrame[10].movAvion_x = 10.0f;
	KeyFrame[10].movAvion_y = 0.0f;
	KeyFrame[10].movAvion_z = 0.0f;
	KeyFrame[10].giroAvion = 0;

	KeyFrame[11].movAvion_x = 10.0f;
	KeyFrame[11].movAvion_y = 0.0f;
	KeyFrame[11].movAvion_z = -1.0f;
	KeyFrame[11].giroAvion = 45;

	KeyFrame[12].movAvion_x = 10.0f;
	KeyFrame[12].movAvion_y = 0.0f;
	KeyFrame[12].movAvion_z = -2.0f;
	KeyFrame[12].giroAvion = 90;

	KeyFrame[13].movAvion_x = 10.0f;
	KeyFrame[13].movAvion_y = 0.0f;
	KeyFrame[13].movAvion_z = -3.0f;
	KeyFrame[13].giroAvion = 135;

	KeyFrame[14].movAvion_x = 10.0f;
	KeyFrame[14].movAvion_y = 0.0f;
	KeyFrame[14].movAvion_z = -4.0f;
	KeyFrame[14].giroAvion = 180.0;

	KeyFrame[15].movAvion_x = 9.0f;
	KeyFrame[15].movAvion_y = 2.0f;
	KeyFrame[15].movAvion_z = -4.0f;
	KeyFrame[15].giroAvion = 180.0;

	KeyFrame[16].movAvion_x = 8.0f;
	KeyFrame[16].movAvion_y = 0.0f;
	KeyFrame[16].movAvion_z = -4.0f;
	KeyFrame[16].giroAvion = 180;

	KeyFrame[17].movAvion_x = 7.0f;
	KeyFrame[17].movAvion_y = -2.0f;
	KeyFrame[17].movAvion_z = -4.0f;
	KeyFrame[17].giroAvion = 180;

	KeyFrame[18].movAvion_x = 6.0f;
	KeyFrame[18].movAvion_y = 0.0f;
	KeyFrame[18].movAvion_z = -4.0f;
	KeyFrame[18].giroAvion = 180;

	KeyFrame[19].movAvion_x = 5.0f;
	KeyFrame[19].movAvion_y = 2.0f;
	KeyFrame[19].movAvion_z = -4.0f;
	KeyFrame[19].giroAvion = 180.0f;

	KeyFrame[20].movAvion_x = 4.0f;
	KeyFrame[20].movAvion_y = 0.0f;
	KeyFrame[20].movAvion_z = -4.0f;
	KeyFrame[20].giroAvion = 180;

	KeyFrame[21].movAvion_x = 3.0f;
	KeyFrame[21].movAvion_y = -2.0f;
	KeyFrame[21].movAvion_z = -4.0f;
	KeyFrame[21].giroAvion = 180;

	KeyFrame[22].movAvion_x = 2.0f;
	KeyFrame[22].movAvion_y = 0.0f;
	KeyFrame[22].movAvion_z = -4.0f;
	KeyFrame[22].giroAvion = 180.0;

	KeyFrame[23].movAvion_x = 1.0f;
	KeyFrame[23].movAvion_y = 2.0f;
	KeyFrame[23].movAvion_z = -4.0f;
	KeyFrame[23].giroAvion = 180.0;

	KeyFrame[24].movAvion_x = 0.0f;
	KeyFrame[24].movAvion_y = 0.0f;
	KeyFrame[24].movAvion_z = -4.0f;
	KeyFrame[24].giroAvion = 180;

	KeyFrame[25].movAvion_x = 0.0f;
	KeyFrame[25].movAvion_y = 0.0f;
	KeyFrame[25].movAvion_z = -3.0f;
	KeyFrame[25].giroAvion = 225;

	KeyFrame[26].movAvion_x = 0.0f;
	KeyFrame[26].movAvion_y = 0.0f;
	KeyFrame[26].movAvion_z = -2.0f;
	KeyFrame[26].giroAvion = 270;

	KeyFrame[27].movAvion_x = 0.0f;
	KeyFrame[27].movAvion_y = 0.0f;
	KeyFrame[27].movAvion_z = -1.0f;
	KeyFrame[27].giroAvion = 315.0f;

	KeyFrame[28].movAvion_x = 0.0f;
	KeyFrame[28].movAvion_y = 0.0f;
	KeyFrame[28].movAvion_z = 0.0f;
	KeyFrame[28].giroAvion = 360.0f;
}

glm::vec3 AnimacionKF::movAvion() {
	return glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion + movAvion_z);
}
float AnimacionKF::rGiroAvion() {
	return giroAvion;
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
