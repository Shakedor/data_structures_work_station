
#include "Technion_DS.h"
using namespace dataStructures;

Technion::Technion(int n) : n(n),bucket(NULL), studentTree(idCompare()), 
	studentHash(studentHasher(), idCompare(), studentHasher().startingModulu),
	facultySets(n){
	//initialize bucket to zero
	bucket = new int[101];
	for (int i = 0; i < 101; ++i){
		bucket[i] = 0;
	}
	//make empty tree
	//make hashtable of a constant size (for example 10)
	//make the facultySets with size n
	//initialize faculty in studyGroups
	for (int i = 0; i < n; i++){
		facultySets.getMember(i).myFaculty = i;
	}
}

Technion::~Technion(){
	//call all destructors
	delete[](bucket);
}

void  Technion::AddStudent(int studentID, int average){
	//check valid conditions
	// average between 0 and 100 including
	// studentID not <0
	//student ID is not in student Tree already - checked in insertion to tree
	if (average < 0 || average > 100 || studentID < 0){
		throw badParameter();
	}

	//if all is good
	//create a sharedpointer to student
	//add it to tree
	//add it to hash
	//add 1 to bucket[counter]
	smart_pointer<Student> studentPtr(new Student);
	studentPtr->average = average;
	studentPtr->studentID = studentID;
	studentPtr->studyGroup = UNINITIALIZED_STUDY_GROUP;

	studentTree.insert(studentID, studentPtr);
	studentHash.insert(studentID, studentPtr);
	++bucket[average];
}

void  Technion::AssignStudent(int studentID, int studyGroup){
	//check params
	//study group out of bounds
	//bad student id num
	// student ID does not exist in hash - checked in tree.find
	if (studentID < 0 || studyGroup < 0 || studyGroup >= n){
		throw badParameter();
	}
	smart_pointer<Student> studentPtr = studentTree.find(studentID);

	//if student exist and valid group, change the student's group to studygroup param
	if (studentPtr->studyGroup == studyGroup){//same study group do nothing
		return;
	}
	if (studentPtr->studyGroup != UNINITIALIZED_STUDY_GROUP){
		throw dataAlreadyAssigned();
	}
	studentPtr->studyGroup = studyGroup;

	// go to the study groups faculty, get top student. update top student
	Faculty& myFaculty = facultySets.getSet(getCorrectSet(studyGroup));

	if (!myFaculty.bestStudent){//no Best student
		myFaculty.bestStudent = studentPtr;
	}
	else{
		if (studentCompare(*studentPtr, *myFaculty.bestStudent) ==1){
			myFaculty.bestStudent = studentPtr;
		}

	}

}

void  Technion::JoinFaculties(int studyGroup1, int studyGroup2){
	//check params
	//study groups in bounds
	if (studyGroup1 < 0 || studyGroup1 >= n || studyGroup2 < 0 || studyGroup2 >= n){
		throw badParameter();
	}
	// 2 study groups are not in same faculty
	int facultyNum1 = getCorrectSet(studyGroup1);
	int facultyNum2 = getCorrectSet(studyGroup2);

	if (facultyNum1 == facultyNum2){
		throw dataAlreadyAssigned();
	}


	//if all is valid
	//update max student

	//check studygroup2's faculty as not valid;
	Faculty& faculty2 = facultySets.getSet(facultyNum2);
	faculty2.isFaculty = false;

	//put in both roots the number of group1's faculty
	StudyGroup& root1 = facultySets.getMember(facultySets.findNum(studyGroup1));
	StudyGroup& root2 = facultySets.getMember(facultySets.findNum(studyGroup2));
	root1.myFaculty = root2.myFaculty = facultyNum1;

	//unionize study group 1 and study group 2
	facultySets.unionize(studyGroup1, studyGroup2);

}

void  Technion::GetFaculty(int studentID, int* faculty){
	//params
	// check student is not negative
	//faculty is not NULL
	if (faculty == NULL || studentID < 0){
		throw badParameter();
	}

	//if student deos not exist, search will throw dataDoesNotExist

	//if all is valid
	//get student from hash
	smart_pointer<Student> myStud = studentHash.search(studentID);
	//get his study group
	//if no study group throw data does not exist
	if (myStud->studyGroup == UNINITIALIZED_STUDY_GROUP){
		throw dataDoesNotExist();
	}


	//get its faculty number (remmeber the faculty number o=in the root group)
	int facultyNum = getCorrectSet(myStud->studyGroup);

	*faculty = facultyNum;
}

void  Technion::UnifyFacultiesByStudents(int studentID1, int studentID2){
	//params
	//student names out of bound
	if (studentID1 < 0 || studentID2 < 0){
		throw badParameter();
	}
	
	// if student dont exist then data does not exist will be thrown out by search
	smart_pointer<Student> stud1 = studentHash.search(studentID1);
	smart_pointer<Student> stud2 = studentHash.search(studentID2);
	//student exist but have no study groups
	if (stud1->studyGroup==-1 || stud2->studyGroup == -1){
		throw dataDoesNotExist();
	}

	//if all is good
	// call unify faculties by the study groups of the students
	JoinFaculties(stud1->studyGroup, stud2->studyGroup);

}


void  Technion::UpgradeStudyGroup(int studyGroup, int factor){
	//params
	//group is out of bound
	//factor is <1
	if (studyGroup < 0 || studyGroup >= n || factor < 1){
		throw badParameter();
	}

	//if all is good
	// create a student save functor that saves the shared pointer of every student 
	treeSaver<int, smart_pointer<Student>> studentArr(studentTree.get_size());
	studentTree.inorder(studentArr);

	smart_pointer<Student> smartest=smart_pointer<Student>();
	int sizeOfGroup = 0;

	//for every student in studyGroup;
	for (int i = 0; i < studentTree.get_size(); i++){
		smart_pointer<Student> currStud = *studentArr.dataArr[i];
		if (currStud->studyGroup != studyGroup){
			continue;
		}

		//reaching here means student is i studyGroup

		// decremenet student's average bucket
		bucket[currStud->average]--;
		//multiply students average by factor (or min 100)
		currStud->average *= factor;
		currStud->average =(100<currStud->average)?100:currStud->average; 
		//increment bucket with new average
		bucket[currStud->average]++;


		// save maximum student of studyGroup
		++sizeOfGroup;
		if (sizeOfGroup == 1){//first Student in study Group
			smartest = currStud;
		}
		else{// other student in studyGroup
			if (studentCompare(*currStud, *smartest) == 1){//curr student is bigger
				smartest = currStud;
			}
		}
	}

	if (sizeOfGroup != 0){//only update smartest if group had students
		//get faculty of study group (using getCorrectSet)
		Faculty& currFaculy = facultySets.getSet(getCorrectSet(studyGroup));
		//if the max updated student is smarter than old smartest student, update 
		//smartest student.
		if (!currFaculy.bestStudent){//no Best student
			currFaculy.bestStudent= smartest;
		}
		else{
			smart_pointer<Student> currSmartest = currFaculy.bestStudent;
			if (studentCompare(*smartest, *currSmartest) == 1){//smartest is smarter
				currFaculy.bestStudent= smartest;
			}
		}

	}


}

void  Technion::GetSmartestStudent(int facultyID, int* student){
	//params
	//is student NULL
	if (student == NULL){
		throw badParameter();
	}
	//is faculty ID out of bounds
	if (facultyID < 0 || facultyID >= n){
		throw badParameter();
	}

	//failure
	//is faculty valid (not a sub faculty)
	// is faculty empty (meaning no smartest student)
	Faculty& currFaculty = facultySets.getSet(facultyID);
	if (currFaculty.isFaculty == false || !currFaculty.bestStudent){
		throw dataDoesNotExist();
	}

	//if faculty is legit, get its smartest student ID
	*student = currFaculty.bestStudent->studentID;

}

void  Technion::GetNumOfStudentsInRange(int fromAvg, int toAvg, int* num){
	//params
	// int is not null
	//from is not equal or greater than to
	//from and to are not out of bounds
	if (num == NULL || fromAvg >= toAvg || fromAvg<0 || toAvg>100){
		throw badParameter();
	}

	//if all is valid
	//sum the counts of bucket[from] to bucket[to] including
	int sum = 0;
	for (int i = fromAvg; i < toAvg + 1; i++){
		sum += bucket[i];
	}

	*num = sum;
}


//note*** to get the faculty number, one must get the setnum 
//of a study group (represesnts the group's root)
// then ask said root what faculty he belongs to.
int Technion::getCorrectSet(int studyGroupNum){
	int root = facultySets.findNum(studyGroupNum);
	StudyGroup& rootGroup = facultySets.getMember(root);
	return rootGroup.myFaculty;
}