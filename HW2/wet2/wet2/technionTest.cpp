

#include "Technion_DS.h"

#include "testsMacros.h"
#include <iostream>




////////////////////////////////////////////////////////////////////////////////
///////		Assertions related to assumptions of AVL correctness	////////////
////////////////////////////////////////////////////////////////////////////////

#define ASSERT_DATA_ALREADY_EXISTS(Hash_NAME, key_value, data_value) do {	\
		try {	\
			Hash_NAME.insert(key_value, data_value);	\
		} catch (dataStructures::dataAlreadyExists&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_DATA_DOESNT_EXIST(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (dataStructures::dataDoesNotExist&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_HASH_IS_EMPTY(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (dataStructures::sturctIsEmpty&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_EXCEPTION_THROWN(command, exception) do {	\
		try {	\
			command;	\
		} catch (exception&){ \
			; \
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

#define ASSERT_NO_EXCEPTION_THROWN(command) do {	\
		try {	\
			command;	\
		}catch (...) { \
			ASSERT_TEST (false); \
		} \
} while (0)

////////////////////////////////////////////////////////////////////////////////
///////					Insert/Remove Assertions					////////////
////////////////////////////////////////////////////////////////////////////////

#define INSERT(Hash_NAME, key_value, data_value) do {	\
		try {	\
			Hash_NAME.insert(key_value, data_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "+++++++ key,data = (" << key_value << "," << data_value << ") was inserted: +++++++" << std::endl; \
		ASSERT_DATA_ALREADY_EXISTS(Hash_NAME, key_value, data_value); \
} while (0)

#define REMOVE(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "------- key = " << key_value << " was removed: -------" << std::endl;	\
		ASSERT_DATA_DOESNT_EXIST(Hash_NAME, key_value); \
} while (0)

#define REMOVE_LAST(Hash_NAME, key_value) do {	\
		try {	\
			Hash_NAME.remove(key_value);	\
		} catch (...) { \
			ASSERT_TEST (false); \
		} \
		std::cout << "------- key = " << key_value << " was removed: -------" << std::endl;	\
		ASSERT_HASH_IS_EMPTY(Hash_NAME, key_value); \
} while (0)

////////////////////////////////////////////////////////////////////////////////
///////			function objects needed for hashTable				////////////
////////////////////////////////////////////////////////////////////////////////

template<class T>
class Compare {
public:
	int operator() (const T& a, const T& b) const{
		if (a < b){
			return -1;
		} else if ( b < a){
			return 1;
		} else {
			return 0;
		}
	}
};

class intHasher {
	int modulu;
public:
	intHasher() : modulu(10){}
	intHasher(const intHasher& hasher) : modulu(hasher.modulu){}

	int operator() (int x){
		return x%modulu;
	}
	void setRange(int newRange){
		modulu = newRange;
	}
};

////////////////////////////////////////////////////////////////////////////////
///////				Essential variables for every test				////////////
////////////////////////////////////////////////////////////////////////////////

#define NUMBER_OF_STUDY_GROUPS 10
#define NUMBER_OF_STUDENTS 10

#define ESSENTIAL_VARIABLES()	\
		Technion technion(NUMBER_OF_STUDY_GROUPS); \
		std::cout << std::endl; \

#define ADD_STUDENTS()  do { \
	for (int i=0 ; i<NUMBER_OF_STUDENTS+1 ; ++i){ \
		ASSERT_NO_EXCEPTION_THROWN(technion.AddStudent(i, 10*i)); \
	} \
} while(0)

////////////////////////////////////////////////////////////////////////////////
///////							Unit tests							////////////
////////////////////////////////////////////////////////////////////////////////

using namespace dataStructures;

static bool testTechnionCreate(){
	bool result = true;

	// Bad input
	ASSERT_EXCEPTION_THROWN(Technion technion(-1), sizeOverFlow);
	ASSERT_EXCEPTION_THROWN(Technion technion(0), sizeOverFlow);
	ASSERT_EXCEPTION_THROWN(Technion technion(1), sizeOverFlow);

	ASSERT_NO_EXCEPTION_THROWN(Technion technion(2));

	return result;
}

static bool testTechnionAddStudent(){
	bool result = true;

	ESSENTIAL_VARIABLES();

	//Bad parameters
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(5, -1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(5, 101), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(-1, 70), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(-10, -20), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(10, 101), badParameter);

	// Valid parameters
	ASSERT_NO_EXCEPTION_THROWN(technion.AddStudent(10, 20));
	ASSERT_NO_EXCEPTION_THROWN(technion.AddStudent(1, 100));
	ASSERT_NO_EXCEPTION_THROWN(technion.AddStudent(2, 0));

	// Check adding a student that his ID already exists
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(2, 0), dataAlreadyExists);
	ASSERT_EXCEPTION_THROWN(technion.AddStudent(2, 10), dataAlreadyExists);

	return result;
}

static bool testTechnionAssignStudent(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	ADD_STUDENTS();

	// Study group does not exist
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(1, -1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(1, NUMBER_OF_STUDY_GROUPS), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(1, NUMBER_OF_STUDY_GROUPS+1), badParameter);

	// Bad student ID
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(-1, NUMBER_OF_STUDY_GROUPS-1), badParameter);

	// Bad student ID and study group does not exist
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(-1, NUMBER_OF_STUDY_GROUPS), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(-2, NUMBER_OF_STUDY_GROUPS+1), badParameter);

	// Valid student ID, but doesn't exist
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(NUMBER_OF_STUDENTS+1, NUMBER_OF_STUDY_GROUPS-1), dataDoesNotExist);

	// Successful adding
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(0, 0));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(1, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(2, 2));

	// Insert to the same study group
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(0, 0));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(1, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(2, 2));

	// Insert student to another study group
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(0, 2), dataAlreadyAssigned);
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(1, 0), dataAlreadyAssigned);
	ASSERT_EXCEPTION_THROWN(technion.AssignStudent(2, 1), dataAlreadyAssigned);

	// Insert students to not empty study groups
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(3, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(4, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(5, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(6, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(7, 1));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(8, 2));
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(9, 2));

	return result;
}

static bool testTechnionJoinFaculties(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	ADD_STUDENTS();

	// Bad faculty number
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(0, NUMBER_OF_STUDY_GROUPS), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(0, NUMBER_OF_STUDY_GROUPS+1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(-1, 0), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(-2, NUMBER_OF_STUDY_GROUPS), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(NUMBER_OF_STUDY_GROUPS, 0), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(NUMBER_OF_STUDY_GROUPS+1, 0), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(0, -1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(NUMBER_OF_STUDY_GROUPS, -2), badParameter);

	// Join empty faculties
	ASSERT_NO_EXCEPTION_THROWN(technion.JoinFaculties(0, 1));

	// Join a faculty with itself
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(2, 2), dataAlreadyAssigned);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(0, 1), dataAlreadyAssigned);

	// Add students to a different study group
	for (int id=0 ; id<NUMBER_OF_STUDENTS/2 ; ++id){
		technion.AssignStudent(id, 1);
	}
	for (int id=NUMBER_OF_STUDENTS/2 ; id<NUMBER_OF_STUDENTS ; ++id){
		technion.AssignStudent(id, 2);
	}

	//Join non empty faculties
	ASSERT_NO_EXCEPTION_THROWN(technion.JoinFaculties(0, 2));

	// 0 and 2 already have been joined
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(0, 2), dataAlreadyAssigned);
	ASSERT_EXCEPTION_THROWN(technion.JoinFaculties(2, 0), dataAlreadyAssigned);

	return result;
}

static bool testTechnionGetFaculty(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	ADD_STUDENTS();
	int faculty;

	// Bad parameters: NULL ptr, Bad student_ID
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(0, NULL), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(-1, &faculty), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(-2, &faculty), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(-1, NULL), badParameter);

	// student_ID does not exist
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(NUMBER_OF_STUDENTS, &faculty), dataDoesNotExist);
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(NUMBER_OF_STUDENTS+1, &faculty), dataDoesNotExist);

	// student_ID is not assigned to a faculty
	ASSERT_EXCEPTION_THROWN(technion.GetFaculty(0, &faculty), dataDoesNotExist);

	// Add the rest of students to study groups
	for (int id=0 ; id<NUMBER_OF_STUDENTS ; ++id){
		technion.AssignStudent(id, id);
	}

	// Check singleton faculties return correct value
	for (int id=0 ; id<NUMBER_OF_STUDENTS ; ++id){
		ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(id, &faculty));
		ASSERT_TEST(faculty == id);
	}

	// Join faculties
	for (int i=0 ; i< NUMBER_OF_STUDENTS-1 ; i+=2){
		technion.JoinFaculties(i, i+1);
	}

	// Check singleton faculties return correct value
	for (int id=0 ; id<NUMBER_OF_STUDENTS-1 ; id+=2){
		ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(id, &faculty));
		ASSERT_TEST(faculty == id);
		ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(id+1, &faculty));
		ASSERT_TEST(faculty == id);
	}

	// Join faculties 0,1,2,3
	ASSERT_NO_EXCEPTION_THROWN(technion.JoinFaculties(3, 0));

	// Check 0,1,2,3 are in faculty 2
	for (int id=0 ; id<4 ; ++id){
		ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(id, &faculty));
		ASSERT_TEST(faculty == 2);
	}

	// Add new students to technion
	for (int i=1 ; i<5 ; ++i){
		ASSERT_NO_EXCEPTION_THROWN(technion.AddStudent(11*i, 66));
		ASSERT_EXCEPTION_THROWN(technion.GetFaculty(11*i, &faculty), dataDoesNotExist);
	}

	// Add students to study groups 0-3
	technion.AssignStudent(11, 1);
	technion.AssignStudent(22, 2);
	technion.AssignStudent(33, 3);
	technion.AssignStudent(44, 0);

	// Check student is in faculty 2
	for (int i=1 ; i<5 ; ++i){
		ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(11*i, &faculty));
		ASSERT_TEST(faculty == 2);
	}

	return result;
}

static bool testTechnionUnifyFacultiesByStudents(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	ADD_STUDENTS();
	int faculty;

	// Bad parameters: Bad student_ID
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, -1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(-1, -1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(1, -1), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(-1, 0), badParameter);

	// student_ID does not exist
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, NUMBER_OF_STUDENTS+1), dataDoesNotExist);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(NUMBER_OF_STUDENTS+2, 1), dataDoesNotExist);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(NUMBER_OF_STUDENTS+4, NUMBER_OF_STUDENTS+1), dataDoesNotExist);

	// student_ID is not assigned to a faculty
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, 1), dataDoesNotExist);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(1, 0), dataDoesNotExist);

	// Add student 0 to faculty 0
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(0, 1));

	// student 1 is not assigned to a faculty
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, 1), dataDoesNotExist);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(1, 0), dataDoesNotExist);

	// Add the rest of students to study groups
	for (int id=0 ; id<NUMBER_OF_STUDENTS-1 ; ++id){
		technion.AssignStudent(id, id+1);
	}

	// Unify faculties 1 and 2
	ASSERT_NO_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, 1));
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, 1), dataAlreadyAssigned);
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(1, 0), dataAlreadyAssigned);

	// Check student 0 (group 1) and student 1 (group 2) are in faculty 1
	ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(0, &faculty));
	ASSERT_TEST(faculty == 1);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(1, &faculty));
	ASSERT_TEST(faculty == 1);

	// Add student 2 to faculty 1
	ASSERT_NO_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(0, 2));
	ASSERT_NO_EXCEPTION_THROWN(technion.GetFaculty(2, &faculty));
	ASSERT_TEST(faculty == 1);

	// same faculty
	ASSERT_EXCEPTION_THROWN(technion.UnifyFacultiesByStudents(3, 3), dataAlreadyAssigned);

	return result;
}

static bool testTechnionUpgradeStudyGroup(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	ADD_STUDENTS();
	int smartestID;

	// Bad parameters: study group does not exist, or factor<1
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(-1, 2), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(-1, 0), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(1, -2), badParameter);

	// Add students 0-2 to study group 1
	for (int id=0 ; id<3 ; ++id){
		technion.AssignStudent(id, 1);
		ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
		ASSERT_TEST(smartestID == id);
	}

	// Add students 3-4 to study group 2
	for (int id=3 ; id<5 ; ++id){
		technion.AssignStudent(id, 2);
		ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(2, &smartestID));
		ASSERT_TEST(smartestID == id);
	}

	// upgradeStudyGroup 1 and check correct max (30,40)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(2, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(2, &smartestID));
	ASSERT_TEST(smartestID == 4);

	// upgradeStudyGroup 1 and check correct max (60,80)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(2, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(2, &smartestID));
	ASSERT_TEST(smartestID == 4);

	// upgradeStudyGroup 1 and check correct max (100,100)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(2, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(2, &smartestID));
	ASSERT_TEST(smartestID == 3);

	// Join faculties 1 and 2
	ASSERT_NO_EXCEPTION_THROWN(technion.JoinFaculties(1, 2));

	// upgradeStudyGroup 1 and check correct max (0,20,40)  (100,100)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(1, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
	ASSERT_TEST(smartestID == 3);

	// upgradeStudyGroup 1 and check correct max (0,40,80)  (100,100)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(1, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
	ASSERT_TEST(smartestID == 3);

	// upgradeStudyGroup 1 and check correct max (0,80,100)  (100,100)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(1, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
	ASSERT_TEST(smartestID == 2);

	// upgradeStudyGroup 1 and check correct max (0,100,100)  (100,100)
	ASSERT_EXCEPTION_THROWN(technion.UpgradeStudyGroup(1, 2), badParameter);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
	ASSERT_TEST(smartestID == 1);


	return result;
}

static bool testTechnionGetSmartestStudent(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	ADD_STUDENTS();
	int smartestID;

	// Bad parameters: study group does not exist, or NULL pointer
	ASSERT_EXCEPTION_THROWN(technion.GetSmartestStudent(-1, NULL), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetSmartestStudent(1, NULL), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetSmartestStudent(-2, &smartestID), badParameter);

	// Empty faculty
	ASSERT_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID), dataDoesNotExist);

	// Add even studentsID 0,2,4,6,8 to study group 1 and check smartest
	for (int id=0 ; id<NUMBER_OF_STUDENTS ; id+=2){
		technion.AssignStudent(id, 1);
		ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
		ASSERT_TEST(smartestID == id);
	}

	// Add odd studentsID 7,5,3,1 to study group 2 and check smartest
	for (int id=7 ; id>=0 ; id-=2){
		technion.AssignStudent(id, 2);
		ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(2, &smartestID));
		ASSERT_TEST(smartestID == 7);
	}

	// Join faculties 1 and 2
	ASSERT_NO_EXCEPTION_THROWN(technion.JoinFaculties(1, 2));

	// Faculty 2 does not exist anymore
	ASSERT_EXCEPTION_THROWN(technion.GetSmartestStudent(2, &smartestID), dataDoesNotExist);

	// Check smartes student is 7
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
	ASSERT_TEST(smartestID == 7);

	//Add studentId 9 to studyGroup 2
	ASSERT_NO_EXCEPTION_THROWN(technion.AssignStudent(9, 2));

	// Check smartes student is 9
	ASSERT_NO_EXCEPTION_THROWN(technion.GetSmartestStudent(1, &smartestID));
	ASSERT_TEST(smartestID == 9);

	return result;
}

static bool testTechnionGetNumOfStudentsInRange(){
	bool result = true;

	ESSENTIAL_VARIABLES();
	int studentsInRange;

	// Bad parameters: study group does not exist, or NULL pointer
	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(10, 20, NULL), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(10, 10, &studentsInRange), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(11, 10, &studentsInRange), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(-1, 10, &studentsInRange), badParameter);
	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(11, 101, &studentsInRange), badParameter);

	// Empty Technion
	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(0, 100, &studentsInRange), badParameter);
	ASSERT_TEST(studentsInRange == 0);

	// Add even studentsID 0, to Technion
	for (int id=0 ; id<NUMBER_OF_STUDENTS ; ++id){
		technion.AddStudent(id, 10*id);

		//Num of students
		ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(0, 100, &studentsInRange), badParameter);
		ASSERT_TEST(studentsInRange == id);
	}

	for (int id=0; id<NUMBER_OF_STUDENTS ; ++id){
		ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(0, 10*id, &studentsInRange), badParameter);
		ASSERT_TEST(studentsInRange == id);
	}

	// Add students to study groups
	for (int id=0; id<NUMBER_OF_STUDENTS ; ++id){
		technion.AssignStudent(id, id);
	}

	ASSERT_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(0, 100, &studentsInRange), badParameter);
	ASSERT_TEST(studentsInRange == NUMBER_OF_STUDENTS);

	// studentID 1 avg=20
	ASSERT_NO_EXCEPTION_THROWN(technion.UpgradeStudyGroup(1, 2));
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(9,11, &studentsInRange));
	ASSERT_TEST(studentsInRange == 0);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(19,21, &studentsInRange));
	ASSERT_TEST(studentsInRange == 2);

	// studentID 2 avg=40
	ASSERT_NO_EXCEPTION_THROWN(technion.UpgradeStudyGroup(2, 2));
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(19,21, &studentsInRange));
	ASSERT_TEST(studentsInRange == 1);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(39,41, &studentsInRange));
	ASSERT_TEST(studentsInRange == 2);

	// studentID 6,7 avg=100
	//Before
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(59,61, &studentsInRange));
	ASSERT_TEST(studentsInRange == 1);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(69,71, &studentsInRange));
	ASSERT_TEST(studentsInRange == 1);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(99,100, &studentsInRange));
	ASSERT_TEST(studentsInRange == 0);
	// Do update
	ASSERT_NO_EXCEPTION_THROWN(technion.UpgradeStudyGroup(6, 2));
	ASSERT_NO_EXCEPTION_THROWN(technion.UpgradeStudyGroup(7, 2));
	//After
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(59,61, &studentsInRange));
	ASSERT_TEST(studentsInRange == 0);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(69,71, &studentsInRange));
	ASSERT_TEST(studentsInRange == 0);
	ASSERT_NO_EXCEPTION_THROWN(technion.GetNumOfStudentsInRange(99,100, &studentsInRange));
	ASSERT_TEST(studentsInRange == 2);

	return result;
}


int main() {
	RUN_TEST(testTechnionCreate);
	RUN_TEST(testTechnionAddStudent);
	RUN_TEST(testTechnionAssignStudent);
	RUN_TEST(testTechnionJoinFaculties);
	RUN_TEST(testTechnionGetFaculty);
	RUN_TEST(testTechnionUnifyFacultiesByStudents);
	RUN_TEST(testTechnionUpgradeStudyGroup);
	RUN_TEST(testTechnionGetSmartestStudent);
	RUN_TEST(testTechnionGetNumOfStudentsInRange);

	printf("Testing completed successfully.\n");
	return 0;

}



