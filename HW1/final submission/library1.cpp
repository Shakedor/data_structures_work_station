#include "library1.h"
#include "DS_struct.h"
#include "dataStructures.h"


#define TRY_CATCH(x) do{try{x}\
	catch (DS_struct::InvalidInput&){return INVALID_INPUT; }\
	catch (std::bad_alloc&){return ALLOCATION_ERROR;}\
	catch (dataStructures::failureExceptions){return FAILURE; }\
	catch (...){assert(0);}\
return SUCCESS;\
} while (0)

void* Init(){
	return new(DS_struct);
}

StatusType AddTrainer(void *DS, int trainerID){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->AddTrainer(trainerID););
}

StatusType CatchPokemon(void *DS, int pokemonID, int trainerID, int level){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->CatchPokemon(pokemonID,trainerID,level););
}

StatusType FreePokemon(void *DS, int pokemonID){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->FreePokemon(pokemonID););
}

StatusType LevelUp(void *DS, int pokemonID, int levelIncrease){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->LevelUp(pokemonID, levelIncrease););
}


StatusType GetTopPokemon(void *DS, int trainerID, int *pokemonID){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->GetTopPokemon(trainerID,pokemonID););
}
StatusType GetAllPokemonsByLevel(void *DS, int trainerID, int **pokemons, int* numOfPokemon){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->GetAllPokemonsByLevel(trainerID, pokemons, numOfPokemon););
}

StatusType EvolvePokemon(void *DS, int pokemonID, int evolvedID){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->EvolvePokemon(pokemonID, evolvedID););
}
StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor){
	if (DS == NULL){
		return INVALID_INPUT;
	}
	DS_struct* ds = (DS_struct*)DS;
	TRY_CATCH(ds->UpdateLevels(stoneCode, stoneFactor););
}

void Quit(void **DS){
	DS_struct** ds = (DS_struct**)(DS);
	if (ds != NULL){
		delete *ds;
		*ds = NULL;
	}
}