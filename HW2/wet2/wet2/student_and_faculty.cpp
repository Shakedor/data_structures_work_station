#include "student_and_faculty.h"

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