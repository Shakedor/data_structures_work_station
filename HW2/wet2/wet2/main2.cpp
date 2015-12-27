/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2015-2016                                     */
/*                                                                         */
/* Homework : Wet 2                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main2.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "library2.h"
#include <cstring>

using namespace std;


/* The command's strings */
typedef enum {
	NONE_CMD = -2,
	COMMENT_CMD = -1,
	INIT_CMD = 0,
	ADDSTUDENT_CMD = 1,
	ASSIGNSTUDENT_CMD = 2,
	JOINFACULTIES_CMD = 3,
	JOINFACULTIESBYSTUDENTS_CMD = 4,
	GETFACULTY_CMD = 5,
	UPGRADESTUDYGROUP_CMD = 6,
	GETSMARTESTSTUDENT_CMD = 7,
	GETNUMOFSTUDENTSINRANGE_CMD = 8,
	QUIT_CMD = 9
} commandType;

static const int numActions = 10;

static const char *commandStr[] = { "Init", "AddStudent", "AssignStudent",
				    "JoinFaculties", "UnifyFacultiesByStudents", "GetFaculty", "UpgradeStudyGroup",
				    "GetSmartestStudent", "GetNumOfStudentsInRange", "Quit" };

static const char* ReturnValToStr(int val) {
	switch (val) {
	case (SUCCESS):
		return "Success";
	case (FAILURE):
		return "Failure";
	case (ALLOCATION_ERROR):
		return "Allocation_error";
	case (INVALID_INPUT):
		return "Invalid_input";
	default:
		return "";
	}
}

/* we assume maximum string size is not longer than 256  */
#define MAX_STRING_INPUT_SIZE (255)
#define MAX_BUFFER_SIZE       (255)

#define StrCmp(Src1,Src2) ( strncmp((Src1),(Src2),strlen(Src1)) == 0 )

typedef enum {
	error_free, error
} errorType;
static errorType parser(const char* const command);

#define ValidateRead(read_parameters,required_parameters,ErrorString) \
if ( (read_parameters)!=(required_parameters) ) { printf(ErrorString); return error; }

static bool isInit = false;

/***************************************************************************/
/* main                                                                    */
/***************************************************************************/

int main(int argc, const char**argv) {

	char buffer[MAX_STRING_INPUT_SIZE];
	// Reading commands
	while (fgets(buffer, MAX_STRING_INPUT_SIZE, stdin) != NULL) {
		fflush(stdout);
		if (parser(buffer) == error)
			break;
	};
	return 0;
}
;

/***************************************************************************/
/* Command Checker                                                         */
/***************************************************************************/

static commandType CheckCommand(const char* const command,
		const char** const command_arg) {

	if (command == NULL || strlen(command) == 0 || StrCmp("\n", command))
		return (NONE_CMD);
	if (StrCmp("#", command)) {
		if (strlen(command) > 1)
			printf("%s", command);
		return (COMMENT_CMD);
	};
	for (int index = 0; index < numActions; index++) {
		if (StrCmp(commandStr[index], command)) {
			*command_arg = command + strlen(commandStr[index]) + 1;
			return ((commandType) index);
		};
	};
	return (NONE_CMD);
}
;

/***************************************************************************/
/* Commands Functions                                                      */
/***************************************************************************/

static errorType OnInit(void** DS, const char* const command);
static errorType OnAddStudent(void* DS, const char* const command);
static errorType OnAssignStudent(void* DS, const char* const command);
static errorType OnJoinFaculties(void* DS, const char* const command);
static errorType OnUnifyFacultiesByStudents(void* DS, const char* const command);
static errorType OnGetFaculty(void* DS, const char* const command);
static errorType OnUpgradeStudyGroup(void* DS, const char* const command);
static errorType OnGetSmartestStudent(void* DS, const char* const command);
static errorType OnGetNumOfStudentsInRange(void* DS,const char* const command);
static errorType OnQuit(void** DS, const char* const command);


/***************************************************************************/
/* Parser                                                                  */
/***************************************************************************/
static errorType parser(const char* const command) {
	static void *DS = NULL; /* The general data structure */
	const char* command_args = NULL;
	errorType rtn_val = error;

	commandType command_val = CheckCommand(command, &command_args);

	switch (command_val) {

	case (INIT_CMD):
		rtn_val = OnInit(&DS, command_args);
		break;
	case (ADDSTUDENT_CMD):
		rtn_val = OnAddStudent(DS, command_args);
		break;
	case (ASSIGNSTUDENT_CMD):
		rtn_val = OnAssignStudent(DS, command_args);
		break;
	case (JOINFACULTIES_CMD):
		rtn_val = OnJoinFaculties(DS, command_args);
		break;
        case (JOINFACULTIESBYSTUDENTS_CMD):
                rtn_val = OnUnifyFacultiesByStudents(DS, command_args);
                break;
	case (GETFACULTY_CMD):
		rtn_val = OnGetFaculty(DS, command_args);
		break;
	case (UPGRADESTUDYGROUP_CMD):
		rtn_val = OnUpgradeStudyGroup(DS, command_args);
		break;
	case (GETSMARTESTSTUDENT_CMD):
		rtn_val = OnGetSmartestStudent(DS, command_args);
		break;
	case (GETNUMOFSTUDENTSINRANGE_CMD):
		rtn_val = OnGetNumOfStudentsInRange(DS, command_args);
		break;
	case (QUIT_CMD):
		rtn_val = OnQuit(&DS, command_args);
		break;
	case (COMMENT_CMD):
		rtn_val = error_free;
		break;
	case (NONE_CMD):
		rtn_val = error;
		break;
	default:
		assert(false);
	};
	return (rtn_val);
}
;

int INIT_n;

/***************************************************************************/
/* OnInit                                                                  */
/***************************************************************************/
static errorType OnInit(void** DS, const char* const command) {
	if (isInit) {
		printf("Init was already called.\n");
		return (error_free);
	};
	isInit = true;

	ValidateRead( sscanf(command, "%d",&INIT_n), 1, "Init failed.\n");

	*DS = Init(INIT_n);
	if (*DS == NULL) {
		printf("Init failed.\n");
		return (error);
	};
	printf("Init done.\n");

	return error_free;
}

/***************************************************************************/
/* OnAddStudent                                                          */
/***************************************************************************/
static errorType OnAddStudent(void* DS, const char* const command) {

	int studentID;
	int average;
	ValidateRead( sscanf(command, "%d %d",&studentID,&average), 2,
			"AddStudent failed.\n");
	StatusType res = AddStudent(DS, studentID, average);

	printf("AddStudent: %s\n", ReturnValToStr(res));

	return error_free;
}

/***************************************************************************/
/* OnAddStudentToStudyGroup                                                */
/***************************************************************************/
static errorType OnAssignStudent(void* DS, const char* const command) {

	int studentID;
	int study_grp;
	ValidateRead( sscanf(command, "%d %d",&studentID,&study_grp), 2,
			"AssignStudent failed.\n");
	StatusType res = AssignStudent(DS, studentID, study_grp);

	printf("AssignStudent: %s\n", ReturnValToStr(res));

	return error_free;
}


/***************************************************************************/
/* OnUnifyFacultiesByStudents                                              */
/***************************************************************************/
static errorType OnUnifyFacultiesByStudents(void* DS, const char* const command) {

  int studentID1;
  int studentID2;

  ValidateRead( sscanf(command, "%d %d",&studentID1,&studentID2), 2,
			"UnifyFacultiesByStudents failed.\n");

  StatusType res = UnifyFacultiesByStudents(DS, studentID1, studentID2);

  printf("UnifyFacultiesByStudents: %s\n", ReturnValToStr(res));

  return error_free;
}


/***************************************************************************/
/* OnJoinFaculties                                                       */
/***************************************************************************/
static errorType OnJoinFaculties(void* DS, const char* const command) {
	int study_grp1;
	int study_grp2;
	ValidateRead( sscanf(command, "%d %d",&study_grp1,&study_grp2), 2,
			"JoinFaculties failed.\n");
	StatusType res = JoinFaculties(DS, study_grp1, study_grp2);

	printf("JoinFaculties: %s\n", ReturnValToStr(res));

	return error_free;
}

/***************************************************************************/
/* OnGetFaculty                                                         */
/***************************************************************************/
static errorType OnGetFaculty(void* DS, const char* const command) {
	int studentID;
	ValidateRead( sscanf(command, "%d",&studentID), 1,
			"GetFaculty failed.\n");
	int faculty;
	StatusType res = GetFaculty(DS, studentID, &faculty);

	if (res != SUCCESS) {
		printf("GetFaculty: %s\n", ReturnValToStr(res));
	} else {
		printf("GetFaculty: %s %d\n", ReturnValToStr(res), faculty);
	}

	return error_free;
}

/***************************************************************************/
/* OnUpgradeStudyGroup                                                           */
/***************************************************************************/
static errorType OnUpgradeStudyGroup(void* DS, const char* const command) {
	int groupID;
	int factor;
	ValidateRead( sscanf(command, "%d %d",&groupID,&factor), 2,
			"UpgradeStudyGroup failed.\n");
	StatusType res = UpgradeStudyGroup(DS, groupID, factor);

	printf("UpgradeStudyGroup: %s\n", ReturnValToStr(res));

	return error_free;
}

/***************************************************************************/
/* OnGetSmartestStudent                                                 */
/***************************************************************************/
static errorType OnGetSmartestStudent(void* DS, const char* const command) {
	int groupID;
	ValidateRead( sscanf(command, "%d",&groupID), 1,
			"GetSmartestStudent failed.\n");
	int student;
	StatusType res = GetSmartestStudent(DS, groupID, &student);

	if (res != SUCCESS) {
		printf("GetSmartestStudent: %s\n", ReturnValToStr(res));
	} else {
		printf("GetSmartestStudent: %s %d\n", ReturnValToStr(res),
				student);
	}

	return error_free;
}

/***************************************************************************/
/* OnGetNumOfStudentsInRange                                                 */
/***************************************************************************/
static errorType OnGetNumOfStudentsInRange(void* DS,
		const char* const command) {
	int min;
	int max;
	ValidateRead( sscanf(command, "%d %d",&min,&max), 2,
			"GetNumOfSuperherosInRange failed.\n");
	int student;
	StatusType res = GetNumOfStudentsInRange(DS, min, max, &student);

	if (res != SUCCESS) {
		printf("GetNumOfStudentsInRange: %s\n", ReturnValToStr(res));
	} else {
		printf("GetNumOfStudentsInRange: %s %d\n", ReturnValToStr(res),
				student);
	}

	return error_free;
}

/***************************************************************************/
/* OnQuit                                                                  */
/***************************************************************************/
static errorType OnQuit(void** DS, const char* const command) {
	Quit(DS);
	if (*DS != NULL) {
		printf("Quit failed.\n");
		return (error);
	};
	isInit = false;
	printf("Quit done.\n");

	return error_free;
}

