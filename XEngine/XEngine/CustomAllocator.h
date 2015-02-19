#ifndef SCENE_OBJECT_H
#include"SceneObject.h"
#endif
class CustomAllocator
{
public:
	static void* MyMalloc(size_t pSize)
	{
		char *p = (char*)malloc(pSize + 1);
		*p = '#';
		return p + 1;
	}

	static void* MyCalloc(size_t pCount, size_t pSize)
	{
		char *p = (char*)calloc(pCount, pSize + 1);
		*p = '#';
		return p + 1;
	}

	static void* MyRealloc(void* pData, size_t pSize)
	{
		if (pData)
		{
			FBX_ASSERT(*((char*)pData - 1) == '#');
			if (*((char*)pData - 1) == '#')
			{
				char *p = (char*)realloc((char*)pData - 1, pSize + 1);
				*p = '#';
				return p + 1;
			}
			else
			{   // Mismatch
				char *p = (char*)realloc((char*)pData, pSize + 1);
				*p = '#';
				return p + 1;
			}
		}
		else
		{
			char *p = (char*)realloc(NULL, pSize + 1);
			*p = '#';
			return p + 1;
		}
	}

	static void MyFree(void* pData)
	{
		if (pData == NULL)
			return;
		FBX_ASSERT(*((char*)pData - 1) == '#');
		if (*((char*)pData - 1) == '#')
		{
			free((char*)pData - 1);
		}
		else
		{   // Mismatch
			free(pData);
		}
	}
};