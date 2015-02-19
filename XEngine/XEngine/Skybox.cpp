#include <Windows.h>
#include "Skybox.h"
#include "Texture.h"

#include "SOIL.h"
Skybox::Skybox()
{
	for (int i = 0; i < 6; i++)
	{
		textureIDs[i] = -1;
	}
}

bool Skybox::InitSkydome(const char* fname)
{

	dometextureID = SOIL_load_OGL_texture(fname ,SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
	
	if (dometextureID == 0)
	{
		printf("SkyDomeFilename incorrect");
		return false;
	}
		glBindTexture(GL_TEXTURE_2D, dometextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, 0);
	
	
	radius = 400;
		stacks =256; 
	slices = 256;
	quadObj = gluNewQuadric();
	gluQuadricDrawStyle(quadObj, GLU_FILL);
	gluQuadricOrientation(quadObj, GLU_INSIDE);
	gluQuadricTexture(quadObj, GLU_TRUE);
	return true;
}

void Skybox::LoadSkydome()
{
	glPushAttrib(GL_ENABLE_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();


	// Rotate an additional 90 degrees around the local X axis
	glRotatef(90,- 1.0f, 0.0f, 0.0f);

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, dometextureID);

	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(quadObj, radius, slices, stacks);

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	glPopAttrib();
}
bool Skybox::InitSkybox(char* top, char* bottom, char* back, char* left, char* right, char* front)
{
	
	
	Texture *texture;
	texture = new Texture(top, TGA, "Skybox Texture");
	textureIDs[TOP] = texture->texID;

	texture = new Texture(bottom, TGA, "Skybox Texture");
	textureIDs[BOTTOM] = texture->texID;

	texture = new Texture(right, TGA, "Skybox Texture");
	textureIDs[RIGHT] = texture->texID;

	texture = new Texture(left, TGA, "Skybox Texture");
	textureIDs[LEFT] = texture->texID;

	texture = new Texture(front, TGA, "Skybox Texture"); //bot//left//right//
	textureIDs[FRONT] = texture->texID;

	texture = new Texture(back, TGA, "Skybox Texture");
	textureIDs[BACK] = texture->texID;


	return true;
	
}

void Skybox::LoadSkybox()
{
	glPushAttrib(GL_ENABLE_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glDisable(GL_LIGHTING);

	float size =800;
	//glColor3f(1.0f, 0.5f, 1.0f);
	//Front fact
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureIDs[BACK]);  //use the texture we want
	glBegin(GL_QUADS);      //and draw a face
	//back face
	glTexCoord2f(0, 1);    //use the correct texture coordinate
	glVertex3f(size / 2, size / 2, size / 2);       //and a vertex
	glTexCoord2f(1, 1);     //and repeat it...
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, -size / 2, size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureIDs[LEFT] );
	glBegin(GL_QUADS);
	//left face
	
	glTexCoord2f(0, 1);

	glVertex3f(-size / 2, size / 2, size / 2);  
	glTexCoord2f(1, 1);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureIDs[FRONT]);
	glBegin(GL_QUADS);
	//front face
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureIDs[RIGHT]);
	glBegin(GL_QUADS);
	//right face
	glTexCoord2f(0, 1);
	glVertex3f(size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureIDs[TOP]);
	glBegin(GL_QUADS);                      //top face
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, size / 2, -size / 2);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureIDs[BOTTOM]);
	glBegin(GL_QUADS);
	//bottom face
	glTexCoord2f(1, 1);
	glVertex3f(size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-size / 2, -size / 2, size / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-size / 2, -size / 2, -size / 2);
	glTexCoord2f(1, 0);
	glVertex3f(size / 2, -size / 2, -size / 2);
	glEnd();

	glPopMatrix();

	glPopAttrib();

}
