
#include "Technion_DS.h"

Technion::Technion(int n){
	//initialize bucket to zero
	//make empty tree
	//make hashtable of a constant size (for example 10)
	//make the facultySets with size n
}

Technion::~Technion(){
	//call all destructors

}

void  Technion::AddStudent(int studentID, int average){
	//check valid conditions
	// average between 0 and 100 including
	// studentID not <0
	//student ID is not in student Tree already

	//if all is good
	//create a sharedpointer to student
	//add it to tree
	//add it to hash
	//add 1 to bucket[counter]


}
void  Technion::AssignStudent(int studentID, int studyGroup){
	//check params
	//study group out of bounds
	//bad student id num
	// student ID does not exist in hash

	//if student exist and valid group, change the student's groupt to studygroup param
	// go to the study groups faculty, get top student. update top student
}
void  Technion::JoinFaculties(int studyGroup1, int studyGroup2){
	//check params
	//study groups in bounds
	// 2 study groups are not in same faculty

	//note*** to get the faculty number, one must get the setnum 
	//of a study group (represesnts the group's root)
	// then ask said root what faculty he belongs to.

	//if all is valid
	//unionize study group 1 and study group 2
	//put in both roots the number of group1's faculty
}
void  Technion::GetFaculty(int studentID, int* faculty){
	//params
	// check student is not negative
	//faculty is not NULL
	//student exist in studetn hash

	//if all is valid
	//get student from hash
	//get his study group

	//get its faculty number (remmeber the faculty number o=in the root group)

}
void  Technion::UnifyFacultiesByStudents(int studentID1, int studentID2){
	//params
	//student names out of bound
	//student name dont exist
	//student exist but have no study groups

	//if all is good
	// call unify faculties by the study groups of the students

}
void  Technion::UpgradeStudyGroup(int studyGroup, int factor){
	//params
	//group is out of bound
	//factor is <1

	//if all is good
	// create a student save functor that saves the shared pointer of every student that
	//is in study group

	//for every student in functor
	// decremenet student's average bucket
	//multiply students average by factor (or min 100)
	//increment bucket with new average
	// save maximum student in all of for loop

	//get faculty of study group (note special usage mentioned above)
	//if the max updated student is ssmarted than old smartest student, update 
	//smartest student.

}
void  Technion::GetSmartestStudent(int facultyID, int* student){
	//params
	//is student NULL
	//is faculty ID out of bounds

	//failure
	//is faculty valid (not a sub faculty)
	// is faculty empty (meaning no smartest student)

	//if faculty is legit, get its smartest student ID
}
void  Technion::GetNumOfStudentsInRange(int fromAvg, int toAvg, int* num){
	//params
	// int is not null
	//from is not equal or greater than to
	//from and to are not out of bounds

	//if all is valid
	//sum the counts of bucket[from] to bucket[to] including

}
int Technion::getCorrectSet(int studyGroupNum){
	int root = facultySets.findNum(studyGroupNum);
	studyGroup& rootGroup = facultySets.getMember(root);
	return rootGroup.myFaculty;
}