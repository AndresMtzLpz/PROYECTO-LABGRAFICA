#pragma once
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Luminaria
{
public:
	Luminaria();
	Luminaria(PointLight* pointlight, SpotLight* spotlight);
	DirectionalLight* getDirectional();
	int getPointCount();
	int getSpotCount();
	void setLuminaria(int horaDia, bool botonK);
	~Luminaria();

private:
	void creaPointLights();
	void creaSpotLights();
	PointLight* pointLight;
	SpotLight* spotLight;

	/*Delcarar todas las luces a usar*/
	DirectionalLight sunLight;
	DirectionalLight moonLight;
	
	PointLight PLFaro_1;
	PointLight PLFaro_2;
	PointLight PLFaro_3;
	PointLight PLFaro_4;
	PointLight PLFaro_5;
	PointLight PLFaro_6;
	PointLight PLFaro_7;
	PointLight PLFaro_8;
	PointLight PLFaro_9;

	SpotLight SPLinterna;
	SpotLight SPPistaCir;
	SpotLight SPPistaLin_1;
	SpotLight SPPistaLin_2;

	
	int pointLightCount;
	int spotLightCount;
	int hora;
	int minuto;
};