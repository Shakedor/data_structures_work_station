#ifndef DAT_STRUCT
#define DAT_STRUCT

namespace dataStructures{

	class dataSturctExceptions{};
	class failureExceptions : public dataSturctExceptions {};
	class dataAlreadyExists : public failureExceptions {};
	class dataDoesNotExist : public failureExceptions {};
	class sturctIsEmpty : public failureExceptions {};


}


#endif  /* DAT_STRUCT */
