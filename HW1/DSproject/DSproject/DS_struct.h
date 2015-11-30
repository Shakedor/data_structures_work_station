#ifndef DS_struct_H
#define DS_struct_H
#include "pokemon.h"
#include "trainer.h"
#include "AVL.h"
#include "library1.h"
#include <new>

class DS_struct{
	AvlTree<int, trainer*, compareFuncInt> t_AVL;
	AvlTree<int, pokemon*, compareFuncInt> p_AVL;
	AvlTree<pokemonKey, pokemon*, compareFuncPokKey> pL_AVL;


public:
	DS_struct() :t_AVL(IDCompare), p_AVL(IDCompare), pL_AVL(pokemonLevelCompare){}
	void AddTrainer(int trainerID);
	void CatchPokemon(int pokemonID, int trainerID, int level);
	void FreePokemon(int pokemonID);
	void LevelUp(int pokemonID, int levelIncrease);
	void GetTopPokemon(int trainerID, int *pokemonID);
	void GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon);
	void EvolvePokemon(int pokemonID, int evolvedID);
	void UpdateLevels(int stoneCode, int stoneFactor);
	~DS_struct()= default;

	class DSException{};
	class InvalidInput : public DSException{};
	class Failure : public DSException{};

};

template<class data>
class treeSaver{
public:
	int size;
	int counter;
	data** dataArr;


	treeSaver(int size) :size(size),counter(0){
		dataArr = new data[size];
	}
	void operator() (data* newdata){
		if (counter <= size){
			throw(dataStructures::sizeOverFlow&);
		}
		else{
			dataArr[counter] = newdata;
		}
	}
	~treeSaver(){
		delete[] dataArr;
	}

};

class isStoneCode{
public:
	int stoneCode;
	bool retVal;

	isStoneCode(int code, bool val) : stoneCode(code), retVal(val){}
	
	bool operator() (pokemon* thisPokemon){
		if (thisPokemon->pokemon_ID%stoneCode == 0){
			return retVal;
		}
		return !retVal;
	}
};

#endif // !DS_struct_H
