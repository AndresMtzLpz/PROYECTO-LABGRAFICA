#pragma once
#define MAX_FRAMES 30
#include <glm.hpp>
#include <glew.h>
#include <glfw3.h>

class AnimacionKF
{

public:
	AnimacionKF();
	void saveFrame(void);
	void resetElements(void);
	void interpolation(void);
	void animate(void);
	
	void animaAvion();

	glm::vec3 movAvion();
	float rGiroAvionX();
	float rGiroAvionZ();
	float rVueltaAvion();
	void inputKeyframes(bool* keys);
	~AnimacionKF();

private:

	float posXavion, posYavion , posZavion;
	float movAvion_x, movAvion_y , movAvion_z;
	float giroAvionX, giroAvionZ;
	float vueltaAvion;

	float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador;

	int i_max_steps;
	int i_curr_steps;

	int FrameIndex;			//introducir datos
	bool play;
	int playIndex;

	bool playDespegue;
	bool animacion;

	typedef struct _frame
	{
		//Variables para GUARDAR Key Frames
		float movAvion_x;		//Variable para PosicionX
		float movAvion_y;		//Variable para PosicionY
		float movAvion_z;		//Variable para PosicionZ
		float movAvion_xInc;		//Variable para IncrementoX
		float movAvion_yInc;		//Variable para IncrementoY
		float movAvion_zInc;		//Variable para IncrementoZ
		float giroAvionX;
		float giroAvionXInc;
		float giroAvionZ;
		float giroAvionZInc;
		float vueltaAvion;
		float vueltaAvionInc;

	}FRAME;

	FRAME KeyFrame[MAX_FRAMES];

};