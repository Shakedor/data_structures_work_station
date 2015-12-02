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
	bool pokemonExistFlag = true;

	// if id <0 get top pokemon id from pL_AVL
	if (trainerID < 0){
		try{ 
			topPokemon = pL_AVL.get_max(); 
		}
		catch (dataStructures::dataDoesNotExist&){
			pokemonExistFlag = false;
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
			pokemonExistFlag = false;
		}
		catch (...){
			assert(0);
		}
	}

	//if exist get its top pokemon id
	if (pokemonExistFlag){
		*pokemonID = topPokemon->pokemon_ID;
	}
	

}

void DS_struct::GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon){
	//if pointer = null or trainer id =0 return invalid input
	if (numOfPokemon == NULL || pokemons == NULL || trainerID == 0){
		throw DS_struct::InvalidInput();
	}
	AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey>* correctTree= &pL_AVL;// will contain the whole DS pokemon tree unless trainer ID >0 and is found
	smart_pointer<trainer> myTrainer;
	
	//if trainer id >0 and not exist throw failure 
	if (trainerID > 0){
		try{
			myTrainer=t_AVL.find(trainerID);
			correctTree = &(myTrainer->tp_AVL);
		}
		catch (dataStructures::dataDoesNotExist&){
			throw dataStructures::failureExceptions();
		}
		catch(...){
			assert(0);
		}
	}

	//get size of MAVL tree from either DS_struct or appropriate trainer
	int treeSize = correctTree->get_size();
	// preform an postorder walk on the appropriate MAVL tree, saving each pokemon to an arrray 
	treeSaver<pokemonKey,smart_pointer<pokemon>> pokemonArr(treeSize);
	correctTree->inorder(pokemonArr);

	// malloc an array of such ints. // if failed return allocation error
	int* IDArr =(int*)malloc(sizeof(int)*treeSize);
	if (IDArr == NULL){
		throw std::bad_alloc();
	}

	for (int i = 0; i < pokemonArr.size; i++){
		smart_pointer<pokemon> currpokemon = *(pokemonArr.dataArr[i]);
		IDArr[i] = currpokemon->pokemon_ID;
	}

	//put number of pokemon in  pointer and the array of iDS_struct in the array pointer
	*numOfPokemon = treeSize;
	*pokemons = IDArr;
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
	if (stoneCode <= 1 || stoneFactor <= 1){
		throw InvalidInput();
	}

	//make array of trainers
	int trainerNum=t_AVL.get_size();
	treeSaver<int,smart_pointer<trainer>> trainerArr=treeSaver<int,smart_pointer<trainer>>(trainerNum);
	t_AVL.postorder(trainerArr);
	
	isStoneCode trueStone = isStoneCode(stoneCode, true);
	isStoneCode falseStone = isStoneCode(stoneCode, false);
	stoneCodeKeyUpdate keyUpdate = stoneCodeKeyUpdate(stoneFactor);
	stoneCodePokemonUpdate pokemonUpdate = stoneCodePokemonUpdate(stoneFactor);

	// for each trainer
	for (int i = 0; i < trainerNum; i++){
		smart_pointer<trainer> currentTrainer = *trainerArr.dataArr[i];
		// copy its pokemon tree twice
		AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey> posTree(currentTrainer->tp_AVL);
		AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey> negTree(currentTrainer->tp_AVL);

		//filter trees based on stone code / not sotne code

		posTree.removeIf(falseStone);
		negTree.removeIf(trueStone);

		// do stone code update operation on each key
		posTree.postorder(keyUpdate);
		// merge trees
		// store them as trainer's tree
		currentTrainer->tp_AVL = AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey>(posTree, negTree);

	}


	//then do the same for the big pl_AVL but also change the pokemon

	// copy its pokemon tree twice
	AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey> posTree(pL_AVL);
	AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey> negTree(pL_AVL);

	//filter trees based on stone code / not sotne code

	posTree.removeIf(falseStone);
	negTree.removeIf(trueStone);

	// do stone code update operation on each key ***and DATA only this one time***
	posTree.postorder(keyUpdate);
	posTree.postorder(pokemonUpdate);
	// merge trees
	// store them as trainer's tree
	pL_AVL =AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey>(posTree, negTree);


}

//*************************





