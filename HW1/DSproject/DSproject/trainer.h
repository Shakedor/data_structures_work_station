#ifndef TRA_H
#define TRA_H

#include "pokemon.h"

typedef enum {
	TRA_SUCCESS, TRA_FAIL, TRA_MEM_ERR
}  traRES;

class trainer{
	int ID;
	
public:
	trainer(int id) : ID(id){};
	int getID() { return ID; };
	pokemon* getPokemon(int level, int pokID);
	traRES addPokemon(pokemon* pok);
	traRES removePokemon(pokemon* pok);
	~trainer();

};


#endif // !TRA_H
