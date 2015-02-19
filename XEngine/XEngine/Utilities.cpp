#include "Utilities.h"
#include "Inspector.h"


class Inspector;
float Utilities::m_fMaxTimeStep;
float Utilities::m_fPrevious;

bool Utilities::EnableAxes = true;
Utilities::Utilities()
{
	m_fMaxTimeStep = maxTimeStep;
}
Utilities::~Utilities()
{

}

void Utilities::UnInit()
{
	delete Utilities::getUtility();
}
Utilities *Utilities::getUtility()
{
	static Utilities *e = new Utilities();
	return e;
}

float Utilities::deltaTime() 
{
	float fCurrentTime = clock() / (float)CLOCKS_PER_SEC;
	float fDeltaTime = fCurrentTime - m_fPrevious;
	m_fPrevious = fCurrentTime;

	// Clamp to the max time step
	if (fDeltaTime < m_fMaxTimeStep)
		return fDeltaTime;
	else
		return m_fMaxTimeStep;
	return fDeltaTime;
}
void Utilities::glEnable2D(void)
{
	GLint iViewport[4];

	glGetIntegerv(GL_VIEWPORT, iViewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(iViewport[0], iViewport[0] + iViewport[2],
		iViewport[1] + iViewport[3], iViewport[1], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
}
void Utilities::glDisable2D(void)
{
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
void Utilities::glPrintf(int x, int y, void* font, const char* string, ...)
{
	char *c;
	char temp[256];
	va_list ap;

	va_start(ap, string);
	vsprintf(temp, string, ap);
	va_end(ap);

	glEnable2D();

	glRasterPos2i(x, y);

	for (c = temp; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}

	glDisable2D();
}

Inspector* Utilities:: addInspector(Inspector *_inspector)
{
	static int prevX = 1200;
	static int prevY = TOP_FRAME_HEIGHT+30;

	_inspector->position.x = prevX;
	_inspector->position.y = prevY;
	prevY += _inspector->height + 5;

	return _inspector;
}

Inspector* Utilities::addInspectorTopFrame(Inspector *_inspector)
{
	static int prevX = 5;
	static int prevY = 5;

	_inspector->position.x = prevX;
	_inspector->position.y = prevY;
	prevX += _inspector->width+ 5;

	return _inspector;
}
void Utilities::SetMouseIn(bool in)
{
	MouseIn = in;
}
void Utilities::draw_axes(void)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glScalef(2, 2, 2);
		glTranslatef(0.0f, 0.0f, 0.0f);
		glLineWidth(1.0f);
		glBegin(GL_LINES);

	
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.0f);

	
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 2.0f, 0.0f);


	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 2.0f);

	glEnd();

	glPopMatrix();

	glPopAttrib();
}

void Utilities::draw_grid()
{
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_LINE_SMOOTH);
	// Draw a grid 500*500
	glColor3f(0.3f, 0.3f, 0.3f);
	glLineWidth(1.0);
	const int hw = 500;
	const int step = 20;
	const int bigstep = 100;
	int       i;

	// Draw Grid
	for (i = -hw; i <= hw; i += step) {

		if (i % bigstep == 0) {
			glLineWidth(2.0);
		}
		else {
			glLineWidth(1.0);
		}
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glVertex3i(i, 0, -hw);
		glVertex3i(i, 0, hw);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINES);
		glVertex3i(-hw, 0, i);
		glVertex3i(hw, 0, i);
		glEnd();

	}

	// Write some grid info
	const GLfloat zoffset = -2.f;
	const GLfloat xoffset = 1.f;

	for (i = -hw; i <= hw; i += bigstep)
	{

		FbxString scoord;
		int lCount;

		// Don't display origin
		//if (i == 0) continue;
		if (i == 0) {
			scoord = "0";
			lCount = (int)scoord.GetLen();

			glPushMatrix();
			glDisable(GL_TEXTURE_2D);
			glVertex3f(i + xoffset, 0, zoffset);
			glRotatef(-90, 1, 0, 0);


			glPopMatrix();

			continue;
		}

		// X coordinates
		scoord = "X: ";
		scoord += i;
		lCount = (int)scoord.GetLen();

		glPushMatrix();
		glTranslatef(i + xoffset, 0, zoffset);
		glRotatef(-90, 1, 0, 0);

		glPopMatrix();

		// Z coordinates
		scoord = "Z: ";
		scoord += i;
		lCount = (int)scoord.GetLen();

		glPushMatrix();
		glTranslatef(xoffset, 0, i + zoffset);
		glRotatef(-90, 1, 0, 0);

		glPopMatrix();

	}

	glPopMatrix();
	glPopAttrib();
}