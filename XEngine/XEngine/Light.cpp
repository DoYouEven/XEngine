#include "Light.h"
vector <int> Light::availableLights;
vector <Light *> Light::lights;
int Light::numLights;


Light::Light(LIGHT_TYPE lightType)
{
	lights.push_back(this);
	if ((int)availableLights.size() > 0)
	{
		lightNum = availableLights[0];

		availableLights.erase(availableLights.begin());

		EnableLight(true);
		setSpotDirection(0, -1, 0);
		setExponent(12);
		setLightType(lightType);
		setAmbient(0, 0, 0, 1);
		setDiffuse(1, 1, 1, 1);
		setSpecular(1, 1, 1, 1);

		updateLight();
	}
}

Light::~Light()
{
	if (lightNum != 0)
	{
		availableLights.push_back(lightNum);
	}

	for (vector<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		if ((*it) == this)
			lights.erase(it);
	}
}

void Light::Init()
{
	for (int i = 0; i < 8; i++)
	{
		availableLights.push_back(GL_LIGHT0 + i);
	}
}
void Light::setDiffuse(float r, float g, float b, float a)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;


	glLightfv(lightNum, GL_DIFFUSE, diffuse);
}
void Light::setAmbient(float r, float g, float b, float a)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;


	glLightfv(lightNum, GL_AMBIENT, ambient);

}

void Light::setSpecular(float r, float g, float b, float a)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;


	glLightfv(lightNum, GL_SPECULAR, specular);
}

void Light::setLightType(LIGHT_TYPE _lightType)
{
	lightType = _lightType;
	switch (lightType)
	{
	case SPOT_LIGHT: pos[3] = 1.0f; break;
	case POINT_LIGHT: {
		pos[3] = 1.0f;
		setCutoff(180.0f);
		break; }
	case DIRECTIONAL_LIGHT: pos[3] = 1.0f; break;
	}
}
void Light::setPosition(float x, float y, float z)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	pos[3] = 1;
	glLightfv(lightNum, GL_POSITION, pos);
}
void Light::setSpotDirection(float x, float y, float z)
{
	spotDirection[0] = x;
	spotDirection[1] = y;
	spotDirection[2] = z;
	spotDirection[3] = 0.0f;
	glLightfv(lightNum, GL_SPOT_DIRECTION, spotDirection);
}
void Light::setCutoff(float val)
{
	cutoff = val;

	glLightf(lightNum, GL_SPOT_CUTOFF, cutoff);
}
void Light::setExponent(float val)
{
	exp = val;

	glLightf(lightNum, GL_SPOT_EXPONENT, exp);
}
void Light::setShininess(float value)
{
	return;
}
void Light::setAttenuation(float constant, float linear, float quadratic)
{
	glLightf(lightNum, GL_CONSTANT_ATTENUATION, constant);
	glLightf(lightNum, GL_LINEAR_ATTENUATION, linear);
	glLightf(lightNum,GL_QUADRATIC_ATTENUATION, quadratic);
}
int Light:: getLightNum()
{
	return lightNum;

}
void Light::updateLight()

{
	glLightfv(lightNum, GL_POSITION, pos);
glLightfv(lightNum, GL_SPOT_DIRECTION, spotDirection);
EnableLight(visible);
return;
}

void Light::EnableLight(bool value)

{
	visible = value;
	if (visible == true)
	{   
		glEnable(lightNum);

	}
	if (visible == false)
	{	
		glDisable(lightNum);
	
	}


}



