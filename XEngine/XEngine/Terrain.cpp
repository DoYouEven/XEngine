#include"Terrain.h"


// initial implementation of terrains
vector<Terrain*>Terrain::terrains;
Terrain::Terrain(const char* filename, int _width, int _height, string name)
{
	SDL_Surface* ig = SDL_LoadBMP(filename);
	vector<float>temp;
	if (!ig)
	{
		printf("Terrain map Failed");
	}

	for (int i = 0; i < ig->h; i++)

	{
		temp.clear();
		for (int j = 0; j < ig->w; j++)
		{
			Uint32 pixel = ((Uint32*)ig->pixels)[i*ig->pitch / 4 + j];
			Uint8 r, g, b;
			SDL_GetRGB(pixel, ig->format, &r, &g, &b);
			temp.push_back((float)r / 255.0);
		}
		heights.push_back(temp);
	}
	terrainTex = new Texture("Forest_Floor.tga",TGA);
	SDL_FreeSurface(ig);
	width = _width;
	height = _height;
	int size = width;
	int h = height;
	int k=0;
	int l;
	vector<Vector3>temp1;
	vector<Vector3>temp2;
	vector<Vector3>temp3;
	vector<Vector3>temp4;
	vector<Vector3>ntemp;
	for (int i = 0; i < heights.size() - 1; i++)
	{
		temp1.clear();
		temp2.clear();
		temp3.clear();
		temp4.clear();
		ntemp.clear();
		for (int j = 0; j < heights[0].size() - 1; j++)
		{
			temp1.push_back(Vector3(i*size, heights[i][j] * h, j*size));
			temp2.push_back(Vector3((i + 1)*size, heights[i + 1][j] * h, j*size));
			temp3.push_back(Vector3(i*size, heights[i][j + 1] * h, (j + 1)*size));
			temp4.push_back(Vector3((i + 1)*size, heights[i + 1][j + 1] * h, (j + 1)*size));	
			Vector3 vec1 = Vector3((i + 1)*size, heights[i + 1][j] * h, j*size) - Vector3(i*size, heights[i][j] * h, j*size);
			Vector3 vec2 = Vector3(i*size, heights[i][j + 1] * h, (j + 1)*size) - Vector3(i*size, heights[i][j] * h, j*size);
			ntemp.push_back((vec2 * vec1).normalize());
		}
		vert1.push_back(temp1);
		vert2.push_back(temp2);
		vert3.push_back(temp3);
		vert4.push_back(temp4);
		normals.push_back(ntemp);
		


	}
}

void Terrain::renderTerrain()
{

	int test = 0;
	if (glList == 0)
	{
		
		glList = glGenLists(1);
		glNewList(glList, GL_COMPILE_AND_EXECUTE);
		glEnable(GL_LIGHTING);
		int size = width =1;
		int h = height = 5;
		glTranslatef(0, -20, 0);
		for (int i = 0; i < heights.size() - 1; i++)
		{

			for (int j = 0; j < heights[0].size() - 1; j++)
			{
				glBegin(GL_TRIANGLE_STRIP);
			
				glColor3f(heights[i][j], heights[i][j], heights[i][j]);
				glNormal3f(normals[i][j].x, normals[i][j].y, normals[i][j].z);
				glVertex3f(vert1[i][j].x, vert1[i][j].y , vert1[i][j].z);
				glVertex3f(vert2[i][j].x, vert2[i][j].y , vert2[i][j].z);
				glVertex3f(vert3[i][j].x, vert3[i][j].y , vert3[i][j].z);
				glVertex3f(vert4[i][j].x, vert4[i][j].y , vert4[i][j].z);
				
				if (test)
				{
					glDisable(GL_LIGHTING);
					glBegin(GL_LINES);
					glColor3f(1.0, 1.0, 1.0);
					glVertex3f(vert1[i][j].x, vert1[i][j].y , vert1[i][j].z);
					normals[i][j] /= 10.0;
					vert1[i][j] += normals[i][j];
					glVertex3f(vert1[i][j].x, vert1[i][j].y, vert1[i][j].z);
					glEnable(GL_LIGHTING);
					glEnd();
				
				}

				/*
				glVertex3f(i*size, heights[i][j] * h, j*size);
				glVertex3f((i + 1)*size, heights[i + 1][j] * h, j*size);
				glVertex3f(i*size, heights[i][j + 1] * h, (j + 1)*size);
				glVertex3f((i + 1)*size, heights[i + 1][j + 1] * h, (j + 1)*size);
				*/
				glDisable(GL_LIGHTING);
				glEnd();
			}
			
		}
		glTranslatef(0, 20, 0); 
		glEndList();
	}
	else
	{
		glCallList(glList);
	}
}
Terrain::~Terrain()
{
	//vector<string>().swap(v);

	for (vector<Terrain*> ::iterator it = terrains.begin(); it != terrains.end(); it++)
	{
		if ((*it) == this)
		{
			terrains.erase(it);
		}
	}
	
}