#ifndef POK_H
#define POK_H

typedef enum {
	POK_SUCCESS, POK_FAIL, POK_MEM_ERR
}  pokRES;

class pokemon{
	int trainer_ID;
	int pokemon_ID;
	int level;
	 
public:
	pokemon(int pokID, int trID, int level) : trainer_ID(trID), pokemon_ID(pokID), level(level) {};
	int getID() { return pokemon_ID; };
	int getTrainerID() { return trainer_ID; };
	int getLevel() { return level; };
	pokRES setLevel(int x){ level = x; return POK_SUCCESS; };

};


#endif // !POK_H
