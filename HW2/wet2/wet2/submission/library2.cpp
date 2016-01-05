
#include "library2.h"
#include "technion_DS.h"
#include "dataStructures.h"
#include <cassert>

#define TRY_CATCH(x) do{try{x}\
	catch (dataStructures::invalidInputExceptions&){return INVALID_INPUT; }\
	catch (std::bad_alloc&){return ALLOCATION_ERROR;}\
	catch (dataStructures::failureExceptions&){return FAILURE; }\
	catch (...){assert(0);}\
	return SUCCESS;\
} while (0)

#define TRY_WITHOUT_RETURN(x) do{try{x}\
	catch (dataStructures::invalidInputExceptions&){}\
	catch (std::bad_alloc&){;}\
	catch (dataStructures::failureExceptions&){}\
	catch (...){assert(0);}\
} while (0)


void*       Init(int n){
	void* technion = NULL;
	TRY_WITHOUT_RETURN(technion = new Technion(n););
	return technion;
}


StatusType   AddStudent(void* DS, int studentID, int average){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->AddStudent(studentID, average););
}



StatusType   AssignStudent(void* DS, int studentID, int studyGroup){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->AssignStudent(studentID, studyGroup););
}



StatusType   JoinFaculties(void* DS, int studyGroup1, int studyGroup2){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->JoinFaculties(studyGroup1, studyGroup2););
}



StatusType  UnifyFacultiesByStudents(void* DS, int studentID1, int studentID2){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->UnifyFacultiesByStudents(studentID1, studentID2););
}


StatusType   GetFaculty(void* DS, int studentID, int* faculty){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->GetFaculty(studentID, faculty););
}


StatusType   UpgradeStudyGroup(void* DS, int studyGroup, int factor){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->UpgradeStudyGroup(studyGroup, factor););
}


StatusType   GetSmartestStudent(void* DS, int facultyID, int* student){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->GetSmartestStudent(facultyID, student););
}


StatusType   GetNumOfStudentsInRange(void* DS, int fromAvg, int toAvg, int* num){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->GetNumOfStudentsInRange(fromAvg, toAvg, num););
}


void        Quit(void** DS){
	Technion** ds = (Technion**)(DS);
	if (ds){
		delete *ds;
		*ds = NULL;
	}
}


