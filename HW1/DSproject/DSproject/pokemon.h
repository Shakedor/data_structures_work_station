#ifndef POK_H
#define POK_H
#include <iostream>

typedef enum {
	POK_SUCCESS, POK_FAIL, POK_MEM_ERR
}  pokRES;




class pokemon{

	 
public:
	pokemon(int pokID, int trID, int level) : trainer_ID(trID), pokemon_ID(pokID), level(level) {};
	int trainer_ID;
	int pokemon_ID;
	int level;
	~pokemon(){};
	void print(){
		printf("(%d,%d,%d)", level, pokemon_ID, trainer_ID);
	}
	friend std::ostream& operator <<(std::ostream& os, pokemon& pok);
};

class pokemonKey{
	public:
	int pokemon_ID;
	int level;

	pokemonKey(int pokID, int level) :pokemon_ID(pokID), level(level){};
	friend std::ostream& operator <<(std::ostream& os, pokemonKey& pokKey);
};



int IDCompare(int a, int b);

class pokemonKeyPrint{
	
public:
	pokemonKeyPrint(){}
	void operator() (pokemonKey& key){
		std::cout << '(' << key.level << ',' << key.pokemon_ID << ')';
	}
};

class pokemonPrint{
public:
	pokemonPrint(){}
	void operator() (pokemon& pok){
		std::cout << '(' << pok.level << ',' << pok.pokemon_ID << ','<< pok.trainer_ID<<')';
	}
};




int pokemonLevelCompare(pokemonKey a, pokemonKey b);

typedef int(*compareFuncInt)(int, int);

typedef int(*compareFuncPokKey)(pokemonKey, pokemonKey);





#endif // !POK_H
