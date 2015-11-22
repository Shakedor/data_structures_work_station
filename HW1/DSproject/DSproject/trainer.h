#ifndef TRA_H
#define TRA_H
#include "pokemon.h"

typedef enum {
	TRA_SUCCESS, TRA_FAIL, TRA_MEM_ERR
}  traRES;

class trainer{
	int ID;
	

public:
	trainer(int id) : ID(id), {};
	int getID() { return ID; };


};


#endif // !TRA_H
