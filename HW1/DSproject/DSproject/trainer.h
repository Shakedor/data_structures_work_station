#ifndef TRA_H
#define TRA_H

#include "pokemon.h"
#include "AVL.h"
#include "dataStructures.h"
using namespace dataStructures;

typedef enum {
	TRA_SUCCESS, TRA_FAIL, TRA_MEM_ERR
}  traRES;

class trainer{
public:
	int ID;
	AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey> tp_AVL;

	trainer(int id) : ID(id), tp_AVL(pokemonLevelCompare){}
	~trainer(){}

};

int trainerIDCompare(trainer& a, trainer& b);
typedef int(*compareTrainer)(int, int);

#endif // !TRA_H
