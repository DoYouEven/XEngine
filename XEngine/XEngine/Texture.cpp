#include "Texture.h"

#define GL_CLAMP_TO_EDGE 0x812F
#ifndef GL_UNSIGNED_SHORT_5_6_5
#define USHORT 0x8363 
#endif 
vector<Texture*>Texture::textures;


Texture::Texture(string _filename, TextureType type, string _name)
{
	imageData = NULL;

	switch (type)
	{
	case TGA: 	loadTGA(_filename); break;
	case BMP:

	{
		const char *fname = _filename.c_str();
		loadBMP(fname);

		break;
	}
	}

	name = _name;
	textures.push_back(this);
}

Texture::~Texture()
{
	for (vector<Texture*> ::iterator it = textures.begin(); it != textures.end(); it++)
	{
		if ((*it) == this)
		{
			textures.erase(it);
		}
	}
}

bool Texture::loadTGA(string filename)
{
	TGA_Header TGAheader;

	ifstream file(filename.data(), std::ios_base::binary);
	{
		if (!file.is_open())
		{
			printf("File is not openning\n");
			return false;
		}
	}
	if (!file.read((char*)&TGAheader, sizeof(TGAheader)))
	{
		printf("Cannot Read header\n");
		return false;
	}
	if (TGAheader.ImageType != 2)
	{
		printf("Wrong Image Type\n");
		return false;
	}
	width = TGAheader.ImageWidth;
	height = TGAheader.ImageHeight;
	bpp = TGAheader.PixelDepth;

	if (width <= 0 || height <= 0 || (bpp != 24 && bpp != 32))
		return false;

	GLuint type = GL_RGBA;
	if (bpp == 24)
	{
		type = GL_RGB;
	}
	GLuint bytesPerPixel = bpp / 8;
	GLuint imageSize = width  * height * bytesPerPixel;

	imageData = new GLubyte[imageSize];

	if (imageData == NULL)
	{
		printf("ImageData is NULL\n");
		return false;
	}

	if (!file.read((char *)imageData, imageSize))
	{
		delete imageData;
		return false;
	}
	//Convert from BGR to RGB
	for (GLuint i = 0; i < imageSize; i += bytesPerPixel)
	{
		GLuint temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}


	createTexture(imageData, width, height, type);
	file.close();
	return true;

}


bool Texture::loadBMP(const char* filename)
{
	glGenTextures(1, &texID);

	SDL_Surface* img = SDL_LoadBMP(filename);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);       //WE need this or else our texxtures will not be smoothly wrapped and we will see the edge of the cubes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, 0x8363, img->pixels);
	//SDL_FreeSurface(img);
	return true;

}
bool Texture::createTexture(unsigned char * imageData, int width, int height, int type)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);       //WE need this or else our texxtures will not be smoothly wrapped and we will see the edge of the cubes
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	return true;
}

