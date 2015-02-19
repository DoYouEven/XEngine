#include <Windows.h>
#include "Camera.h"
#include <SDL\SDL.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "Skybox.h"
#include"Utilities.h"
#define PI 3.14
#define toRad(angle) (angle * PI/180)
Camera::Camera(Skybox* _skybox)
{
	//rotX, rotY = 0;
	rotation.x, rotation.y = 0;
	float sensitivity = 0.2;
	float movementSpeed = 0.2 ;
	isInWindow = false;
	skyBox = _skybox;
}
/*
Camera::Camera(vec3 _pos, float rotY, float rotX)
{

	//
	rotation.x, rotation.y = 0;
	float sensitivity = 0.2;

	float movementSpeed = 0.2;
	isInWindow = false;
}
*/
Camera::Camera(vec3 pos)
{
	rotation.x, rotation.y ,rotation.z= 0;

}
void Camera::setSensitivity(float _sensitivity)
{
	sensitivity = _sensitivity;
}
void Camera::setMovementSpeed(float _movementSpeed)
{
	movementSpeed = _movementSpeed;
}
void Camera::setClampX(float _minRotX, float _maxRotX)
{
	minRotX = _minRotX;
	maxRotX = _maxRotX;
}
void Camera::setClampY(float _minRotY, float _maxRotY)
{
	minRotY = _minRotY;
	maxRotY = _maxRotY;
}
void Camera::clampCameraRotX()
{
	if (rotation.x > maxRotX)
		rotation.x = maxRotX;
	if (rotation.x < minRotX)
		rotation.x= minRotX;
}

void Camera::clampCameraRotY()
{
	if (rotation.y > maxRotY)
		rotation.y = maxRotY;
	if (rotation.y < minRotY)
		rotation.y = minRotY;
}
vec3 Camera::getDirection()
{
	return position;
}

vec3 Camera::getPosition()
{
	return position;
}
vec3 Camera::getRotation()
{
	return vec3(rotation);
}
float Camera::getRotX()
{
	return rotation.x;
}
float Camera::getRotY()
{
	return rotation.y;
}
float Camera::getRotZ()
{
	return rotation.z;
}

vec3 Camera::getVector()
{
	return vec3(-cos(toRad(rotation.x))*sin(toRad(rotation.y)), sin(toRad(rotation.x))*-cos(toRad(rotation.x)), -cos(toRad(rotation.x))*cos(toRad(rotation.y)));
}


void Camera::moveCameraUp(float direction)
{

}

bool Camera::IsInWindow()
{
	return IsWindow;
}

void Camera::Controller()
{

}
/*
Camera::Camera(vec3 pos, float rotY, float rotX, float rotZ)
{

}
*/
void Camera::setTarget(CharactorController* _target)

{
	target = _target;
}
void Camera::SetInWindow(bool in)
{
	isInWindow = in;
}
void Camera::Update(CharactorController *target)
{
	rotation = target->rotation;
	position = target->position;
	position.z += -30;

}

void Camera::lookAt(CharactorController *target)
{
	static float time;
	
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);                    //first draw Camera.


	glTranslatef(0, 0.0f, -30);
	glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 0.0f);
	//drawCube1();
	FbxArray<FbxPose *> lPoseArray = target->pokeFbx->GetPoseArray();
	//lPoseArray[0]->IsBindPose();
	//target->pokeFbx->SetCurrentPoseIndex(0);
	//glScalef(0.2, 0.2, 0.2);
	//time += Time->deltaTime();
	//if (time > 1/100000000000)
	//{
		//time = 0;
		glScalef(0.2, 0.2, 0.2);
		glTranslatef(0.0, -40.0, 0.0);
		target->pokeFbx->OnDisplay();
	//}

	//XUtils->draw_axes();
	glPopMatrix();

	glPushMatrix();
		//skyBox->LoadSkydome();
	glPopMatrix();
	glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
	skyBox->LoadSkybox();
	//target->pokeFbx->OnDisplay();
	glTranslatef(-position.x, -position.y, -(position.z+30));
	//vec3 vec = m_pTerrain->NormalAt(vec3(position.x, position.y, position.z));

	//drawNormal(vec, height, position);
	XUtils->draw_axes();
	target->pokeFbx->OnTimerClick();

}