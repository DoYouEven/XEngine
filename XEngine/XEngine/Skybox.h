#ifndef SKYBOX_H
#define SKYBOX_H
/*Sky Box
Only Supports BMP and TGA textures.
*/

//#pragma Xengine
#ifndef GL_H
#include <gl\GL.h>
#include <gl\GLU.h>
#endif
enum SkyBoxFace{
LEFT,
BACK,
FRONT,
TOP,
BOTTOM,
RIGHT
};



class Skybox{

public:
	int textureIDs[6];
	int dometextureID;
	Skybox();
	
	bool InitSkybox(char* top, char* bottom, char* back, char* left, char* right, char* front);
	bool InitSkydome(const char* string);
	void LoadSkybox();
	void LoadSkydome();

private:
	int slices;
	int stacks;
	int radius;
	GLUquadricObj* quadObj;

};

#endif