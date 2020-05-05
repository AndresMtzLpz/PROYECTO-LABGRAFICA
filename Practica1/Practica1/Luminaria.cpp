#include "Luminaria.h"
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include"SpotLight.h"

Luminaria::Luminaria()
{
}

Luminaria::Luminaria(PointLight * pointlight, SpotLight * spotlight)
{
	minuto = 0;
	hora = 0;
	pointLight = pointlight;
	spotLight = spotlight;
	allPointLights = (PointLight*) malloc(sizeof(PointLight)*3);
	allSpotLights = (SpotLight*)malloc(sizeof(PointLight) *1);

	*allSpotLights = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f, //Posicion de la luz
		0.0f, -1.0f, 0.0f, //Direccion de la luz
		1.0f, 0.0f, 0.0f,//Atenuacion
		20.0f);

	/* Definicion luces*/
	sunLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.8f, //Intensidad luminosa
		0.0f, 0.0f, -1.0f);

	moonLight = DirectionalLight(0.05f, 0.1f, 0.60f,
		0.3f, 0.5f, //Intensidad luminosa
		0.0f, 0.0f, -1.0f);






	/*Declaracion de todas las luces tipo PointLigth*/
	*allPointLights = PointLight(1.0f, 0.0f, 1.0f,
							0.0f, 8.0f, //Intensidades
							1.0f, 3.0f, 0.0f, //Posicion de la luz
							0.3f, 0.2f, 0.1f);	//coeficientes de una ecuacion de segundo grado
											//que no den valores complejos
	*(allPointLights +1) = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 8.0f, 
		-1.0f, 4.0f, 0.0f, 
		0.3f, 0.2f, 0.1f);

	*(allPointLights + 2) = PointLight(1.0f, 1.0f, 1.0f,
		0.0f, 10.0f,
		0.0f, 3.5f, 1.0f,
		0.3f, 0.2f, 0.1f);

}

DirectionalLight* Luminaria::getDirectional()
{
	if ((hora >= 600 && hora < 1800)) {
		return &sunLight;
	}
	return &moonLight;
}

int Luminaria::getPointCount()
{
	return pointLightCount;
}


int Luminaria::getSpotCount()
{
	return spotLightCount;
}

void Luminaria::setLuminaria(int horaDia)
{
	hora = horaDia;
	minuto = horaDia % 50 == 0 ? (minuto + 1) % 2: minuto ; //Cada 50 pasos (cada media hora) 1H =50 
	


	if ((hora >= 600 && hora < 1800)) {
		spotLightCount = 0;
		pointLightCount = 0;
	}
	else {
		*spotLight = *allSpotLights;
		for (int i = 0; i < 2; i++)
		{
			*(pointLight + i) = *(allPointLights + minuto+i);
		}
		spotLightCount = 1;
		pointLightCount = 2;
	}
}

Luminaria::~Luminaria()
{
	//free(allPointLights);
}