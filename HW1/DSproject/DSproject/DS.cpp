#include "pokemon.h"
#include "trainer.h""
#include "DS.h"


DS::DS(){
	// initialize the trainer AVL
	// the pokemon by level MAVL
	// the pokemon by id AVL

	//if either of the 3 fail MEMERR, call destructor
}

DS::~DS(){
	// check if any of the 3 trees are null, if so destroy them
	// remember that for the pokemon AVL we need to 
	//delete the actual pokemon for every pokemon in the tree

}

StatusType DS::AddTrainer(int trainerID){
	if (trainerID <= 0){
		return INVALID_INPUT;
	}
	// if trainer already exists return failure

	//create new trainer
	// if allocation failed, return ALLOCFAIL

	// insert trainer to T_AVL
	return SUCCESS;


}

StatusType DS::CatchPokemon(int pokemonID, int trainerID, int level){
	// if either ints is not poisitve return invalidinput
	// get trainer with that ID , if not exist return failure

	//get pokemon with that id, if exists return failure

	// create new pokemon, if failed return allocation error.
	//add it to the MAVL, AVL and to given trainer

	//return success
}

StatusType DS::FreePokemon(int pokemonID){
	// if id not positive return invalid input
	// get pokemon with that id from AVL
	// if null return failure.

	//get its level and trainer id,
	// get trainer.
	// remove pokemon from all 3 trees and delete pokemon

	// return success

}

StatusType DS::LevelUp(int pokemonID, int levelIncrease){
	// if int non positive return INVALID input
	// get pokemon from AVL
	// if not exists return failure.
	// get level and trainer
	
	// remove this pokemon from all 3 trees,
	//change its level
	// add it to all 3 trees

	//return success

}


StatusType DS::GetTopPokemon(int trainerID, int *pokemonID){
	//if id== null or trainerID==o return invalid input

	//if id >0 get trainer ID
	// if not exist retrun failure

	//if exist get its top pokemon id

	// if id <0 get top pokemon id from p_MAVL


}

StatusType DS::GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon){
	//if pointer = null or trainer id =0 return invalid input

	//if trainer id >0 and not exist return failure

	//get size of MAVL tree from either DS or appropriate trainer

	// malloc an array of such ints. // if failed return allocation error
	// preform an postorder walk on the appropriate MAVL tree, for each pokemon put its id in the index

	//put number of pokemon in  pointer and the array of ids in the array pointer

	//return success
}
StatusType DS::EvolvePokemon(int pokemonID, int evolvedID){
	// if either id <=0 return invalid input

	//get pokemon with pokemon id and evolved id, if pid not exist or eveloved id does exist 
	// return failure

	// remove pokemon from all 3 trees,
	// change its id
	// add it to all 3 trees

}



StatusType DS::UpdateLevels(void *DS, int stoneCode, int stoneFactor){}

//*************************


void* Init(){
StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor);
void Quit(void **DS){
	if (DS != nullptr){
		delete DS;
	}
}


