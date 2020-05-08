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

	/* Definicion luces*/
	sunLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.8f, //Intensidad luminosa
		0.0f, 0.0f, -1.0f);

	moonLight = DirectionalLight(0.05f, 0.1f, 0.60f,
		0.3f, 0.5f, //Intensidad luminosa
		0.0f, 0.0f, -1.0f);

	creaPointLights();
	creaSpotLights();

}

DirectionalLight* Luminaria::getDirectional()
{
	if (/*(hora >= 600 && hora < 1800)*/ false) {
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

void Luminaria::setLuminaria(int horaDia, bool botonK)
{
	hora = horaDia;
	//minuto = horaDia % 50 == 0 ? (minuto + 1) % 2: minuto ; //Cada 50 pasos (cada media hora) 1H =50 


	if (/*(hora >= 600 && hora < 1800)*/ false) {
		spotLightCount = 0;
		pointLightCount = 0;
	}
	else {
		switch (minuto)
		{
		case 0:
			*(pointLight + 1) = PLFaro_1;
			*(pointLight + 2) = PLFaro_2;
			*(pointLight + 0) = PLFaro_3;
			*(pointLight + 3) = PLFaro_4;
			*(pointLight + 4) = PLFaro_5;
			*(pointLight + 5) = PLFaro_6;
			*(pointLight + 6) = PLFaro_7;
			*(pointLight + 7) = PLFaro_8;
			pointLightCount = 8;
			break;
		case 1:
			*(pointLight + 1) = PLFaro_2;
			*(pointLight + 2) = PLFaro_3;
			*(pointLight + 0) = PLFaro_4;
			pointLightCount = 3;
			break;
		default:
			break;
		}
		/*La primera luz simpre debe de ser la linterna*/
		*spotLight = SPLinterna;
		*(spotLight + 1) = SPPistaCir;
		*(spotLight + 2) = SPPistaLin_1;
		*(spotLight + 3) = SPPistaLin_2;
		
		spotLightCount = botonK? 4 : 1;
	}
}

Luminaria::~Luminaria()
{
	//free(allPointLights);
}

void Luminaria::creaPointLights()
{
	/*Declaracion de todas las luces tipo PointLigth*/
	PLFaro_8 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		-7.5f, 4.2f, -66.0f,
		0.3f, 0.2f, 0.1f);
	PLFaro_7 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		7.5f, 4.2f, -48.0f,
		0.3f, 0.2f, 0.1f);
	PLFaro_6 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		-7.5f, 4.2f, 62.0f,
		0.3f, 0.2f, 0.1f);
	PLFaro_5 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f, //Intensidades
		-7.5f, 4.2f, -40.0f, //Posicion de la luz
		0.3f, 0.2f, 0.1f);	//coeficientes de una ecuacion de segundo grado, que no den valores complejos
	PLFaro_4 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		7.5f, 4.2f, 50.0f,
		0.3f, 0.2f, 0.1f);
	PLFaro_3 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		7.5f, 4.2f, 28.0f,
		0.3f, 0.2f, 0.1f);
	PLFaro_2 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		-7.5f, 4.2f, 42.0f,
		0.3f, 0.2f, 0.1f);
	PLFaro_1 = PointLight(1.0f, 1.0f, 0.9f,
		2.0f, 12.0f,
		7.5f, 4.2f, -26.0f,
		0.3f, 0.2f, 0.1f);
}

void Luminaria::creaSpotLights()
{
	SPPistaCir = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 10.0f,
		0.0f, 0.0f, 0.0f, //Posicion de la luz
		0.0f, -1.0f, 0.0f, //Direccion de la luz
		1.0f, 0.0f, 0.0f,//Atenuacion
		2.0f); //Tenia 1.5
	SPPistaLin_1 = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 10.0f,
		0.0f, 0.0f, 0.0f, //Posicion de la luz
		0.0f, -1.0f, 0.0f, //Direccion de la luz
		1.0f, 0.0f, 0.0f,//Atenuacion
		25.0f); //Baja atenuación implica PF pequeño Se ve más claro el circulo
	SPPistaLin_2 = SpotLight(1.0f, 0.0f, 0.0f,
		1.0f, 10.0f,
		0.0f, 0.0f, 0.0f, //Posicion de la luz
		0.0f, -1.0f, 0.0f, //Direccion de la luz
		1.0f, 0.0f, 0.0f,//Atenuacion
		25.0f);
	SPLinterna = SpotLight(0.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f, //Posicion de la luz
		0.0f, -1.0f, 0.0f, //Direccion de la luz
		1.0f, 0.0f, 0.0f,//Atenuacion
		20.0f);
}
