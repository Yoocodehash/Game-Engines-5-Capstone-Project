#include "MemoryAllocation.h"


void MemoryAllocation::ReturnMemoryAllocation()
{

	VectorVariables vec;

	std::cout << "The byte size of the vectors = " << sizeof(vec) << std::endl << "\n";

	void* vectorPtr = malloc(1000000);
	void* vectorPtr2 = new VectorVariables[50000];

	std::cout << "vectorPtr size = " << sizeof(vectorPtr) << " " << vectorPtr << std::endl << "\n";


	float* floatNumber = nullptr;
	floatNumber = new float[1000];

	std::queue<float*> numbers;

	int counter = 0;
	while (counter < 10)
	{
		counter++;

		std::cout << "Input a float number: \n";
		std::cin >> *floatNumber;

		numbers.push(floatNumber);

		std::cout << "Printed float numbers: \n";
		while (!numbers.empty())
		{
			std::cout << numbers.front() << " " << *floatNumber << std::endl;
			numbers.pop();
		}
	}

	delete[] floatNumber;
	free(vectorPtr);

	//delete vec;
}