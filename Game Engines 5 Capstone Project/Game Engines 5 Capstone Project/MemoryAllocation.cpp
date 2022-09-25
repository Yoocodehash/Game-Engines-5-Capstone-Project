#include "MemoryAllocation.h"

MemoryAllocation::MemoryAllocation()
{

}

MemoryAllocation::~MemoryAllocation()
{

}

void MemoryAllocation::ReturnMemoryAllocation()
{
	std::cout << "\nThe byte size of enemy flappy bird file: " << sizeof("Enemy Flappy Bird.png") << " bytes!" << std::endl;
	std::cout << "The byte size of player flappy bird file: " << sizeof("Flappy bird sprite.png") << " bytes!" << std::endl;

	VectorVariables vec;

	std::cout << "\nThe byte size of the vectors: " << sizeof(vec) << " bytes!" << std::endl << "\n";

	void* vectorPtr = malloc(1000000);
	void* vectorPtr2 = new VectorVariables[50000];

	std::cout << "vectorPtr size = " << sizeof(vectorPtr) << " " << vectorPtr << std::endl << "\n";
	std::cout << "vectorPtr2 size = " << sizeof(vectorPtr2) << " " << vectorPtr2 << std::endl << "\n";


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

		std::cout << "\nPrinted float numbers: \n";
		while (!numbers.empty())
		{
			std::cout << numbers.front() << " " << *floatNumber << std::endl << "\n";
			numbers.pop();
		}
	}

	delete[] floatNumber;
	free(vectorPtr);

	//delete vec;
}