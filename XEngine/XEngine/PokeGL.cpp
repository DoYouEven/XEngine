#include "PokeGL.h"





void PokeGL::start()
{

	AudioSource *music = new AudioSource("ThrowPokeball.wav", FX);
	glEnable(GL_DEPTH_TEST);

	//texture = new Texture("skyTop.tga", TGA,"Surface Texture");
	//texture = new Texture("skyTop.tga", TGA, "Surface Texture");
	//skyBox.InitSkybox("skyTop.tga", "skyBottom.tga", "skyBack.tga", "skyLeft.tga", "skyRight.tga", "skyFront.tga");
	//skyBox.InitSkybox("ashcanyon_up.tga", "ashcanyon_dn.tga", "ashcanyon_bk.tga", "ashcanyon_lf.tga", "ashcanyon_rt.tga", "ashcanyon_ft.tga");
	//glEnable(GL_LIGHTING);


	//Objects = new ObjectLoader("SuperMan.obj");
	//Objects->loadMaterial("cube.mtl");
	//seed = new int[8];

	Vector3 vec1(1, 2, 3);
	Vector3 vec2(1, 2, 3);


	Vector3 **vec3;
	bool mouseIn = false;
	int x = vec1.dotProduct(vec2);
	//std::cout << vec1;
	//std::cout << vec2;
	printf("The dot product is%d", x);

	for (int i = 0; i < 8; i++)
	{

		Light *lights = new Light(DIRECTIONAL_LIGHT);
		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		lights->setDiffuse(1, 1, 1, 1);
		lights->setAmbient(1.0, 1.0, 1.0, 1.0);
		lights->setSpotDirection(0, -1, 0);
		lights->setPosition(0, -1, -3);
		float cutoff = 20.0f + (float)rand() / (float)RAND_MAX *60.0f;
		lights->setCutoff(cutoff);
		lights->setExponent(20.0f * cutoff);
		//seed[i] = rand();
	}
}