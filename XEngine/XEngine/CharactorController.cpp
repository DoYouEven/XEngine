
#include "CharactorController.h"

#include <sstream>
#include <SDL\SDL.h>
#include "Camera.h"


void drawNormal(vec3 normal, float height, vec3 position)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	vec3 startpoint = vec3(0,0, 0);
	glVertex3fv(glm::value_ptr(startpoint));
	vec3 endpoint = vec3(normal);
	glVertex3fv(glm::value_ptr(endpoint));
	glEnd();


	glPopAttrib();

	glPopMatrix();
	char string[128];

	sprintf(string, "Camera <%f,%f,%f>", position.x, position.y, position.z);
	XUtils->glPrintf(30, 30, GLUT_BITMAP_9_BY_15, string);
	sprintf(string, "Camera <%f,%f,%f>", endpoint.x, endpoint.y, endpoint.z);
	XUtils->glPrintf(500, 30, GLUT_BITMAP_9_BY_15, string);

}
CharactorController::CharactorController()
{
	
}
CharactorController::CharactorController(SceneObject *_pokeFbx,SceneObject *_idleFbx)
	: Terrain(NULL)
	, HeightOffset(0.5f)
	, Force(0)
	, Velocity(0)
	, Friction(0.9f)
	, IsOnGround(false)
	, sensitivity(0.2)
	, canRotate(true)
{
	idleFbx = _idleFbx;
	runFbx = _pokeFbx;
	pokeFbx = _idleFbx;
	idleFbx->SetCurrentAnimStack(0);
	runFbx->SetCurrentAnimStack(0);
	enabled = false;
}

void CharactorController::SetCamera(Camera* _MainCamera)
{

	MainCamera = _MainCamera;

	//_MainCamera->setTarget(this);
}
void CharactorController::SetTerrain(TerrainLoader* pTerrain)
{
	Terrain = pTerrain;
}
void CharactorController::AddForce(vec3 vec)
{
	glm::quat rotation(glm::vec3(0.0f, glm::radians(-rotation.y), 0.0f));
	Force += (rotation * vec);
}
void CharactorController::WalkForward(float fAmount)
{
	AddForce(vec3(0, 0, fAmount));
	/*float xrotrad, yrotrad;
	yrotrad = (rotation.y / 180.0f * 3.141592654f);
	xrotrad = (rotation.x / 180.0f * 3.141592654f);
	position.x += (float)(sin(yrotrad));
	position.y -= (float)(sin(xrotrad));=*/
}


void CharactorController::Jump(float fAmount)
{
	if (IsOnGround)
	{
		glm::vec3 upVector(0.0f, fAmount, 0.0f);
		Force += upVector;
		IsOnGround = false;
	}
}

void CharactorController::MouseMove(float dx, float dy)
{
	

	rotation.x = glm::clamp<float>(rotation.x + sensitivity*dy, 10, 90); 
	rotation.y += sensitivity*dx;
	 
}
bool isRunning = false;
bool isIdle = false;
void CharactorController::Update(float update)
{	
	if (enabled)
	{
		if (update == 0)
		{
			glPushMatrix();
			glRotatef(180, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.2, 0.2);
			pokeFbx->OnDisplay();
			glPopMatrix();
			return;
		}

		float prevRot = rotation.y;
		if (XUtils->MouseIn == true)
		{
			int centerX = 500;
			int centerY = 500;
			int x, y;
			SDL_GetMouseState(&x, &y);
			MouseMove(centerX - x, centerY - y);
			SDL_WarpMouse(500, 500);

		}
		Uint8 *state = SDL_GetKeyState(NULL);
		if (state[SDLK_SPACE])
		{
			Jump(7000);
		}
		if (state[SDLK_w])		{

			//if (pokeFbx->GetPoseArray() == -1 )
			if (isRunning == false)
			{

				isIdle = false;
				pokeFbx = runFbx;
				isRunning = true;

			}


			//pokeFbx->SetCurrentPoseIndex(0);

			WalkForward(-400);
		}
		else if (state[SDLK_s])
		{

			WalkForward(200);
		}
		else

		{
			if (isIdle == false)
			{
				isRunning = false;
				isIdle = true;
				pokeFbx = idleFbx;

			}

		}
		float deltaTime;
		deltaTime = Time->deltaTime();

		// Add the force of gravity
		Force += glm::vec3(0.0f, 3.5*-100.0f, 0.0f);
		Velocity += (Force * deltaTime);
		position += (Velocity * deltaTime);


		// Make sure we don't fall through the terrain
		if (Terrain != NULL)
		{
			vec3 newps = vec3(position.x, position.y, position.z );
			height = Terrain->HeightAt(newps);

			if (position.y < (height + HeightOffset))
			{
				IsOnGround = true;
				position.y = height + HeightOffset;
			}

			if (rotation.x + position.y <= height)
			{
				rotation.y = prevRot;
			}
		}
		// Reset the force vector
		Force = glm::vec3(0);
		// Slow down our movement by friction amount
		const float fTargetFrameRate = 60.0f;
		Velocity *= powf(Friction, fTargetFrameRate * deltaTime);

		ApplyViewTransform();
	}
	return;
}
void CharactorController::show_camera_stats(void)
{
	char string[128];
	sprintf(string, "H <%f>", height);
	XUtils->glPrintf(400, 30, GLUT_BITMAP_9_BY_15, string);
	sprintf(string, "Camera <%f,%f,%f>", position.x, position.y, position.z);
	XUtils->glPrintf(30, 30, GLUT_BITMAP_9_BY_15, string);
	sprintf(string, "Camera rotation<%f,%f,%f>", rotation.x, rotation.x + position.y, rotation.z);
	XUtils->glPrintf(500, 30, GLUT_BITMAP_9_BY_15, string);
}


void CharactorController::ApplyViewTransform()
{
	MainCamera->Update(this);
	MainCamera->lookAt(this);

		//glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	
}


