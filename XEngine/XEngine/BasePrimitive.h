#ifndef BASE_PRIMITIVE
#define BASE_PRIMITIVE

#include "Mesh.h"


class BasePrimitive : public Mesh

{
public:

	virtual void create(char* name = "") = 0;

};

#endif