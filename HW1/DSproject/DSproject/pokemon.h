#ifndef POK_H
#define POK_H

typedef enum {
	POK_SUCCESS, POK_FAIL, POK_MEM_ERR
}  pokRES;

class pokemon{

	 
public:
	pokemon(int pokID, int trID, int level) : trainer_ID(trID), pokemon_ID(pokID), level(level) {};
	int trainer_ID;
	int pokemon_ID;
	int level;
	~pokemon() = default;
};


#endif // !POK_H
