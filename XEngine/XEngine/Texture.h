#ifndef TEXTURE_H
#define TEXTURE_H

#include<Windows.h>
#include <SDL\SDL.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include <string>
#include<vector>
#include<fstream>
using std::vector;
using std::string;
using std::ifstream;
enum TextureType
{
	TGA,
	BMP
};
struct TGA_Header
{
	GLubyte  ID_Lenght;
	GLubyte  ColorMapType;
	GLubyte  ImageType;
	GLubyte colorMapSpecification[5];
	GLshort  xOrigin;
	GLshort  yOrigin;
	GLshort  ImageWidth;
	GLshort  ImageHeight;
	GLubyte PixelDepth;

};
class Texture
{
public:
	Texture(string filename, TextureType type,string name = "");
	virtual ~Texture();

public :
	unsigned char *imageData;
	unsigned int bpp;
	unsigned int width;
	unsigned int height;
	unsigned int texID;

	string name;
	static vector<Texture*> textures;
private:
	bool loadTGA(string filename);
	bool loadBMP(const char* filename);
	bool createTexture(unsigned char * imageData, int width, int height, int type);


};

#endif