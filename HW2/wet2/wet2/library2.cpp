
#include "library2.h"
#include "Technion_DS.h"
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

/* Description:   Initializes the Technion's Data Structure with n study groups (that are initially identical to faculties).
 * Input:         n - Number of study groups.
 * Output:        None.
 * Return Values: A pointer to a new instance of the data structure - as a void* pointer.
 */
void*       Init(int n){
	void* technion = NULL;
	TRY_WITHOUT_RETURN(technion = new Technion(n););
	return technion;
}


/* Description:   A student was added to the system.
 * Input:         DS - A pointer to the data structure.
 *                studentID - The ID of the student.
 *                average - initial average value of the student.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or studentID<0 or average<0.
 *                FAILURE - If a student with studentID already in the system, or in the case of any other problem.
 *                SUCCESS - Otherwise.
 */
StatusType   AddStudent(void* DS, int studentID, int average){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->AddStudent(studentID, average););
}


/* Description:   The student with ID studentID decides to join a study group.
 * Input:         DS - A pointer to the data structure.
 *                studentID - The ID of the student.
 *                studyGroup - The ID of the study group he joins.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL, studentID<0 or study group is not an illegal number.
 *                FAILURE - If a student with same ID already assigned to another study group, there is no student in the system with this ID or in case of any other error.
 *                SUCCESS - Otherwise.
 */
StatusType   AssignStudent(void* DS, int studentID, int studyGroup){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->AssignStudent(studentID, studyGroup););
}


/* Description:   Joins the two faculties of studyGroup1 and studyGroup2 together, the name of the joined faculties is the name of faculty of studyGroup1.
 * Input:         DS - A pointer to the data structure.
 *                studyGroup1 - The identifier of the 1st study group.
 *                studyGroup2 - The identifier of the 2nd study group.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or either studyGroup1 or studyGroup2 are illegal group numbers.
 *                FAILURE - If both study groups belong to the same faculties, or in case of any other error.
 *                SUCCESS - Otherwise.
 */
StatusType   JoinFaculties(void* DS, int studyGroup1, int studyGroup2){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->JoinFaculties(studyGroup1, studyGroup2););
}



/* Description:   Joins the two faculties of student1 and student2 together, the name of the joined faculties is the name of faculty of student1.
 * Input:         DS - A pointer to the data structure.
 *                studentID1 - The identifier of a 1st student.
 *                studentID2 - The identifier of a 2nd student.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 *                INVALID_INPUT - If DS==NULL or either studyGroup1 or studyGroup2 are illegal stugy group numbers.
 *                FAILURE - If both students belong to the same faculties, or in case of any other error.
 *                SUCCESS - Otherwise.
 */
StatusType  UnifyFacultiesByStudents(void* DS, int studentID1, int studentID2){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->UnifyFacultiesByStudents(studentID1, studentID2););
}


/* Description:   Returns the faculty to which studentID belongs.
 * Input:         DS - A pointer to the data structure.
 *                studentID - The identifier of the student.
 * Output:        faculty - The identifier of the faculty.
 * Return Values: INVALID_INPUT - If DS==NULL, studentID<0.
 *                FAILURE - If there is no student in the system with this ID, or student doesn't belong to any team or in case of any other error.
 *                SUCCESS - Otherwise.
 */
StatusType   GetFaculty(void* DS, int studentID, int* faculty){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->GetFaculty(studentID, faculty););
}


/* Description:   Multiplies the average of every student in study group in factor.
 * Input:         DS - A pointer to the data structure.
 *                studyGroup - The study group which is upgraded.
 *                factor - the factor in which to multiply the average of the students. Can not exceed 100.
 * Output:        None.
 * Return Values: ALLOCATION_ERROR - In case of an allocation error.
 * 				  INVALID_INPUT - If DS==NULL, factor<1 or studyGroup is an illegal study group number.
 *                SUCCESS - Otherwise.
 */
StatusType   UpgradeStudyGroup(void* DS, int studyGroup, int factor){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->UpgradeStudyGroup(studyGroup, factor););
}


/* Description:   Returns the ID of the strongest student in faculty studyGroup.
 * 				  If two or more students have the same average, the student with minimal ID is returned.
 * Input:         DS - A pointer to the data structure.
 *                facultyID - The identifier of the faculty.
 * Output:        student - The identifier of the student.
 * Return Values: INVALID_INPUT - If DS==NULL, facultyID isn't in range 0,.., n-1 or student==NULL.
 *                FAILURE - If there is no students in facultyID or in case of any other error.
 *                SUCCESS - Otherwise.
 */
StatusType   GetSmartestStudent(void* DS, int facultyID, int* student){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->GetSmartestStudent(facultyID, student););
}


/* Description:   Returns the number of students in system with average in range (min, max].
 * Input:         DS - A pointer to the data structure.
 *                min - The average low value.
 *                max - The average high value.
 * Output:        num - The number of students.
 * Return Values: INVALID_INPUT - If DS==NULL, num==NULL, fromAvg>=toAvg or fromAvg<0 or toAvg>100.
 *                FAILURE - In case of any other error.
 *                SUCCESS - Otherwise.
 */
StatusType   GetNumOfStudentsInRange(void* DS, int fromAvg, int toAvg, int* num){
	if (!DS){
		return INVALID_INPUT;
	}
	Technion* ds = (Technion*)DS;
	TRY_CATCH(ds->GetNumOfStudentsInRange(fromAvg, toAvg, num););
}


/* Description:   Quits and deletes the database.
 *                The variable pointed by DS should be set to NULL.
 * Input:         DS - A pointer to the data structure.
 * Output:        None.
 * Return Values: None.
 */
void        Quit(void** DS){
	Technion** ds = (Technion**)(DS);
	if (ds){
		delete *ds;
		*ds = NULL;
	}
}


