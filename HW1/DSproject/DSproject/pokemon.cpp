#include "pokemon.h"

int IDCompare(int a, int b){
	if (a > b){
		return 1;
	}
	else if (a < b){
		return -1;
	}
	else{
		return 0;
	}
}

int pokemonLevelCompare(pokemonKey a, pokemonKey b){
	if (a.level > b.level){
		return 1;
	}
	else if (a.level < b.level){
		return -1;
	}
	else{
		if (a.pokemon_ID < b.pokemon_ID){
			return 1;
		}
		else if (a.pokemon_ID > b.pokemon_ID){
			return -1;
		}
		else{
			return 0;
		}
	}
}

std::ostream& operator <<(std::ostream& os, pokemon& pok){
	os << '(' << pok.level << ',' << pok.pokemon_ID << ',' << pok.trainer_ID << ')';
	return os;
}

std::ostream& operator <<(std::ostream& os, pokemonKey& pokKey){
	os << '(' << pokKey.level << ',' << pokKey.pokemon_ID << ')';
	return os;
}