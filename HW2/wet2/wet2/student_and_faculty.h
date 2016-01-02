#ifndef STUDENT_FACULTY_
#define STUDENT_FACULTY_

#include "dataStructures.h"

static const int UNINITIALIZED_STUDENT_ID = -1;
static const int UNINITIALIZED_AVERAGE = -1;
static const int UNINITIALIZED_STUDY_GROUP = -1;
static const int UNINITIALIZED_FACULTY_NUMBER = -1;

using namespace dataStructures;

class Student{
public:
	int studentID;
	int average;
	int studyGroup;

	Student() : studentID(UNINITIALIZED_STUDENT_ID), average(UNINITIALIZED_AVERAGE),
				studyGroup(UNINITIALIZED_STUDY_GROUP){}
};

class Faculty{
public:
	smart_pointer<Student> bestStudent;
	bool isFaculty;

	Faculty() : isFaculty(true){} // Argument-less constructor of smart pointer sets it to NULL
};

class StudyGroup{
public:
	int myFaculty;
	StudyGroup() : myFaculty(UNINITIALIZED_FACULTY_NUMBER){}
};

int studentCompare(Student stud1, Student stud2);

class idCompare{
public:
	int operator () (const int a,const int b)const{
		if (a > b){
			return 1;
		}
		else if(a< b){
			return -1;
		}
		else{
			return 0;
		}
	}
};

class studentHasher {
	int modulu;
public:
	static const int startingModulu = 10;
	studentHasher() : modulu(startingModulu){}
	studentHasher(const studentHasher& hasher) : modulu(hasher.modulu){}

	int operator() (int x){
		return x%modulu;
	}
	void setRange(int newRange){
		modulu = newRange;
	}
};

#endif // !STUDENT_FACULTY_
