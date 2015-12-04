#ifndef DS_struct_H
#define DS_struct_H
#include "pokemon.h"
#include "trainer.h"
#include "AVL.h"
#include "library1.h"
#include <new>
#include "dataStructures.h"

using namespace dataStructures;

class DS_struct{
	AvlTree<int,smart_pointer<trainer>, compareFuncInt> t_AVL;
	AvlTree<int, smart_pointer<pokemon>, compareFuncInt> p_AVL;
	AvlTree<pokemonKey, smart_pointer<pokemon>, compareFuncPokKey> pL_AVL;

	bool doDebug; // TODO: remove
public:
	DS_struct() :t_AVL(IDCompare), p_AVL(IDCompare), pL_AVL(pokemonLevelCompare), doDebug(true){}
	void AddTrainer(int trainerID);
	void CatchPokemon(int pokemonID, int trainerID, int level);
	void FreePokemon(int pokemonID);
	void LevelUp(int pokemonID, int levelIncrease);
	void GetTopPokemon(int trainerID, int *pokemonID);
	void GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon);
	void EvolvePokemon(int pokemonID, int evolvedID);
	void UpdateLevels(int stoneCode, int stoneFactor);
	~DS_struct(){}

	class DSException{};
	class InvalidInput : public DSException{};
	class Failure : public DSException{};

	void debug() const; // TODO: remove
};

template<class key,class data>
class treeSaver{
public:
	int size;
	int counter;
	data** dataArr;
	key** keyArr;


	treeSaver(int len) :size(len),counter(0){
		dataArr = new data*[size];
		keyArr = new key*[size];
	}
	void operator() (key& newKey,data& newdata){
		assert(&newKey && &newdata);
		if (counter >= size){
			throw dataStructures::sizeOverFlow();
		}
		else{
			dataArr[counter] = &newdata;
			keyArr[counter] = &newKey;
		}
		counter++;
	}
	~treeSaver(){
		delete[] dataArr;
		delete[] keyArr;
	}

};

class isStoneCode{
public:
	int stoneCode;
	bool retVal;

	isStoneCode(int code, bool val) : stoneCode(code), retVal(val){}
	
	bool operator() (pokemonKey& key, smart_pointer<pokemon>& thisPokemon) const{
		if ((thisPokemon->pokemon_ID)%stoneCode == 0){
			return retVal;
		}
		return !retVal;
	}
};

class stoneCodeKeyUpdate{
public:
	int stoneFactor;

	stoneCodeKeyUpdate(int factor) :stoneFactor(factor){}

	void operator() (pokemonKey& key, smart_pointer<pokemon>& pokData){
		key.level *= stoneFactor;
	}

};

class stoneCodePokemonUpdate{
public:
	int stoneFactor;

	stoneCodePokemonUpdate(int factor) :stoneFactor(factor){}

	void operator() (pokemonKey& key, smart_pointer<pokemon>& pokData){
		pokData->level *= stoneFactor;
	}

};

#endif // !DS_struct_H
