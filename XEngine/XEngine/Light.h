#include <windows.h>
#ifndef GL_H
#include <gl\GL.h>
#include <gl\GLU.h>
#endif // !1


#include <vector>
// Header file for Lighting used 
// Utility code Referenced from tutorials and developed over time
//This will be used as a container for our lights
using std::vector;

enum LIGHT_TYPE
{
	SPOT_LIGHT,
	POINT_LIGHT,
	DIRECTIONAL_LIGHT
};
class Light
{
public:


	Light(LIGHT_TYPE lightType);
	~Light();



	static void Init();
	void setDiffuse(float r, float g, float b, float a);
	void setAmbient(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setLightType(LIGHT_TYPE lightType);
	void setPosition(float x, float y, float z);
	void setSpotDirection(float x, float y, float z);
	void setCutoff(float value);
	void setShininess(float value);
	void setAttenuation(float constant, float linear, float quadratic);
	int getLightNum(void);
	void setExponent(float val);
	void updateLight();
	void EnableLight(bool value);
	enum LIGHT_TYPE lightType;
	static vector <int> availableLights;
	static vector <Light *> lights;
	static int numLights;
	bool visible;
private :

	GLfloat pos[4];
	GLfloat diffuse[4];
	GLfloat ambient[4];
	GLfloat specular[4];
	GLfloat spotDirection[4];
	float cutoff;
	float exp;
	
	
	int lightNum;
};