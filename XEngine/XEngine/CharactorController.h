
#ifndef CHARACTOR_CONTROLLER_H
#define CHARACTOR_CONTROLLER_H

#include"SceneObject.h"

#ifndef GLM_H
#include "glm.hpp"
#endif
#include "TerrainLoader.h"
#include "Utilities.h"
#include "Transform.h"

#include "Rigidbody.h"

class Camera;


using glm::vec3;
class CharactorController : public Rigidbody
{
public:
	bool enabled;
	vec3 position;
	vec3 rotation;
	SceneObject *pokeFbx;
	SceneObject *idleFbx;
	SceneObject *runFbx;
	Camera* MainCamera;
	CharactorController() ;
	CharactorController(SceneObject *pokeFbx, SceneObject *idlePoke);
	vec2 prevDxDy;
	vec2 currDxDy;
	void SetTerrain(TerrainLoader* pTerrain);
	float sensitivity;
	void SetClamp(void);
	void WalkForward(float fAmount);
	void Strafe(float fAmount);
	void Jump(float fAmount);

	// Add the mouse X and Y movement to look around
	void MouseMove(float dx, float dy);
	void show_camera_stats(void);
	// Update causes the camera to be pushed back down onto the terrain
	void Update(float fDeltaTime);
	void AddForce(vec3 vec);
	void ApplyViewTransform();
	void SetCamera(Camera* MainCamera);
	
protected:

private:
	bool canRotate;
	TerrainLoader*    Terrain;
	float       HeightOffset;
	float minPitch;
	float maxPitch;
	float height;
	vec3   Force;
	vec3   Velocity;

	// Friction to be applied to velocity
	float       Friction;
	bool        IsOnGround;
};

#endif