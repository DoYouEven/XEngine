#ifndef CAMERA_H
#define CAMERA_H
#ifndef VECTOR3_H
#include "glm.hpp"
#endif
#include "Transform.h"
#include "CharactorController.h"
class Skybox;
//#include "XEngine.h"


class Camera: Transform{

	float minRotX;
	float maxRotX;

	float minRotY;
	float maxRotY;


	float rotX;
	float rotY;
	float rotZ;

	float sensitivity;

	CharactorController *target;
	Skybox* skyBox;
	float movementSpeed;
	bool isInWindow;
	void clampCameraRotX();
	void clampCameraRotY();
	void clampCameraRot();
	void UpdateCameraPosition(float direction);
	void rotate(float rotX, float rotY);
	void moveCameraUp(float direction);


public :
	Camera(Skybox*_skyBox);

	Camera(vec3 pos);

	//Camera(vec3 pos, float rotY, float rotX);

	//Camera(vec3 pos, float rotY, float rotX, float rotZ);

	//vec3 getVector();
	float getRotX();
	float getRotY();
	float getRotZ();
	vec3 getRotation();

	void setClampX(float _minRotX, float _maxRotX);
	void setClampY(float _minRotY, float _maxRotY);
	void setSensitivity(float _sensitivity);
	void setMovementSpeed(float movementSpeed);
	vec3 getDirection();

	vec3 getPosition();

	vec3 Camera::getVector();

	void SetInWindow(bool in);
	bool IsInWindow();
	void Controller();
	void setTarget(CharactorController* target);
	void Update(CharactorController *target);

	void lookAt(CharactorController *target);
};
#endif