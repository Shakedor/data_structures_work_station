#ifndef STUDENT_FACULTY_
#define STUDENT_FACULTY_

class Student{
public:
	int studentID;
	int average;
	int studyGroup
};

class Faculty{
public:
	Student bestStudent;
};

class studyGroup{
public:
	int myFaculty;
	studyGroup() :myFaculty(-1){}
};

class Student_Comparison{
public:
	int operator() (Student stud1, Student stud2){
		if (stud1.average > stud2.average){
			return 1;
		}
		else if (stud1.average < stud2.average){
			return -1;
		}
		else{
			if (stud1.studentID < stud2.studentID){
				return 1;
			}
			else if (stud1.studentID>stud2.studentID){
				return -1;
			}
			else{
				return 0;
			}
		}
	}
};

class idCompare{
public:
	int operator () (int a, int b){
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

class studentHasher{

};

#endif // !STUDENT_FACULTY_
