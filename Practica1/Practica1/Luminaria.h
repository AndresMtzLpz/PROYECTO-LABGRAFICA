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
	void setLuminaria(int horaDia);
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

	SpotLight SPLinterna;
	SpotLight SPPista;

	
	int pointLightCount;
	int spotLightCount;
	int hora;
	int minuto;
};