#include "pokemon.h"
#include "trainer.h"


; int trainerIDCompare(trainer& a, trainer& b){
	if (a.ID > b.ID){
		return 1;
	}
	else if (a.ID < b.ID){
		return -1;
	}
	else{
		return 0;
	}
}