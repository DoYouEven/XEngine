
#include"SceneObject.h"

#include<Windows.h>
#include <math.h>
#include "XEngine.h"
#include "SOIL.h"
#include "glm.hpp"
#include "Utilities.h"
#include "PokemonObject.h"
//#include"Terrain.h"


using std::vector;
const GLsizei windowWidth = 1440;
const GLsizei windowHeight = 900;
GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;
Texture *texture = NULL;
Uint8 *keys = NULL;
int *seed = NULL;
GLuint gridList = 0;
Camera* MainCamera;


vector<ObjectLoader*> static_objects;
ObjectLoader *House;
ObjectLoader *Lego;


//**********LIGHTING
Light *MainLight = NULL;
bool ToggleMainLight = true;
bool ToggleSpotLight = false;
bool EnableSpinLights = false;
int cube;


PokemonObject pokeObj;
SceneObject * RunFbx;
SceneObject * IdleFbx;
vector<vector<float>> heights;


//*********************STATIC STUFF FOR DEMO
TerrainLoader* terrain;
TerrainLoader* terrain2;
AudioSource *music;
TerrainLoader g_Terrain(30.0f, 2.0f);
Skybox* skybox;
bool FbxLoaded = false;
bool DrawSkybox = true;
float updateVal = 0;


enum EngineMode{
	PLAY_MODE,
	PAUSE_MODE,
	SCENE_MODE
};

EngineMode eMode = SCENE_MODE;
//*********************************Inspector*
MouseInput input;
Inspector* Selected_Inspector;
float sliderVal = 5.0;
//iList *list1;



//***************************UI*****************************

iTexture *mainBackground; Texture* tMainBackground;
iTexture* ViewPortFrame; Texture* tViewPortFrame;
Texture* tTopFrame;		iTexture* TopFrame;
Texture* tRightFrame;		iTexture* RightFrame;
iTexture* LeftFrame;
Texture* tBottomFrame;  iTexture* BottomFrame;
iTexture* ObjLoader;
//This camera is used for navigationg within the Engine

//*******************OBJ VIEWER*************************
vector<ObjectLoader*> objList;
iList* objListViewer;
iButton* addObj;
iButton* loadObj;
iTextBox* tbLoadObj;


//*************************UI LEFT FRAME
iSlider *STX, *STY, *STZ, *SRX, *SRY, *SRZ, *SSX, *SSY, *SSZ;
//****************TOP FRAME*************************
iButton *btnParent, *btnUnParent;
iButton *btnCreateSphere, *btnCreatePlane;
iButton *btnAddTexture;
iButton *btnAmbientLight;
iButton *btnSpotLights;
iButton *btnSpinLights;
iButton *btnAddSkyBox;
iButton *btnAddTerrain;
iButton *btnAddTerrain2;
iButton *btnAddPlane;
iButton *btnToggleAxes;
iButton *btnPlay;
iButton *btnStop;
iButton *addThirdPersonController;
//******************LEFT FRAME*************************
iTextBox* tbTexture;

Mesh * selectedMesh = NULL;

list<Mesh*> selectedMeshes;
;
bool isNavigating;
vec2 navRotation = vec2(45, 45);


vec3 zoom = vec3(0.0, 0.0, -10.0);
enum UITexture{

	BUTTON_BASE,
	BUTTON_HOVER,
	BOTTOM_FRAME,
	MAIN_BACKGROUND,
	VIEW_PORT_FRAME,
	TOP_FRAME,
	RIGHT_FRAME,
	LEFT_FRAME,
	SLIDER_BASE,
	SLIDER_KNOB,
	INS_BG,
	BUTTON_PLAY_UP,
	BUTTON_PLAY_DOWN,
	BUTTON_STOP_UP,
	BUTTON_STOP_DOWN,
	OBJ_LOADER
};

int UITextures[16];
//***************************************Inspector Hierarchy
iButton *btnCreateCube;

//*****************METHOD DECLRATIONS
void deleteNode(Node* node)
{
	list<Node*> children = node->getChildren();

	for (list<Node*>::iterator it = children.begin(); it != children.end(); it++)
	{
		deleteNode(*it);  //avoid mem leaks and children without parents!LOL
	}
	delete node;
}

GLvoid drawScene(bool display = false);
void updateViewPortNavigator()
{
	static int prevX = -1;
	static int prevY = -1;
	static int prevZ = -1;
	if (prevX == -1 && prevY == -1 && prevZ == -1)
	{
		prevX = input.x;
		prevY = input.y;
		prevZ = 3.25;
	}
	int dx = prevX - input.x;
	int dy = prevY - input.y;
	int dz;
	prevX = input.x;
	prevY = input.y;


	if (input.LeftMouseButton && Selected_Inspector == NULL)
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);
		navRotation.y -= (float)dx *0.5f;
		navRotation.x -= (float)dy *0.5f;
		isNavigating = true;
	}
	else if (input.RightMouseButton && Selected_Inspector == NULL)
	{
		SDL_WM_GrabInput(SDL_GRAB_ON);
		zoom.x -= dx * MAXZOOMSPEED;
		zoom.y -= dy * MAXZOOMSPEED;


	}
	else if (input.ScrollDown && Selected_Inspector == NULL)
	{
		zoom.z -= 1;
	}
	else if (input.ScrollUp && Selected_Inspector == NULL)
	{
		zoom.z += 1;
	}
	else
	{
		SDL_WM_GrabInput(SDL_GRAB_OFF);
		isNavigating = false;
	}

	Uint8 *state = SDL_GetKeyState(NULL);
	if (state[SDLK_DELETE])
	{

		for (list<Mesh*>::iterator it = selectedMeshes.begin(); it != selectedMeshes.end(); it++)
		{
			Mesh *mesh = dynamic_cast <Mesh*> (*it);
			if (mesh == NULL)
				continue;
			mesh->setParent(NULL);

		}
		for (list<Mesh*>::iterator it = selectedMeshes.begin(); it != selectedMeshes.end(); it++)
		{
			Mesh *mesh = dynamic_cast <Mesh*> (*it);
			if (mesh == NULL)
				continue;
			deleteNode(mesh);

		}
		iSlider::enable = false;
		//selectedMesh = nullMesh;
		selectedMeshes.clear();
	}
}
GLvoid displayFPS(GLvoid)
{
	static long lastTime = SDL_GetTicks();
	static long loops = 0;
	static GLfloat fps = 0.0f;
	int newTime = SDL_GetTicks();

	if (newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime)*1000.f;
		fps = (fps + newFPS) / 2.0f;
		char title[80];
		sprintf_s(title, "Powered by XEngine V1.0 <<FPS %f>>", fps);
		SDL_WM_SetCaption(title, 0);
	}

	loops++;
}





GLvoid SpinLights()
{
	for (int i = 0; i < (int)Light::lights.size(); i++)
	{

		double rand = (double)SDL_GetTicks() + seed[i];
		float x = (float)sin(rand / 1600.0f) * (float)cos(rand / 1200.0f)*50.0f;
		float y = (float)sin(rand / 900.0f) * (float)cos(rand / 1400.0f)*50.0f;
		Light::lights[i]->setPosition(x, 100, y);
		Light::lights[i]->updateLight();
	}
}



void UpdateInspector()
{
	STX->setVal(&selectedMesh->getTransform().position.x);
	STY->setVal(&selectedMesh->getTransform().position.y);
	STZ->setVal(&selectedMesh->getTransform().position.z);

	SRX->setVal(&selectedMesh->getTransform().rotation.x);
	SRY->setVal(&selectedMesh->getTransform().rotation.y);
	SRZ->setVal(&selectedMesh->getTransform().rotation.z);

	SSX->setVal(&selectedMesh->getTransform().scale.x);
	SSY->setVal(&selectedMesh->getTransform().scale.y);
	SSZ->setVal(&selectedMesh->getTransform().scale.z);

}
void HandleEvent(Inspector * currentInspector)
{
	if (addThirdPersonController == currentInspector)
	{
		pokeObj.enabled = true;
		updateVal == 0;
	}

	if (btnPlay == currentInspector)
	{
		updateVal = 1;
		eMode = PLAY_MODE;
		music->Play();
		SDL_ShowCursor(SDL_DISABLE);
		DrawSkybox = false;
	}
	if (btnStop == currentInspector)
	{
		SDL_ShowCursor(SDL_ENABLE);
		updateVal = 0;
		eMode = SCENE_MODE;
		music->Stop();
		DrawSkybox = true;
	}

	if (currentInspector == btnToggleAxes)
	{
		XUtils->EnableAxes = !XUtils->EnableAxes;
	}
	if (currentInspector == btnAddPlane)
	{
		Plane *plane = new Plane();
		plane->divNum = 100;
		plane->setSize(150, 150);
		plane->create();
		selectedMesh = (Mesh*)plane;
		selectedMeshes.push_back(plane);
		UpdateInspector();
		zoom = vec3(0, 0, -25);
	}

	if (currentInspector == loadObj)
	{
		Mesh *mesh = new Mesh();
		if (objList.at(objListViewer->getIndex()) != NULL)
		{

			mesh->obj = objList.at(objListViewer->getIndex());
			const char *cstr = mesh->obj->filename.c_str();
			sprintf(mesh->name, cstr);
			mesh->setMeshType(OBJ);
			selectedMesh = mesh;
			selectedMeshes.clear();
			selectedMeshes.push_back(selectedMesh);
			zoom.z = mesh->obj->getRadius() * -4;
			UpdateInspector();
		}
	}
	if (currentInspector == addObj)
	{
		const char *cstr = tbLoadObj->text.c_str();
		strlen(cstr);
		char  *copy = new char(strlen(cstr) + 1);
		strcpy(copy, cstr);
		ObjectLoader* obj = new ObjectLoader(tbLoadObj->text);

		objList.push_back(obj);
		objListViewer->add(copy);
	}
	if (currentInspector == btnAddTerrain)
	{

		Mesh *mesh = new Mesh();

		sprintf(mesh->name, "Terrain [T]");
		mesh->terrain = terrain;
		mesh->setMeshType(TERRAIN);
		selectedMesh = mesh;
		pokeObj.SetTerrain(terrain);
		//cube->drawMesh();
		selectedMeshes.clear();
		selectedMeshes.push_back(selectedMesh);
		UpdateInspector();
	}
	if (currentInspector == btnAddTerrain2)
	{

		Mesh *mesh = new Mesh();

		sprintf(mesh->name, "Terrain2 [T]");
		mesh->terrain = terrain2;
		mesh->setMeshType(TERRAIN);
		selectedMesh = mesh;
		pokeObj.SetTerrain(terrain2);
		//cube->drawMesh();
		selectedMeshes.clear();
		selectedMeshes.push_back(selectedMesh);
		UpdateInspector();
	}
	if (currentInspector == btnAddTexture)

	{
		if (selectedMesh != NULL)
		{
			const char *cstr = tbTexture->text.c_str();
			texture = new Texture(cstr, TGA, "Surface Texture");
			selectedMesh->texID = texture->texID;
		}
	}

	if (currentInspector == btnAmbientLight)
	{
		ToggleMainLight = !ToggleMainLight;
		for (int i = 0; i < Light::lights.size(); i++)
		{
			if (Light::lights[i]->lightType == DIRECTIONAL_LIGHT)
				Light::lights[i]->visible = ToggleMainLight;
		}


	}
	if (currentInspector == btnSpotLights)
	{

		ToggleSpotLight = !ToggleSpotLight;
		for (int i = 0; i < Light::lights.size(); i++)
		{
			if (Light::lights[i]->lightType == POINT_LIGHT)
				Light::lights[i]->visible = ToggleSpotLight;
		}
	}

	if (currentInspector == btnSpinLights)
	{
		EnableSpinLights = !EnableSpinLights;

	}
	if (currentInspector == btnAddSkyBox)
	{

		Mesh *mesh = new Mesh();

		sprintf(mesh->name, "Skybox[T]");
		mesh->skybox = skybox;
		mesh->setMeshType(SKYBOX);
		selectedMesh = mesh;
		//cube->drawMesh();
		selectedMeshes.clear();
		selectedMeshes.push_back(selectedMesh);
		UpdateInspector();
	}

	if (currentInspector == btnCreateCube)
	{
		Cube *cube = new Cube();
		cube->create();
		cube->setMeshType(CUBE);
		selectedMesh = cube;
		//cube->drawMesh();
		selectedMeshes.clear();
		selectedMeshes.push_back(selectedMesh);
		UpdateInspector();
		zoom = vec3(0, 0, -25);
	}
	if (currentInspector == btnCreateSphere)
	{
		Sphere* sphere = new Sphere();
		sphere->Init(1, 32, 32);
		sphere->create();

		selectedMesh = sphere;
		selectedMeshes.clear();
		selectedMeshes.push_back(selectedMesh);
		UpdateInspector();
		zoom = vec3(0, 0, -25);
	}
	else if (currentInspector == btnParent && selectedMeshes.size() > 1)
	{
		Mesh *parent = (*(--selectedMeshes.end()));
		for (list<Mesh*> ::iterator it = selectedMeshes.begin(); it != selectedMeshes.end(); it++)
		{
			Mesh * mesh = *it;

			if (mesh && mesh != parent)
			{
				mesh->setParent(NULL);
			}
		}
		for (list<Mesh*> ::iterator it = selectedMeshes.begin(); it != selectedMeshes.end(); it++)
		{
			Mesh * mesh = *it;

			if (mesh && mesh != parent)
			{
				mesh->setParent(parent);
			}
		}

	}
	else if (currentInspector == btnUnParent)
	{
		for (list<Mesh*> ::iterator it = selectedMeshes.begin(); it != selectedMeshes.end(); it++)
		{
			Mesh * mesh = *it;

			if (mesh)
			{
				mesh->setParent(NULL);
			}
		}
	}

}


//http://web.cse.ohio-state.edu/~hwshen/581/Site/Slides_files/picking.pdf reference
void HandleViewPortSelection()
{
	static bool btnDown = false;

	if (input.MiddleButton)
		btnDown = true;
	else if (btnDown)
	{
		btnDown = false;
		GLuint zBuffer[512];

		GLint viewport[4];

		glGetIntegerv(GL_VIEWPORT, viewport);
		glSelectBuffer(512, zBuffer);

		glRenderMode(GL_SELECT);
		glInitNames();

		glMatrixMode(GL_PROJECTION);

		glPushMatrix();
		glLoadIdentity();

		gluPickMatrix(input.x, viewport[3] - input.y, 1.0f, 1.0f, viewport); //SELECT A VERY SMALL AREA FROM THE LOOP

		gluPerspective(45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.1f,1000.0f);

		drawScene(true);
		glMatrixMode(GL_PROJECTION);         //we are getting total hits on the stack
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		int xyCollisions = glRenderMode(GL_RENDER);
		bool found = false;
		if (xyCollisions > 0)
		{
			int isValid = zBuffer[3];
			int selection = zBuffer[6];
			int depth = zBuffer[4];

			for (int i = 1; i < xyCollisions; i++)
			{
				if (zBuffer[i * 4 - 1] == 1)
				{
					found = true;
					if (zBuffer[i * 4] < depth)
					{
						depth = zBuffer[i * 4];
						selection = zBuffer[i * 4 + 2];

					}

				}


			}
			if (found)
			{
				selectedMesh = reinterpret_cast<Mesh*>(selection);    //Converted the integer name of the zBuffered selected stack at that time of mouse collision in the 
				iSlider::enable = true;



				Uint8 *state = SDL_GetKeyState(NULL);
				if (!state[SDLK_LSHIFT])
				{
					selectedMeshes.clear();
				}


				//Meed to check if selected mesh exists
				bool exists = false;
				for (list<Mesh*>::iterator it = selectedMeshes.begin(); it != selectedMeshes.end(); it++)
				{
					if ((*it) == selectedMesh)
					{
						exists = true;
						break;
					}
				}

				if (!exists)
					selectedMeshes.push_back(selectedMesh);
			}
			UpdateInspector();
		}
		else
			selectedMeshes.clear();

	}
}



void DrawHeirarchyRecursive(Node* Children, int *interation)
{
	for (list<Node*>::iterator it = Children->getChildren().begin(); it != Children->getChildren().end(); it++)
	{
		Mesh *mesh = dynamic_cast<Mesh*>(*it);
		if (std::find(selectedMeshes.begin(), selectedMeshes.end(), mesh) != selectedMeshes.end())
		{
			(*interation)++;
			char buf[40];
			glColor3f(0.1, 0.9, 0.1);
			if (mesh->texID>0)
				sprintf(buf, "%s [T]-->Selected node", mesh->name);
			else
				sprintf(buf, "%s -->Selected node", mesh->name);
			XUtils->glPrintf(5 + (*interation) * 2, 2 * (TOP_FRAME_HEIGHT - 30) + *interation*TEXT_HEIGHT, GLUT_BITMAP_8_BY_13, buf);
		}
		else
		{
			char buf[40];
			(*interation)++;
			glColor3f(1.0, 1.0, 1.0);
			if (mesh->texID>0)
				sprintf(buf, ">>%s [T]", mesh->name);
			else
				sprintf(buf, ">>%s ", mesh->name);
			XUtils->glPrintf(5 + (*interation) * 2, 2 * (TOP_FRAME_HEIGHT - 30) + *interation*TEXT_HEIGHT, GLUT_BITMAP_8_BY_13, buf);
		}

		DrawHeirarchyRecursive(mesh, interation);
	}
}
void DrawHeirarchyList()
{
	int i = 0;

	for (list<Node*>::iterator it = Node::getNodes().begin(); it != Node::getNodes().end(); it++)
	{

		Mesh *node = dynamic_cast<Mesh*>(*it);
		if (node && node->getParent() == NULL)
		{
			if (std::find(selectedMeshes.begin(), selectedMeshes.end(), node) != selectedMeshes.end())
			{
				i++;
				char buf[40];
				glColor3f(0.1, 0.9, 0.1);
				if (node->texID>0)
					sprintf(buf, "%s [T]-->Selected node", node->name);
				else
					sprintf(buf, "%s -->Selected node", node->name);
				XUtils->glPrintf(5, 2 * (TOP_FRAME_HEIGHT - 30) + i*TEXT_HEIGHT, GLUT_BITMAP_8_BY_13, buf);
			}
			else
			{
				char buf[40];
				i++;
				glColor3f(1.0, 1.0, 1.0);
				if (node->texID>0)
					sprintf(buf, "%s [T]", node->name);
				else
					sprintf(buf, "%s", node->name);
				XUtils->glPrintf(5, 2 * (TOP_FRAME_HEIGHT - 30) + i*TEXT_HEIGHT, GLUT_BITMAP_8_BY_13, buf);
			}
			DrawHeirarchyRecursive(node, &i);

		}

	}
	/*for (list<Node*>::iterator it = Node::getNodes().begin(); it != Node::getNodes().end(); it++)
	{

	Mesh *node = dynamic_cast<Mesh*>(*it);
	if (node != NULL)
	{
	if (std::find(selectedMeshes.begin(), selectedMeshes.end(), node )!= selectedMeshes.end())
	{

	char buf[40];
	glColor3f(0.1, 0.9, 0.1);
	sprintf(buf, "%s -->Selected node", node->name);
	XUtils->glPrintf(5, 2 * TOP_FRAME_HEIGHT + i*TEXT_HEIGHT, GLUT_BITMAP_8_BY_13, buf);
	}
	else
	glColor3f(1.0, 1.0, 1.0);
	XUtils->glPrintf(5, 2 * TOP_FRAME_HEIGHT + i*TEXT_HEIGHT, GLUT_BITMAP_8_BY_13, node->name);
	}
	i++;
	}
	*/
}
void DrawInspectorList()
{
	for (list<Inspector*> ::iterator it = Inspector::InspectorUnits.begin(); it != Inspector::InspectorUnits.end(); it++)
	{
		Inspector * currentInspector = *it;

		currentInspector->drawInspectorUnit();

		if (Selected_Inspector != NULL && Selected_Inspector != currentInspector || isNavigating)  //need to maintain priority for one control
			continue;

		if (currentInspector->updateMouse(input))
		{

			Selected_Inspector = currentInspector;
			//zoom.z = static_objects[list1->getIndex()]->getRadius() * -3;
			HandleEvent(currentInspector);
			// handle events;

		}
		else if (currentInspector == Selected_Inspector)
		{
			Selected_Inspector = NULL;
		}
	}

}
void drawloader(ObjectLoader *object)
{
	//glRotatef(MainCamera.getRotY() - 90, 0, 1, 0);
	//glRotatef(-MainCamera.getRotX(), 0, 0, 1);
	glPushMatrix();
	glTranslatef(10, 10, 10);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	object->drawModel();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPopMatrix();
}
GLvoid drawScene(bool display)
{
	if (display == false)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		X_Engine->establishProjecttionMatrix(windowWidth, windowHeight);

	}


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);


	//glEnable(GL_LIGHTING);
	if (EnableSpinLights)
		SpinLights();

	XUtils->glEnable2D();
	mainBackground->drawUITexture();
	//ViewPortFrame->drawUITexture();
	glTranslatef(0, 0, 0);
	XUtils->glDisable2D();


	//*********************************** TOP AXES
	if (eMode == SCENE_MODE)
	{
		glPushMatrix();
		glTranslatef(3.75, 2.75, -10);
		glRotatef(navRotation.x, 1, 0, 0);
		glRotatef(navRotation.y, 0, 1, 0);
		glScalef(0.15, 0.15, 0.15);
		XUtils->draw_axes();
		glPopMatrix();
	}
	glPushMatrix();
	if (eMode == PLAY_MODE)
		pokeObj.Update(updateVal);
	else if (eMode == SCENE_MODE)
	{
		//view port camera
		if (eMode == SCENE_MODE || eMode == PAUSE_MODE)
		glTranslatef(zoom.x, zoom.y, zoom.z);

		glRotatef(navRotation.x, 1, 0, 0);
		glRotatef(navRotation.y, 0, 1, 0);
	}





	for (int i = 0; i < Light::lights.size(); i++)
	{
		Light::lights[i]->updateLight();
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//glDisable(GL_LIGHTING);
	//if (list1 != NULL)
	//{
	//	//static_objects[list1->getIndex()]->drawModel();
	//}
	glPopAttrib();

	//**********************draw grid

	//skybox->LoadSkydome();
	glPushMatrix();
	glEnable(GL_LIGHTING);
	for (list<Node*>::iterator it = Node::getNodes().begin(); it != Node::getNodes().end(); it++)
	{
		Mesh *mesh = dynamic_cast<Mesh*>(*it);
		if (mesh && mesh->getParent() == NULL)
		{

			mesh->drawMesh(true, NULL, true, DrawSkybox);
		}

	}

	if (eMode == SCENE_MODE)
		pokeObj.Update(updateVal);
	//***********SELECTED ITEMS

	list<Mesh*> itms = selectedMeshes;
	int item_size = itms.size();
	while (item_size > 0)
	{
		for (list<Node*>::iterator it = Node::getNodes().begin(); it != Node::getNodes().end(); it++)
		{
			Mesh *mesh = dynamic_cast<Mesh*>(*it);
			if (mesh && mesh->getParent() == NULL)
			{
				if (item_size < 1)
					break;
				for (list<Mesh*>::iterator m = itms.begin(); m != itms.end(); m++)
				{
					Mesh * inMesh = (*m);
					//glDisable(GL_LIGHTING);
					//glColor4f(0.0, 1.0f, 0.0f, 0.2f);
					if (mesh->drawMesh(true, inMesh, true, DrawSkybox))
					{
						//XUtils->draw_axes();
						item_size -= 1;
						//m = itms.erase(m); //itms.remove(inMesh);
						if (itms.size() == 0)
							break;
					}
				}



			}
		}

	}


	glPushMatrix();
	if (eMode == SCENE_MODE || eMode == PAUSE_MODE)
		XUtils->draw_grid();
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();

	if (display == true)
		return;
	//****************************************TEXTURE FINAL
	if (eMode == SCENE_MODE || eMode == PAUSE_MODE)
	{
		glPushMatrix();
		XUtils->glEnable2D();

		TopFrame->drawUITexture();
		RightFrame->drawUITexture();
		LeftFrame->drawUITexture();
		BottomFrame->drawUITexture();
		ObjLoader->drawUITexture();
		DrawHeirarchyList();
		DrawInspectorList();
		XUtils->glDisable2D();
		glPopMatrix();
	}
	/*

	//drawGrid();

	XUtils->glEnable2D();
	mainBackground->drawUITexture();
	//ViewPortFrame->drawUITexture();
	glTranslatef(-10, 0,0);

	XUtils->glDisable2D();


	//*********************************** TOP AXES
	glPushMatrix();
	glTranslatef(4.5, 3.25, -10);
	glRotatef(navRotation.x, 1, 0, 0);
	glRotatef(navRotation.y, 0, 1, 0);
	glScalef(0.15, 0.15, 0.15);
	XUtils->draw_axes();
	glPopMatrix();


	//*********************************** TOP AXES
	glPushMatrix();
	glTranslatef(zoom.x, zoom.y, zoom.z);
	glRotatef(navRotation.x, 1, 0, 0);
	glRotatef(navRotation.y, 0, 1, 0);

	//******************DRAW SCENE*****************************************
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	if (list1 != NULL)
	{
	//static_objects[list1->getIndex()]->drawModel();
	}
	glPopAttrib();

	//drawCube();\
	glPushMatrix();
	XUtils->draw_grid();
	glPopMatrix();
	for (list<Node*>::iterator it = Node::getNodes().begin(); it != Node::getNodes().end(); it++)\
	{
	Mesh *mesh = dynamic_cast<Mesh*>(*it);
	if (mesh && mesh->getParent() == NULL)
	mesh->drawMesh();
	}


	XUtils->glEnable2D();
	TopFrame->drawUITexture();
	RightFrame->drawUITexture();
	DrawInspectorList();
	XUtils->glDisable2D();
	//*********************UII
	//glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);


	//drawGrid();
	*/
	glFlush();

	SDL_GL_SwapBuffers();



	/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                   // Clear the color buffer, and the depth buffer.

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//light->setPosition(-MainCamera.getPosition().x, 100, -MainCamera.getPosition().z);
	//light->setSpotDirection(0, -0.707, 0);




	glEnable(GL_LIGHTING);
	glPushMatrix();
	//SpinLights();
	pokeObj.Update(20);
	glTranslatef(0, -20, 0);
	glEnable(GL_LIGHTING);
	g_Terrain.Render();
	glTranslatef(0, 0, 20);
	drawloader(House);
	//drawloader(Lego);

	glPushMatrix();
	//glScalef(0.3, 0.3, 0.3);
	glTranslatef(4, 30, 0);


	//RunFbx->OnDisplay();  //Load Pikachu one


	glTranslatef(10, 0, 10);


	//RunFbx->OnDisplay();  //Load Pikachu two
	glTranslatef(10, 0, 10);


	//RunFbx->OnDisplay();  //Load Pikachu two


	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	SDL_GL_SwapBuffers();


	if (RunFbx->GetStatus() == SceneObject::MUST_BE_LOADED)
	{
	// This function is only called in the first display callback
	// to make sure that the application window is opened and a
	// status message is displayed before.
	RunFbx->LoadFile();
	//CreateMenus();
	// Call the timer to display the first frame.
	}
	*/
}


void InitCamera()
{

}
int main(int argc, char**argv)
{
	displayFPS();
	X_Engine->Init(windowWidth, windowHeight);
	Mix_Chunk* effect1;

	music = new AudioSource("audio.wav", Music);
	glEnable(GL_DEPTH_TEST);

	//*********************************BUTTTONS*******************************
	texture = new Texture("Textures/UI/Button1_up.tga", TGA, "Surface Texture");
	UITextures[BUTTON_BASE] = texture->texID;
	texture = new Texture("Textures/UI/Button1_down.tga", TGA, "Surface Texture");
	UITextures[BUTTON_HOVER] = texture->texID;
	texture = new Texture("Textures/UI/Button_Play_Up_30x30.tga", TGA, "Surface Texture");
	UITextures[BUTTON_PLAY_UP] = texture->texID;
	texture = new Texture("Textures/UI/Button_Play_Down_30x30.tga", TGA, "Surface Texture");
	UITextures[BUTTON_PLAY_DOWN] = texture->texID;
	texture = new Texture("Textures/UI/Button_Stop_Down_30x30.tga", TGA, "Surface Texture");
	UITextures[BUTTON_STOP_DOWN] = texture->texID;
	texture = new Texture("Textures/UI/Button_Stop_Up_30x30.tga", TGA, "Surface Texture");
	UITextures[BUTTON_STOP_UP] = texture->texID;

	//******************************INSPECTOR FRAMES AND BACKGROUND
	tMainBackground = new Texture("Textures/UI/Background_1440x900.tga", TGA, "Surface Texture");
	UITextures[MAIN_BACKGROUND] = tMainBackground->texID;
	tViewPortFrame = new Texture("Textures/UI/Top Frame.tga", TGA, "Surface Texture");
	UITextures[VIEW_PORT_FRAME] = tViewPortFrame->texID;
	tTopFrame = new Texture("Textures/UI/Top(79H).tga", TGA, "Surface Texture");
	UITextures[TOP_FRAME] = tTopFrame->texID;
	tRightFrame = new Texture("Textures/UI/Right.tga", TGA, "Surface Texture");
	UITextures[RIGHT_FRAME] = tRightFrame->texID;
	tRightFrame = new Texture("Textures/UI/Left.tga", TGA, "Surface Texture");
	UITextures[LEFT_FRAME] = tRightFrame->texID;
	tBottomFrame = new Texture("Textures/UI/Bottom(25H).tga", TGA, "Surface Texture");
	UITextures[BOTTOM_FRAME] = tBottomFrame->texID;
	texture = new Texture("Textures/UI/OBJ_Loader.tga", TGA, "Surface Texture");
	UITextures[OBJ_LOADER] = texture->texID;
	//*******************************SLIDERS**********************************************************


	texture = new Texture("Textures/UI/Slider_Background_190x15.tga", TGA, "Surface Texture");
	UITextures[SLIDER_BASE] = texture->texID;
	texture = new Texture("Textures/UI/Slider_15x15.tga", TGA, "Surface Texture");
	UITextures[SLIDER_KNOB] = texture->texID;
	texture = new Texture("Textures/UI/Button1_down.tga", TGA, "Surface Texture");
	UITextures[INS_BG] = texture->texID;


	//***************************LOADING BACKGROUND TEXTURES******************************************

	mainBackground = new iTexture("Test", 0, 0, 1440, 900, UITextures[MAIN_BACKGROUND]);
	//ViewPortFrame = new iTexture("Test", 300, 0, 900, 900, UITextures[VIEW_PORT_FRAME]);
	TopFrame = new iTexture("TopFrame", 0, 0, 1440, TOP_FRAME_HEIGHT, UITextures[TOP_FRAME]);
	RightFrame = new iTexture("TopFrame", 1440 - 244, TOP_FRAME_HEIGHT - 1, 244, LR_FRAME_HEIGHT, UITextures[RIGHT_FRAME]);
	LeftFrame = new iTexture("TopFrame", 0, TOP_FRAME_HEIGHT - 1, 244, LR_FRAME_HEIGHT, UITextures[LEFT_FRAME]);
	BottomFrame = new iTexture("TopFrame", 0, 900 - 25, 1440, 25, UITextures[BOTTOM_FRAME]);
	ObjLoader = new iTexture("TopFrame", 0, 555, 240, 323, UITextures[OBJ_LOADER]);
	//************************OBJECT VIEWER****************************************************

	tbLoadObj = new iTextBox("TextBox1", 0, 580, 185, TEXTBOX_HEIGHT, UITextures[BUTTON_BASE]);
	addObj = new iButton("ADD .obj", 180, 580, BUTTON_WIDTH - 13, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]);
	loadObj = new iButton("LOAD OBJECT TO HEIRARCHY", 0, 845, 238, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]);
	objListViewer = new iList(0, 645, 238, 200);
	//iSlider *testSlider = (iSlider*)XUtils->addInspector(new iSlider("TestSlider",-5.0, 5.0,0, 0, 200, 50));

	tbTexture = new iTextBox("TextBox1", 0, 515, 185, TEXTBOX_HEIGHT, UITextures[BUTTON_BASE]);
	btnAddTexture = new iButton("Add texture", 180, 515, BUTTON_WIDTH - 13, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]);

	//**********************************Top Frame*********************************************
	btnToggleAxes = (iButton*)XUtils->addInspectorTopFrame(new iButton("Toggle Axes[ON/OFF]", 0, 0, BUTTON_WIDTH + 30, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnAddPlane = (iButton*)XUtils->addInspectorTopFrame(new iButton("Create Plane", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnCreateCube = (iButton*)XUtils->addInspectorTopFrame(new iButton("Create Cube", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnCreateSphere = (iButton*)XUtils->addInspectorTopFrame(new iButton("Create Sphere", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));

	//************************************NODE BUTTONS***************************************
	btnParent = (iButton*)XUtils->addInspectorTopFrame(new iButton("Parent", 0, 0, BUTTON_WIDTH - 30, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnUnParent = (iButton*)XUtils->addInspectorTopFrame(new iButton("UnParent", 0, 0, BUTTON_WIDTH - 20, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	//**********************************************Static models

	btnAmbientLight = (iButton*)XUtils->addInspectorTopFrame(new iButton("Toggle Directional Light", 0, 0, BUTTON_WIDTH + 40, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnSpotLights = (iButton*)XUtils->addInspectorTopFrame(new iButton(" Toggle Spot Light", 0, 0, BUTTON_WIDTH + 30, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnSpinLights = (iButton*)XUtils->addInspectorTopFrame(new iButton(" Spin Spot Lights", 0, 0, BUTTON_WIDTH + 30, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnAddSkyBox = (iButton*)XUtils->addInspectorTopFrame(new iButton("Add Sky Dome", 0, 0, BUTTON_WIDTH - 5, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnAddTerrain = (iButton*)XUtils->addInspectorTopFrame(new iButton("Load Terrain", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnAddTerrain2 = (iButton*)XUtils->addInspectorTopFrame(new iButton("Load Terrain 2", 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	addThirdPersonController = (iButton*)XUtils->addInspectorTopFrame(new iButton("Add third person controller[FBX]", 0, 0, BUTTON_WIDTH + 85, BUTTON_HEIGHT, UITextures[BUTTON_BASE], UITextures[BUTTON_HOVER]));
	btnPlay = (iButton*)XUtils->addInspectorTopFrame(new iButton("", 0, 0, 35, 35, UITextures[BUTTON_PLAY_UP], UITextures[BUTTON_PLAY_DOWN]));
	btnStop = (iButton*)XUtils->addInspectorTopFrame(new iButton("", 0, 0, 35, 35, UITextures[BUTTON_STOP_DOWN], UITextures[BUTTON_STOP_UP]));

	//static_objects.push_back(new ObjectLoader("SuperMan.obj"));
	//static_objects.push_back(new ObjectLoader("Models/House08.obj"));
	//list1->setSelected(0);

	//zoom.z = static_objects[list1->getIndex()]->getRadius() * -3;



	//**************************LIGHT************************************************************


	MainLight = new Light(DIRECTIONAL_LIGHT);
	MainLight->setDiffuse(0.8, 0.8, 0.8, 1.0f);
	MainLight->setAmbient(1.0, 1.0, 1.0, 1.0f);
	MainLight->setPosition(0, 100, 0);


	//*************************SLIDERS*******************************************************
	STX = (iSlider*)XUtils->addInspector(new iSlider("Translate X", -100, 100, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));
	STY = (iSlider*)XUtils->addInspector(new iSlider("Translate Y", -100, 100, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));
	STZ = (iSlider*)XUtils->addInspector(new iSlider("Translate Z", -100, 100, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));

	SRX = (iSlider*)XUtils->addInspector(new iSlider("Rotation X", -360, 360, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));
	SRY = (iSlider*)XUtils->addInspector(new iSlider("Rotation Y", -360, 360, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));
	SRZ = (iSlider*)XUtils->addInspector(new iSlider("Rotation Z", -360, 360, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));

	SSX = (iSlider*)XUtils->addInspector(new iSlider("Scale X", .1, 10, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));
	SSY = (iSlider*)XUtils->addInspector(new iSlider("Scale Y", .1, 10, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));
	SSZ = (iSlider*)XUtils->addInspector(new iSlider("Scale Z", .1, 10, 0, 0, 230, 40, UITextures[SLIDER_BASE], UITextures[SLIDER_KNOB], UITextures[INS_BG]));


	glPushMatrix();
	XUtils->glEnable2D();

	TopFrame->drawUITexture();
	RightFrame->drawUITexture();
	LeftFrame->drawUITexture();
	BottomFrame->drawUITexture();
	ObjLoader->drawUITexture();
	DrawHeirarchyList();
	DrawInspectorList();
	XUtils->glDisable2D();
	glPopMatrix();

	//**********************************INIT BASE VIEWPORT SCENE*********************************************************************
	Plane *plane = new Plane();
	plane->divNum = 100;
	plane->setSize(180, 180);
	plane->create();
	selectedMesh = (Mesh*)plane;
	selectedMeshes.push_back(plane);
	UpdateInspector();
	iSlider::enable = true;
	skybox = new Skybox();


	terrain = new TerrainLoader(30.0, 2.0f);
	terrain->LoadHeightmap("Terrain/Kanto.raw", 8, 256, 256);
	terrain->LoadTexture("Textures/grass4.jpg", 0);
	terrain->LoadTexture("Textures/rock.png", 1);
	terrain->LoadTexture("Textures/snow.jpg", 2);


	terrain2 = new TerrainLoader(60.0, 2.0f);
	terrain2->LoadHeightmap("Terrain/KantoHeight.raw", 16, 257, 257);
	terrain2->LoadTexture("Textures/rock2.jpg", 0);
	terrain2->LoadTexture("Textures/grass.jpg", 1);
	terrain2->LoadTexture("Textures/snow3.jpg", 2);



	//texture = new Texture("skyTop.tga", TGA, "Surface Texture");
	skybox->InitSkybox("Textures/Skybox/skyTop.tga", "Textures/Skybox/skyBottom.tga", "Textures/Skybox/skyBack.tga", "Textures/Skybox/skyLeft.tga", "Textures/Skybox/skyRight.tga", "Textures/Skybox/skyFront.tga");
	//skyBox.InitSkybox("ashcanyon_up.tga", "ashcanyon_dn.tga", "ashcanyon_bk.tga", "ashcanyon_lf.tga", "ashcanyon_rt.tga", "ashcanyon_ft.tga");

	//skyBox = new Skybox();
	//skyBox->InitSkydome("Textures/sky_map_p_04_5200x1300.jpg");
	//glEnable(GL_LIGHTING);
	//Lego = new ObjectLoader("SuperMan.obj");
	//House = new ObjectLoader("Models/House08.obj");
	RunFbx = new SceneObject("Pikachu_Running.FBX", 1000, 1000, 1);
	RunFbx->LoadFile();
	IdleFbx = new SceneObject("Pikachu_Idle_Inches.FBX", 1000, 1000, 1);
	IdleFbx->LoadFile();

	//Objects->loadMaterial("cube.mtl");


	//terrain = new Terrain("Sample.bmp", 2, 5);

	//g_Camera.SetTerrain(&g_Terrain);
	MainCamera = new Camera(skybox);

	pokeObj = PokemonObject(RunFbx, IdleFbx);
	pokeObj.SetTerrain(terrain);
	pokeObj.SetCamera(MainCamera);



	seed = new int[8];
	bool mouseIn = false;
	for (int i = 0; i < 7; i++)
	{
		Light* light = new Light(POINT_LIGHT);
		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		light->setDiffuse(r, g, b, 1);
		light->setAmbient(0.3, 0.1, 0.2, 1);
		light->setSpotDirection(0, -1, 0);
		light->setPosition(0, 50, 0);
		float cutoff = 20.0f + (float)rand() / (float)RAND_MAX *60.0f;
		light->visible = false;
		light->setCutoff(cutoff);
		light->setExponent(20.0f * cutoff);
		seed[i] = rand();
	}




	//effect1= Mix_LoadWAV("ThrowPokeball.wav");
	//Mix_VolumeChunk(effect1, 29);
	static int node_iterator = 0;
	int max_itr = selectedMeshes.size();
	int done = 0;
	while (!done)
	{
		if (eMode == PAUSE_MODE)
			music->Stop();

		/*
		if (selectedMeshes.size() >0)
		{
		max_itr = (max_itr - 1) % (selectedMeshes.size());
		std::list<Mesh*>::iterator it = selectedMeshes.begin();
		std::advance(it, max_itr);
		selectedMesh = *it;
		if (max_itr == 0)
		max_itr = (selectedMeshes.size()) ;
		UpdateInspector();
		}
		*/
		SDL_GetMouseState(&input.x, &input.y);
		input.LeftMouseButton = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
		input.MiddleButton = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
		input.RightMouseButton = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);


		drawScene();
		HandleViewPortSelection();
		displayFPS();
		updateViewPortNavigator();
		SDL_Event event;
		input.keydown = false;
		input.ScrollUp = false;
		input.ScrollDown = false;
		while (SDL_PollEvent(&event))
		{

			input.keydown = false;
			switch (event.type)
			{

			case SDL_QUIT:
				done = 1; break;
			case SDL_KEYUP:
				input.keydown = false;	 break;
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_WHEELUP:
					input.ScrollUp = true; break;
				case SDL_BUTTON_WHEELDOWN:
					input.ScrollDown = true; break;
				}break;
			case SDL_KEYDOWN:
			{

				switch (event.key.keysym.sym)
				{

				case SDLK_ESCAPE:
				{
					done = 1;
					break;
				}

				case SDLK_1:
				{
					//Mix_PlayChannel(-1, effect1, 0);
					//	Mix_PlayMusic(music, 1);
					//Mix_FadeInMusic(music, 1,40);
					//AudioSource::PlayOneShot("skyTop.bmp");
					cubeRotateX -= 30;

					break;
				}
				case SDLK_PAGEDOWN:
				{

					SDL_ShowCursor(SDL_ENABLE);
					XUtils->SetMouseIn(false);
					eMode = PAUSE_MODE;
					break;
				}


				}
				if (input.keydown == true)
					input.keydown = false;
				else {
					input.event = event;
					input.keydown = true;

				}
				break;
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{

				//SDL_ShowCursor(SDL_DISABLE);
				XUtils->SetMouseIn(true);
				break;


			}

			}

		}



	}

	while (Node::getNodes().size() > 1)
		delete(*Node::getNodes().begin());
	/*
	for (int i = 0; i < Light::lights.size(); i++)
	{
	delete Light::lights[i];
	}
	*/
	SDL_Quit();
	return 0;
}