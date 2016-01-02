#ifndef STUDENT_FACULTY_
#define STUDENT_FACULTY_

class Student{
public:
	int studentID;
	int average;
	int studyGroup;

	Student() :studyGroup(-1){}
};

class Faculty{
public:
	int bestStudentID;
	bool isFaculty;

	Faculty() :bestStudentID(-1), isFaculty(true){}
};

class StudyGroup{
public:
	int myFaculty;
	StudyGroup() :myFaculty(-1){}
};


int studentCompare(Student stud1, Student stud2){
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

class Student_Comparison{
public:
	int operator() (Student stud1, Student stud2){
		studentCompare(stud1, stud2);
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
