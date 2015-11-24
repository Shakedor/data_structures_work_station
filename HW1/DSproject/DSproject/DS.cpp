#include "DS.h"




void DS::AddTrainer(int trainerID){
	if (trainerID <= 0){
		throw InvalidInput();
	}
	
	t_AVL.insert(trainerID, trainer(trainerID));

}

void DS::CatchPokemon(int pokemonID, int trainerID, int level){
	// if either ints is not poisitve return invalidinput
	if (pokemonID <= 0 || trainerID <= 0 || level <= 0){
		throw InvalidInput();
	}
	
	// get trainer with that ID , if not exist will throw failure
	trainer& myTrainer = t_AVL.find(trainerID);

	//get pokemon with that id, if exists throw failure
	try{
		p_AVL.find(pokemonID);
	}
	catch (p_AVL.dataAlreadyExists&){// do nothing 
	}
	catch (...){
		throw Failure();
	}

	
	// add pokeymon to the MAVL, AVL and to given trainer
	// if allocation error then remove from previously aded trees
	p_AVL.insert(pokemonID, pokemon(pokemonID, trainerID, level));
	pokemon* pokemonAddress = &p_AVL.find(pokemonID);
	pokemonKey myKey = pokemonKey(pokemonID, level);
	try{ 
		pL_AVL.insert(myKey, pokemonAddress);
		try{
			myTrainer.tp_AVL.insert(myKey, pokemonAddress);
		}
		catch (std::bad_alloc){
			pL_AVL.remove(myKey);
			throw std::bad_alloc();
		}
		catch (...){
			assert(0);
		}
	}
	catch (std::bad_alloc&){
		p_AVL.remove(pokemonID);
		throw std::bad_alloc();		
	}
	catch (...){
		assert(0);
	}

}

void DS::FreePokemon(int pokemonID){
	// if id not positive return invalid input
	if (pokemonID <= 0 ){
		throw InvalidInput();
	}
	// get pokemon with that id from AVL, if not found will throw data does not exist
	pokemon& myPokemon = p_AVL.find(pokemonID);
	
	//get its level and trainer id,
	int trainerID = myPokemon.trainer_ID;
	int level = myPokemon.level;
	pokemonKey myKey = pokemonKey(pokemonID, level);
	// get trainer.
	trainer& myTrainer = t_AVL.find(trainerID);
	// remove pokemon from all 3 trees and delete pokemon
	myTrainer.tp_AVL.remove(myKey);
	pL_AVL.remove(myKey);
	p_AVL.remove(pokemonID);

	// return success

}

void DS::LevelUp(int pokemonID, int levelIncrease){
	// if int non positive return INVALID input
	if (pokemonID <= 0 || levelIncrease <=0){
		throw InvalidInput();
	}

	// get pokemon with that id from AVL, if not found will throw data does not exist
	pokemon& myPokemon = p_AVL.find(pokemonID);

	//get its level and trainer id,
	int trainerID = myPokemon.trainer_ID;
	int level = myPokemon.level;


	// remove this pokemon from all 3 trees,
	//change its level
	// add it to all 3 trees
	FreePokemon(pokemonID);

	CatchPokemon(pokemonID, trainerID, level + levelIncrease);

}


void DS::GetTopPokemon(int trainerID, int *pokemonID){
	//if id== null or trainerID==o return invalid input
	if ( pokemonID==NULL || trainerID == 0){
		throw InvalidInput();
	}

	if (trainerID < 0){
		pokemonID=pL_AVL.
	}
	//if id >0 get trainer ID
	// if not exist retrun failure

	//if exist get its top pokemon id

	// if id <0 get top pokemon id from p_MAVL


}

void DS::GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon){
	//if pointer = null or trainer id =0 return invalid input

	//if trainer id >0 and not exist return failure

	//get size of MAVL tree from either DS or appropriate trainer

	// malloc an array of such ints. // if failed return allocation error
	// preform an postorder walk on the appropriate MAVL tree, for each pokemon put its id in the index

	//put number of pokemon in  pointer and the array of ids in the array pointer

	//return success
}
void DS::EvolvePokemon(int pokemonID, int evolvedID){
	// if either id <=0 return invalid input

	//get pokemon with pokemon id and evolved id, if pid not exist or eveloved id does exist 
	// return failure

	// remove pokemon from all 3 trees,
	// change its id
	// add it to all 3 trees

	//return sucess

}



void DS::UpdateLevels( int stoneCode, int stoneFactor){
	//if code or factor <1 return invalid input
	// make an array of trainers
	//for each trainer, preform 2 conditional walks of pokemons according to is stonde code function
	// destroying the tree after wards
	// so 4 arrays of total size 2n_trainer for each trainer
	// each 2 walks (for condition true and false) reconstruct to an avl tree

	// make the same conditional walks for the p_MAVL
	// for every poemon in true condition, multiply its level by stone factor.
	//then build back the p_mavl true and false conditions trees.

	// now for each false and true tree pair, merge the trees
	// and allocate them back to their place.


}

//*************************





