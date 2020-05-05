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
	PointLight* pointLight;
	SpotLight* spotLight;
	PointLight* allPointLights;
	SpotLight* allSpotLights;

	/*Delcarar todas las luces a usar*/
	DirectionalLight sunLight;
	DirectionalLight moonLight;
	
	int pointLightCount;
	int spotLightCount;
	int hora;
	int minuto;
};