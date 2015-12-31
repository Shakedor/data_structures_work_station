#ifndef TECHNION_DS_
#define TECHNION_DS_

#include <cstdlib>
#include <cassert>
#include "dataStructures.h"
#include <cmath>
#include "rankAvl.h"
#include "hashTable.h"
#include "UnionFind.h"
#include "library2.h"
#include "student_and_faculty.h"

class Technion{
	int bucket[101];
	AvlTree<int, Student, idCompare> studentTree;
	hashTable<int, Student, idCompare, studentHasher> studentHash;
	UnionFind<Faculty, studyGroup> facultySets;

	int getCorrectSet(int studyGroup);
public:
	Technion(int n);
	~Technion();
	void  AddStudent(int studentID, int average);
	void  AssignStudent(int studentID, int studyGroup);
	void  JoinFaculties(int studyGroup1, int studyGroup2);
	void  GetFaculty(int studentID, int* faculty);
	void  UnifyFacultiesByStudents(int studentID1, int studentID2);
	void  UpgradeStudyGroup(int studyGroup, int factor);
	void  GetSmartestStudent(int facultyID, int* student);
	void  GetNumOfStudentsInRange(int fromAvg, int toAvg, int* num);
};

#endif // !TECHNION_DS_
