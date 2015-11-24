#ifndef DS_H
#define DS_H

typedef enum {
	SUCCESS, FAILURE, ALLOCATION_ERROR, INVALID_INPUT

}  StatusType;

class DS{


public:
	DS();
	StatusType AddTrainer(int trainerID);
	StatusType CatchPokemon(int pokemonID, int trainerID, int level);
	StatusType FreePokemon(int pokemonID);
	StatusType LevelUp(int pokemonID, int levelIncrease);
	StatusType GetTopPokemon(int trainerID, int *pokemonID);
	StatusType GetAllPokemonsByLevel(int trainerID, int **pokemons, int* numOfPokemon);
	StatusType EvolvePokemon(int pokemonID, int evolvedID);
	StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor);
	~DS();

};

void* Init();StatusType AddTrainer(void *DS, int trainerID);StatusType CatchPokemon(void *DS, int pokemonID, int trainerID, int level);StatusType FreePokemon(void *DS, int pokemonID);StatusType LevelUp(void *DS, int pokemonID, int levelIncrease);StatusType GetTopPokemon(void *DS, int trainerID, int *pokemonID);StatusType GetAllPokemonsByLevel(void *DS, int trainerID, int **pokemons, int* numOfPokemon);StatusType EvolvePokemon(void *DS, int pokemonID, int evolvedID);
StatusType UpdateLevels(void *DS, int stoneCode, int stoneFactor);
void Quit(void **DS);


#endif // !DS_H
