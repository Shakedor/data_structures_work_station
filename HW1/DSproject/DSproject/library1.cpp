#include "library1.h"
#include "DS.h"


void* Init(){
	return new(DS);
}

StatusType AddTrainer(void *DS, int trainerID);
StatusType CatchPokemon(void *DS, int pokemonID, int trainerID, int level);
StatusType FreePokemon(void *DS, int pokemonID);
StatusType LevelUp(void *DS, int pokemonID, int levelIncrease);
StatusType GetTopPokemon(void *DS, int trainerID, int *pokemonID);
StatusType GetAllPokemonsByLevel(void *DS, int trainerID, int **pokemons, int numOfPokemon);
StatusType EvolvePokemon(void *DS, int pokemonID, int evolvedID);
StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor);
void Quit(void **DS){
	if (DS != nullptr){
		delete DS;
	}
}