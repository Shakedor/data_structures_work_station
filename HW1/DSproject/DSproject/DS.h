#ifndef DS_H
#define DS_H
#include "pokemon.h"
#include "trainer.h"
#include "AVL.h"
#include "library1.h"
#include <new>

class DS{
	AvlTree<int, trainer, compareFuncInt> t_AVL;
	AvlTree<int, pokemon, compareFuncInt> p_AVL;
	AvlTree<pokemonKey, pokemon*, compareFuncPokKey> pL_AVL;


public:
	DS() :t_AVL(IDCompare), p_AVL(IDCompare), pL_AVL(pokemonLevelCompare){};
	void AddTrainer(int trainerID);
	void CatchPokemon(int pokemonID, int trainerID, int level);
	void FreePokemon(int pokemonID);
	void LevelUp(int pokemonID, int levelIncrease);
	void GetTopPokemon(int trainerID, int *pokemonID);
	void GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon);
	void EvolvePokemon(int pokemonID, int evolvedID);
	void UpdateLevels(void *DS, int stoneCode, int stoneFactor);
	~DS()= default;

	class DSException{};
	class InvalidInput : public DSException{};
	class Failure : public DSException{};

};


#endif // !DS_H
