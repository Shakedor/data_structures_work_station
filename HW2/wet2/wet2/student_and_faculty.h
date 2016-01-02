#ifndef STUDENT_FACULTY_
#define STUDENT_FACULTY_

#include "dataStructures.h"

static const int UNINITIALIZED_STUDY_GROUP = -1;


class Student{
public:
	int studentID;
	int average;
	int studyGroup;

	Student() : studyGroup(-1){}
};

class Faculty{
public:
	smart_pointer<Student> bestStudent;
	bool isFaculty;

	Faculty() : isFaculty(true), bestStudent(NULL){}
};

class StudyGroup{
public:
	int myFaculty;
	StudyGroup() : myFaculty(-1){}
};


int studentCompare(Student stud1, Student stud2);


class Student_Comparison{
public:
	int operator() (Student stud1, Student stud2){
		studentCompare(stud1, stud2);
	}
};



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
