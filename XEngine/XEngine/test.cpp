#include<glut.h>
#include<gl\GL.h>
#include <gl\GLU.h>
#include<stdio.h>
#include<SDL\SDL.h>
#ifdef WIN32
#include <Windows.h>
#endif
#define GLUT_KEY_ESCAPE 27
const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;
bool keys[255];
GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;


GLvoid keyboardCB(unsigned char key, int x, int y)
{
	keys[key] = true;
}
GLvoid keyboardUpCB(unsigned char key, int x, int y)
{
	keys[key] = false;
}
GLvoid keyboardSpecialCB( int key, int x, int y)
{
	keys[key] = true;
}
GLvoid keyboardSpecialUpCB( int key, int x, int y)
{
	keys[key] = false;
}

GLboolean checkKeys(GLvoid)
{
	const GLfloat speed = 1.0f;
	if (keys[GLUT_KEY_ESCAPE])
	{
		return true;
	}

	if (keys[GLUT_KEY_LEFT])
	{
		cubeRotateX -= speed;
	}
	if (keys[GLUT_KEY_RIGHT])
	{
		cubeRotateX += speed;
	}

	return false;
}
GLvoid displayFPS(GLvoid)
{
	static long lastTime = glutGet(GLUT_ELAPSED_TIME);
	static long loops = 0;
	static GLfloat fps = 0.0f;
	int newTime = glutGet(GLUT_ELAPSED_TIME);

	if (newTime - lastTime > 100)
	{
		float newFPS = (float)loops / float(newTime - lastTime)*1000.f;
		fps = (fps + newFPS) / 2.0f;
		char title[80];
		sprintf_s(title, "FPS %f",fps);
		glutSetWindowTitle(title);
	}

	loops++;
}
GLvoid establishProjecttionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.001f, 200.0f);
}

GLvoid initGL(GLsizei width, GLsizei height)
{
	establishProjecttionMatrix(width, height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}
GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -10);
	glRotatef(cubeRotateX, 1, 0, 0);
	glRotatef(cubeRotateY, 0, 1, 0);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.5f, 0.0f);
		//Front fact
		glVertex3d(-1.0f, -1.0f, 1.0f);
		glVertex3d(-1.0f, 1.0f, 1.0f);
		glVertex3d(1.0f, 1.0f, 1.0f);
		glVertex3d(1.0f, -1.0f, 1.0f);

		//Backface
		glVertex3d(-1.0f, -1.0f, -1.0f);
		glVertex3d(-1.0f, 1.0f, -1.0f);
		glVertex3d(1.0f, 1.0f, -1.0f);
		glVertex3d(1.0f, -1.0f, -1.0f);
		glColor3f(1.0f, 1.5f, 0.0f);
		//top face
		glVertex3d(-1.0f, 1.0f, -1.0f);
		glVertex3d(-1.0f, 1.0f, 1.0f);
		glVertex3d(1.0f, 1.0f, -1.0f);
		glVertex3d(1.0f, 1.0f, 1.0f);
		//Bottom face
		glVertex3d(-1.0f, -1.0f, -1.0f);
		glVertex3d(-1.0f, -1.0f, 1.0f);
		glVertex3d(1.0f, -1.0f, -1.0f);
		glVertex3d(1.0f, -1.0f, 1.0f);

		//Left Face
		glVertex3d(-1.0f, -1.0f, 1.0f);
		glVertex3d(-1.0f, 1.0f, 1.0f);
		glVertex3d(-1.0f, 1.0f, -1.0f);
		glVertex3d(-1.0f, -1.0f, -1.0f);

		//Right
		glVertex3d(1.0f, -1.0f, 1.0f);
		glVertex3d(1.0f, 1.0f, 1.0f);
		glVertex3d(1.0f, 1.0f, -1.0f);
		glVertex3d(1.0f, -1.0f, -1.0f);

		glEnd();

	

	glFlush();
	glutSwapBuffers();
}

GLvoid update(int value)
{
	displayFPS();
	glutPostRedisplay();
	
	glutTimerFunc(1, update, 0);

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	int windowID = glutCreateWindow("OpenGl Demo");
	glutReshapeWindow(windowWidth, windowHeight);
	initGL(windowWidth, windowHeight);
	glutDisplayFunc(drawScene);

	glutKeyboardFunc(keyboardCB);
	glutKeyboardUpFunc(keyboardUpCB);
	glutSpecialFunc(keyboardSpecialCB);
	glutSpecialUpFunc(keyboardSpecialUpCB);
	glutTimerFunc(1, update, 0);
	glutMainLoop();
}
