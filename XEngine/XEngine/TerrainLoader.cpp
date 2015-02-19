
#include "TerrainLoader.h"

//I had intially coded Terrain.h as basic terrain loaded which is included in this project, but was very unhappy with the outcome.
// After Following 4-6 tutorials on terrains, I put together a full fledged terrain loader
//Able to generate multi textures based on height,also highly optimized since uses vbo buffers
//Able to get normals and height at different positions in space;
//Most of this code here I had written based on tutorials on VBO buffers and procedural terrain generation
#ifndef ENABLE_MULTITEXTURE
#define ENABLE_MULTITEXTURE 1
#endif


#ifndef ENABLE_SLOPE_BASED_BLEND
#define ENABLE_SLOPE_BASED_BLEND 1
#endif 

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

inline float GetPercentage(float value, const float min, const float max)
{
	value =clamp(value, min, max);
	return (value - min) / (max - min);
}


inline int GetFileLength(std::istream& file)
{
	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = file.tellg();

	file.seekg(pos);

	return length;
}

inline void DeleteVertexBuffer(GLuint& vboID)
{
	if (vboID != 0)
	{
		glDeleteBuffersARB(1, &vboID);
		vboID = 0;
	}
}

inline void CreateVertexBuffer(GLuint& vboID)
{
	
	DeleteVertexBuffer(vboID);
	glGenBuffersARB(1, &vboID);
}

inline void DeleteTexture(GLuint& texID)
{
	if (texID != 0)
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}
}

inline void CreateTexture(GLuint& texID)
{
	DeleteTexture(texID);
	glGenTextures(1, &texID);
}


inline float GetHeightValue(const unsigned char* data, unsigned char numBytes)
{
	switch (numBytes)
	{
	case 1:
	{
		return (unsigned char)(data[0]) / (float)0xff;
	}
		break;
	case 2:
	{
		return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
	}
		break;
	case 4:
	{
		return (unsigned int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffffff;
	}
		break;
	default:
	{
		assert(false); 
	}
		break;
	}

	return 0.0f;
}

TerrainLoader::TerrainLoader(float heightScale , float blockScale )
	: GLVertexBuffer(0)
	, GLNormalBuffer(0)
	, GLColorBuffer(0)
	, GLTex0Buffer(0)
	, GLTex1Buffer(0)
	, GLTex2Buffer(0)
	, GLIndexBuffer(0)
	, LocalToWorldMatrix(1)
	, InverseLocalToWorldMatrix(1)
	, HeightmapDimensions(0, 0)
	, fHeightScale(heightScale)
	, fBlockScale(blockScale)
{
	memset(GLTextures, 0, sizeof(GLTextures));
}

TerrainLoader::~TerrainLoader()
{
	Terminate();
}

void TerrainLoader::Terminate()
{
	DeleteVertexBuffer(GLVertexBuffer);
	DeleteVertexBuffer(GLNormalBuffer);
	DeleteVertexBuffer(GLColorBuffer);
	DeleteVertexBuffer(GLTex0Buffer);
	DeleteVertexBuffer(GLTex1Buffer);
	DeleteVertexBuffer(GLTex2Buffer);
	DeleteVertexBuffer(GLIndexBuffer);

	for (unsigned int i = 0; i < uiNumTextures; ++i)
	{
		DeleteTexture(GLTextures[i]);
	}
}

bool TerrainLoader::LoadTexture(const string& filename, unsigned int textureStage /*= 0*/)
{
	assert(textureStage < uiNumTextures);
	DeleteTexture(GLTextures[textureStage]);

	GLTextures[textureStage] = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (GLTextures[textureStage] != 0)
	{
		glBindTexture(GL_TEXTURE_2D, GLTextures[textureStage]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return (GLTextures[textureStage] != 0);
}

bool TerrainLoader::LoadHeightmap(const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height)
{
	
	ifstream ifs;
	ifs.open(filename, ifstream::binary);
	if (ifs.fail())
	{
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	const unsigned int bytesPerPixel = bitsPerPixel / 8;
	const unsigned int expectedFileSize = (bytesPerPixel * width * height);
	const unsigned int fileSize = GetFileLength(ifs);

	if (expectedFileSize != fileSize)
	{
		
		return false;
	}

	unsigned char* heightMap = new unsigned char[fileSize];
	ifs.read((char*)heightMap, fileSize);
	if (ifs.fail())
	{
		
		ifs.close();
		delete[] heightMap;
		return false;
	}
	ifs.close();

	unsigned int numVerts = width * height;
	PositionBuffer.resize(numVerts);
	ColorBuffer.resize(numVerts);
	NormalBuffer.resize(numVerts);
	Tex0Buffer.resize(numVerts);

	HeightmapDimensions = glm::u32vec2(width, height);

	// Size of the terrain in world units
	float terrainWidth = (width - 1) * fBlockScale;
	float terrainHeight = (height - 1) * fBlockScale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	for (unsigned int j = 0; j < height; ++j)
	{
		for (unsigned i = 0; i < width; ++i)
		{
			unsigned int index = (j * width) + i;
			assert(index * bytesPerPixel < fileSize);
			float heightValue = GetHeightValue(&heightMap[index * bytesPerPixel], bytesPerPixel);

			float S = (i / (float)(width - 1));
			float T = (j / (float)(height - 1));

			float X = (S * terrainWidth) - halfTerrainWidth;
			float Y = heightValue * fHeightScale;
			float Z = (T * terrainHeight) - halfTerrainHeight;

			// Blend 3 textures depending on the height of the terrain
			float tex0Contribution = 1.0f - GetPercentage(heightValue, 0.0f, 0.75f);
			float tex2Contribution = 1.0f - GetPercentage(heightValue, 0.75f, 1.0f);

			NormalBuffer[index] = glm::vec3(0);
			PositionBuffer[index] = glm::vec3(X, Y, Z);
#if ENABLE_MULTITEXTURE
			ColorBuffer[index] = glm::vec4(tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution);
#else
			ColorBuffer[index] = glm::vec4(1.0f);
#endif
			Tex0Buffer[index] = glm::vec2(S, T);
		}
	}

	std::cout << "TerrainLoader has been loaded!" << std::endl;
	delete[] heightMap;

	GenerateIndexBuffer();
	GenerateNormals();
	GenerateVertexBuffers();

	return true;
}

void TerrainLoader::GenerateIndexBuffer()
{
	if (HeightmapDimensions.x < 2 || HeightmapDimensions.y < 2)
	{
		// TerrainLoader hasn't been loaded, or is of an incorrect size
		return;
	}

	const unsigned int terrainWidth = HeightmapDimensions.x;
	const unsigned int terrainHeight = HeightmapDimensions.y;

	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;

	// 3 indices for each triangle in the terrain mesh
	IndexBuffer.resize(numTriangles * 3);

	unsigned int index = 0; // Index in the index buffer
	for (unsigned int j = 0; j < (terrainHeight - 1); ++j)
	{
		for (unsigned int i = 0; i < (terrainWidth - 1); ++i)
		{
			int vertexIndex = (j * terrainWidth) + i;
			// Top triangle (T0)
			IndexBuffer[index++] = vertexIndex;                           // V0
			IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
			IndexBuffer[index++] = vertexIndex + 1;                       // V1
			// Bottom triangle (T1)
			IndexBuffer[index++] = vertexIndex;                           // V0
			IndexBuffer[index++] = vertexIndex + terrainWidth;            // V2
			IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
		}
	}
}

void TerrainLoader::GenerateNormals()
{
	for (unsigned int i = 0; i < IndexBuffer.size(); i += 3)
	{
		glm::vec3 v0 = PositionBuffer[IndexBuffer[i + 0]];
		glm::vec3 v1 = PositionBuffer[IndexBuffer[i + 1]];
		glm::vec3 v2 = PositionBuffer[IndexBuffer[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		NormalBuffer[IndexBuffer[i + 0]] += normal;
		NormalBuffer[IndexBuffer[i + 1]] += normal;
		NormalBuffer[IndexBuffer[i + 2]] += normal;
	}

	const glm::vec3 UP(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < NormalBuffer.size(); ++i)
	{
		NormalBuffer[i] = glm::normalize(NormalBuffer[i]);

#if ENABLE_SLOPE_BASED_BLEND
		float fTexture0Contribution = glm::saturate(glm::dot(NormalBuffer[i], UP) - 0.1f);
		ColorBuffer[i] = glm::vec4(fTexture0Contribution, fTexture0Contribution, fTexture0Contribution, ColorBuffer[i].w);
#endif

	}
}

void TerrainLoader::GenerateVertexBuffers()
{
	// First generate the buffer object ID's
	CreateVertexBuffer(GLVertexBuffer);
	CreateVertexBuffer(GLNormalBuffer);
	CreateVertexBuffer(GLColorBuffer);
	CreateVertexBuffer(GLTex0Buffer);
	CreateVertexBuffer(GLTex1Buffer);
	CreateVertexBuffer(GLTex2Buffer);
	CreateVertexBuffer(GLIndexBuffer);

	// Copy the host data into the vertex buffer objects
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLVertexBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * PositionBuffer.size(), &(PositionBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLColorBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec4) * ColorBuffer.size(), &(ColorBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLNormalBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * NormalBuffer.size(), &(NormalBuffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLTex0Buffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * Tex0Buffer.size(), &(Tex0Buffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLTex1Buffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * Tex0Buffer.size(), &(Tex0Buffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLTex2Buffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * Tex0Buffer.size(), &(Tex0Buffer[0]), GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GLIndexBuffer);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * IndexBuffer.size(), &(IndexBuffer[0]), GL_STATIC_DRAW_ARB);
}

//Improved Tutorial adding a normal finder
vec3 TerrainLoader::NormalAt(const glm::vec3& position)
{
	float height = -FLT_MAX;
	// Check if the terrain dimensions are valid
	if (HeightmapDimensions.x < 2 || HeightmapDimensions.y < 2) return vec3(0,0,0);

	// Width and height of the terrain in world units
	float terrainWidth = (HeightmapDimensions.x - 1) * fBlockScale;
	float terrainHeight = (HeightmapDimensions.y - 1) * fBlockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	// Multiple the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	glm::vec3 terrainPos = glm::vec3(InverseLocalToWorldMatrix * glm::vec4(position, 1.0f));
	glm::vec3 invBlockScale(1.0f / fBlockScale, 0.0f, 1.0f / fBlockScale);

	// Calculate an offset and scale to get the vertex indices
	glm::vec3 offset(halfWidth, 0.0f, halfHeight);

	// Get the 4 vertices that make up the triangle we're over
	glm::vec3 vertexIndices = (terrainPos + offset) * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;
	glm::vec3 normal;
	if (u0 >= 0 && u1 < (int)HeightmapDimensions.x && v0 >= 0 && v1 < (int)HeightmapDimensions.y)
	{
		glm::vec3 p00 = PositionBuffer[(v0 * HeightmapDimensions.x) + u0];    // Top-left vertex
		glm::vec3 p10 = PositionBuffer[(v0 * HeightmapDimensions.x) + u1];    // Top-right vertex
		glm::vec3 p01 = PositionBuffer[(v1 * HeightmapDimensions.x) + u0];    // Bottom-left vertex
		glm::vec3 p11 = PositionBuffer[(v1 * HeightmapDimensions.x) + u1];    // Bottom-right vertex
		normal = glm::normalize(glm::cross(p10 - p00, p01 - p00));

	}
	
	return normal;


}
float TerrainLoader::HeightAt(const glm::vec3& position)
{
	float height = -FLT_MAX;
	// Check if the terrain dimensions are valid
	if (HeightmapDimensions.x < 2 || HeightmapDimensions.y < 2) return height;

	// Width and height of the terrain in world units
	float terrainWidth = (HeightmapDimensions.x - 1) * fBlockScale;
	float terrainHeight = (HeightmapDimensions.y - 1) * fBlockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	// Multiple the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	glm::vec3 terrainPos = glm::vec3(InverseLocalToWorldMatrix * glm::vec4(position, 1.0f));
	glm::vec3 invBlockScale(1.0f / fBlockScale, 0.0f, 1.0f / fBlockScale);

	// Calculate an offset and scale to get the vertex indices
	glm::vec3 offset(halfWidth, 0.0f, halfHeight);

	// Get the 4 vertices that make up the triangle we're over
	glm::vec3 vertexIndices = (terrainPos + offset) * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if (u0 >= 0 && u1 < (int)HeightmapDimensions.x && v0 >= 0 && v1 < (int)HeightmapDimensions.y)
	{
		glm::vec3 p00 = PositionBuffer[(v0 * HeightmapDimensions.x) + u0];    // Top-left vertex
		glm::vec3 p10 = PositionBuffer[(v0 * HeightmapDimensions.x) + u1];    // Top-right vertex
		glm::vec3 p01 = PositionBuffer[(v1 * HeightmapDimensions.x) + u0];    // Bottom-left vertex
		glm::vec3 p11 = PositionBuffer[(v1 * HeightmapDimensions.x) + u1];    // Bottom-right vertex

		// Which triangle are we over?
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		glm::vec3 dU, dV;
		if (percentU > percentV)
		{   // Top triangle
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else
		{   // Bottom triangle
			dU = p11 - p01;
			dV = p01 - p00;
		}

		glm::vec3 heightPos = p00 + (dU * percentU) + (dV * percentV);
		// Convert back to world-space by multiplying by the terrain's world matrix
		heightPos = glm::vec3(LocalToWorldMatrix * glm::vec4(heightPos, 1));
		height = heightPos.y;
	}

	return height;
}

void TerrainLoader::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(LocalToWorldMatrix));

	//
	// Texture Stage 0
	//
	// Simply output texture0 for stage 0.
	//
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(32.0f, 32.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextures[0]);

	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLTex0Buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

#if ENABLE_MULTITEXTURE
	
	glDisable(GL_LIGHTING);                                              //

//stage 1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(32.0f, 32.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextures[1]);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_COLOR);

	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLTex1Buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

//Stage 2
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(32.0f, 32.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GLTextures[2]);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE_ARB);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);

	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_PRIMARY_COLOR_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLTex2Buffer);
	glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));

#else
	glEnable(GL_TEXTURE);
	glEnable(GL_LIGHTING);
#endif

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLVertexBuffer);
	glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLColorBuffer);
	glColorPointer(4, GL_FLOAT, 0, BUFFER_OFFSET(0));
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, GLNormalBuffer);
	glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GLIndexBuffer);
	glDrawElements(GL_TRIANGLES, IndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

#if ENABLE_MULTITEXTURE
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glClientActiveTextureARB(GL_TEXTURE2_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glClientActiveTextureARB(GL_TEXTURE1_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#endif

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

#if _DEBUG
	//DebugRender();
#endif

}

void TerrainLoader::RenderNormals()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(glm::value_ptr(LocalToWorldMatrix));

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 0.0f);// Yellow

	glBegin(GL_LINES);
	{
		for (unsigned int i = 0; i < PositionBuffer.size(); ++i)
		{
			glm::vec3 p0 = PositionBuffer[i];
			glm::vec3 p1 = (PositionBuffer[i] + NormalBuffer[i]);

			glVertex3fv(glm::value_ptr(p0));
			glVertex3fv(glm::value_ptr(p1));
		}
	}
	glEnd();

	glPopAttrib();

	glPopMatrix();
}

void TerrainLoader::DebugRender()
{
	RenderNormals();
}