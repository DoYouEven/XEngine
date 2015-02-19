#pragma once
#ifndef POKEMONOBJECT_H
#define POKEMONOBJECT_H
#ifndef FPC_H
#include "CharactorController.h"
#endif

class PokemonObject : public CharactorController
{


public:

	PokemonObject();
	PokemonObject(SceneObject *fbxPoke, SceneObject* idlePoke) :CharactorController(fbxPoke, idlePoke){}
	void Init();


};

#endif