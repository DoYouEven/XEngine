#include "XEngine.h"

XEngine::XEngine()
{

}
XEngine::~XEngine()
{

}

GLvoid XEngine::UnInit()
{
	delete XEngine::getXEngine();
}
XEngine *XEngine::getXEngine()
{
	static XEngine *e = new XEngine();
	return e;
}

GLvoid XEngine::establishProjecttionMatrix(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.01f, 1000.00f);
}

GLvoid XEngine::initGL(GLsizei width, GLsizei height)
{
	establishProjecttionMatrix(width, height);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}
void XEngine::Init(GLint windowWidth, GLint windowHeight)

{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	Uint32 flags = SDL_SWSURFACE;
	//SDL_Surface *screen = SDL_SetVideoMode(windowWidth, windowHeight, 32, flags);
	//flags = screen->flags;
	//screen = SDL_SetVideoMode(0, 0, 0, screen->flags ^ SDL_FULLSCREEN);
	// screen = SDL_SetVideoMode(0, 0, 0, flags);
	SDL_SetVideoMode(windowWidth, windowHeight, 0, SDL_OPENGL);
	initGL(windowWidth, windowHeight);
	SDL_EnableUNICODE(1);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Light::Init();
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initilalize GLEW!" << std::endl;
		exit(-1);
	}
	if (!glewIsSupported("GL_ARB_multitexture "
		"GL_ARB_texture_env_combine "
		"GL_ARB_vertex_buffer_object "
		))
	{
		std::cout << "Required OpenGL extension support is missing." << std::endl;
		exit(-1);
	}
}
