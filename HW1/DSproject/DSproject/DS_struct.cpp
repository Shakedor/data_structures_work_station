#include "DS_struct.h"
#include "dataStructures.h"



void DS_struct::AddTrainer(int trainerID){
	if (trainerID <= 0){
		throw InvalidInput();
	}
	
	t_AVL.insert(trainerID, smart_pointer<trainer>(new trainer(trainerID)));

}

void DS_struct::CatchPokemon(int pokemonID, int trainerID, int level){
	// if either ints is not poisitve return invalidinput
	if (pokemonID <= 0 || trainerID <= 0 || level <= 0){
		throw InvalidInput();
	}
	
	// get trainer with that ID , if not exist will throw failure
	smart_pointer<trainer> myTrainer = t_AVL.find(trainerID);

	//get pokemon with that id, if exists throw failure
	try{
		p_AVL.find(pokemonID);
	}
	catch (dataStructures::dataDoesNotExist&){// do nothing 
	}
	catch (...){
		throw Failure();
	}

	
	// add pokeymon to the MAVL, AVL and to given trainer
	// if allocation error then remove from previously aded trees
	
	smart_pointer<pokemon> pokemonAddress(new pokemon(pokemonID, trainerID, level));
	
	p_AVL.insert(pokemonID, pokemonAddress);
	pokemonKey myKey = pokemonKey(pokemonID, level);
	try{ 
		pL_AVL.insert(myKey, pokemonAddress);
		try{
			myTrainer->tp_AVL.insert(myKey, pokemonAddress);
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

void DS_struct::FreePokemon(int pokemonID){
	// if id not positive return invalid input
	if (pokemonID <= 0 ){
		throw InvalidInput();
	}
	// get pokemon with that id from AVL, if not found will throw data does not exist
	smart_pointer<pokemon> myPokemon = p_AVL.find(pokemonID);
	
	//get its level and trainer id,
	int trainerID = myPokemon->trainer_ID;
	int level = myPokemon->level;
	pokemonKey myKey = pokemonKey(pokemonID, level);
	// get trainer.
	smart_pointer<trainer>  myTrainer = t_AVL.find(trainerID);
	// remove pokemon from all 3 trees and delete pokemon
	myTrainer->tp_AVL.remove(myKey);
	pL_AVL.remove(myKey);
	p_AVL.remove(pokemonID);

	//myPokemon is deleted once it goes out of scope
	// return success

}

void DS_struct::LevelUp(int pokemonID, int levelIncrease){
	// if int non positive return INVALID input
	if (pokemonID <= 0 || levelIncrease <=0){
		throw InvalidInput();
	}

	// get pokemon with that id from AVL, if not found will throw data does not exist
	smart_pointer<pokemon> myPokemon = p_AVL.find(pokemonID);

	//get its level and trainer id,
	int trainerID = myPokemon->trainer_ID;
	int level = myPokemon->level;


	// remove this pokemon from all 3 trees,
	//change its level
	// add it to all 3 trees
	FreePokemon(pokemonID);

	CatchPokemon(pokemonID, trainerID, level + levelIncrease);

}


void DS_struct::GetTopPokemon(int trainerID, int *pokemonID){
	//if id== null or trainerID==o return invalid input
	if ( pokemonID==NULL || trainerID == 0){
		throw DS_struct::InvalidInput();
	}
	*pokemonID = -1; // deafult value
	smart_pointer<pokemon> topPokemon = NULL;

	// if id <0 get top pokemon id from pL_AVL
	if (trainerID < 0){
		try{ 
			topPokemon = pL_AVL.get_max(); 
		}
		catch (dataStructures::dataDoesNotExist&){
			;
		}
		catch (...){
			assert(0);
		}
	}

	//if id >0 get trainer ID
	// no trainer is an exception that will be caught
	// no max pokemon should result in the default value remaining in pokemonID pointer
	else if (trainerID>0){
		smart_pointer<trainer>  myTrainer = t_AVL.find(trainerID);
		try{
			topPokemon = myTrainer->tp_AVL.get_max();
		}
		catch (dataStructures::dataDoesNotExist&){
			;
		}
		catch (...){
			assert(0);
		}
	}

	//if exist get its top pokemon id
	*pokemonID = topPokemon->pokemon_ID;

}

void DS_struct::GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon){
	//if pointer = null or trainer id =0 return invalid input

	//if trainer id >0 and not exist return failure

	//get size of MAVL tree from either DS_struct or appropriate trainer

	// malloc an array of such ints. // if failed return allocation error
	// preform an postorder walk on the appropriate MAVL tree, for each pokemon put its id in the index

	//put number of pokemon in  pointer and the array of iDS_struct in the array pointer

	//return success
}
void DS_struct::EvolvePokemon(int pokemonID, int evolvedID){
	// if either id <=0 throw invalid input
	if (pokemonID <= 0 || evolvedID <= 0){
		throw InvalidInput();
	}

	//get pokemon with pokemon id, if not exist will throw failure

	smart_pointer<pokemon> myPokemon = p_AVL.find(pokemonID);

	//get its level and trainer id,
	int trainerID = myPokemon->trainer_ID;
	int level = myPokemon->level;

	try{// if evolved does not exist, then continue evolution in catch
		p_AVL.find(evolvedID);
	}
	catch (dataStructures::dataDoesNotExist&){
		// remove pokemon from all 3 trees,
		//catch new pokemon

		FreePokemon(pokemonID);
		CatchPokemon(evolvedID, trainerID, level);

		//return sucessfully
		return;

	}
	catch (...){
		assert(0);
	}
	// if evolved exists then we get here and throw failure
	throw Failure();

}



void DS_struct::UpdateLevels( int stoneCode, int stoneFactor){
	//if code or factor <1 return invalid input
	// make an array of trainers
	//for each trainer, preform 2 conditional walks of pokemons according to is stonde code function
	// destroying the tree after warDS_struct
	// so 4 arrays of total size 2n_trainer for each trainer
	// each 2 walks (for condition true and false) reconstruct to an avl tree

	// make the same conditional walks for the p_MAVL
	// for every poemon in true condition, multiply its level by stone factor.
	//then build back the p_mavl true and false conditions trees.

	// now for each false and true tree pair, merge the trees
	// and allocate them back to their place.


}

//*************************





