#ifndef DAT_STRUCT
#define DAT_STRUCT
#include <iostream>

namespace dataStructures{
	
	template<class data>
	class smart_pointer;
	template<class data>
	std::ostream& operator<<(std::ostream& os, smart_pointer<data>& sp);

	class dataSturctExceptions{};
	class failureExceptions : public dataSturctExceptions {};
	class dataAlreadyExists : public failureExceptions {};
	class dataDoesNotExist : public failureExceptions {};
	class stucountertIsEmpty : public failureExceptions {};
	class sizeOverFlow : public failureExceptions {};
	class sturctIsEmpty : public dataDoesNotExist{};

	class counter{
		int number;

	public:
		counter() :number(0){}

		void increment(){
			number++;
		}

		void decrement(){
			--number;
		}

		int getNumber(){
			return number;
		}

	};


	template<class data>
	class smart_pointer{
		data* dataPointer;
		counter* Counter;
	public:
		smart_pointer() : dataPointer(NULL){
			Counter = new counter();
			Counter->increment();
		}

		smart_pointer(data* dataP) : dataPointer(dataP){
			 
			Counter = new counter();
			Counter->increment();
		}

		smart_pointer(const smart_pointer<data>& sp) : dataPointer(sp.dataPointer), Counter(sp.Counter)
		{
			Counter->increment();
		}

		~smart_pointer()
		{
			Counter->decrement();
			if (Counter->getNumber() == 0)
			{
				delete dataPointer;
				delete Counter;
			}
		}

		data& operator* ()
		{
			return *dataPointer;
		}

		data* operator-> ()
		{
			return dataPointer;
		}

		smart_pointer<data>& operator = (const smart_pointer<data>& sp)
		{
			
			if (this != &sp) // avoid assigning to itself
			{
				//destruction protocol for old data
				Counter->decrement();

				if (Counter->getNumber() == 0)
				{
					delete dataPointer;
					delete Counter;
				}

				//copy fields and increment
				dataPointer = sp.dataPointer;
				Counter = sp.Counter;
				Counter->increment();
			}
			return *this;
		}



	};


	template<class data>
	class spPrint{
	public:
		spPrint(){}
		void operator() (smart_pointer<data>& point){
			(*point).print();
		}
	};

}



int minInt(int a, int b){
	return (a < b) ? a : b;
}


#endif  /* DAT_STRUCT */
