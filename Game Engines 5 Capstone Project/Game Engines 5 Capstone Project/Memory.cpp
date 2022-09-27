#include "Memory.h"

Memory* Memory::instance;

Memory::Memory(std::vector<const char*> file_, std::vector<std::size_t> playerSize_)
{
	File = file_;

	std::cout << "\n";
	for (int i = 0; i < file_.size(); i++)
	{
		std::ifstream in_file(file_[i], std::ios::binary);
		in_file.seekg(0, std::ios::end);

		int fileSize = in_file.tellg();

		std::cout << "The size of " << file_[i] << " file is " << fileSize << " bytes!\n";

		for (int i = 0; i < playerSize_.size(); i++)
		{
			file = static_cast<byte*>(malloc(playerSize_[i]));
			fileIndex = 0;
		}

		std::cout << "The size of " << file_[i] << " string is " << playerSize_[i] << " bytes!\n";
	}

	std::cout << "\nThe size of byte* (name) is " << sizeof(file) << " bytes!\n" << std::endl;
}


Memory::~Memory()
{
	free(file);

	std::cout << "\n";
	for (int i = 0; i < File.size(); i++)
	{
		std::cout << "File " << File[i] << " freed from memory\n";
	}
	std::cout << "\n";
}

void Memory::MemorySizeOfVectors()
{
	void* vectorPtr = malloc(1000000);
	void* vectorPtr2 = new Vec3[50000];

	std::cout << "vectorPtr size = " << sizeof(vectorPtr) << " " << vectorPtr << std::endl << "\n";
	std::cout << "vectorPtr2 size = " << sizeof(vectorPtr2) << " " << vectorPtr2 << std::endl << "\n";
}

void Memory::ManageMemory(int max, char startLetterAt)
{
	char* letters;
	letters = (char*)malloc((max + 1) * sizeof(char)); // Memory will be dynamically allocated

	if (letters != NULL)  // If the letters are not equal to NULL, then print the letters
	{
		for (int i = 0; i < max; i++) // Depending on the maximum amount of characters, the returned letters will vary
		{
			letters[i] = startLetterAt + i;

			letters[max] = '\0';
		}

		// Prints out the alphabet starting from the letter you used in the parameter
		printf("Alphabet = %s\n", letters);

		std::cout << "The size of the letters is " << sizeof(letters) << " bytes!\n";

		free(letters); // Frees the letters from the memory after it's done
		std::cout << "The letters have been freed from the memory!\n";
	}
	else {
		printf("Not enough memory\n"); // If letters are NULL, return this statement to the console
	}
}

void Memory::TypeNumbers(int maxAmount)
{
	int n;
	int* p;

	// std::nothrow is used as an argument for operator new and operator new[] to 
	// indicate that these functions won't throw an exception on failure, but return nullptr
	p = new (std::nothrow) int[maxAmount];

	if (p == nullptr)
	{
		std::cerr << "Memory could not be allocated";
	}

	else
	{
		std::cout << "\nInput numbers: ";
		for (n = 0; n < maxAmount; n++)
		{
			std::cin >> p[n];

			if (!p[n])
			{
				std::cerr << "You didn't return a number, try again!\n";
			}
		}

		std::cout << "\nReceived numbers: ";
		for (n = 0; n < maxAmount; n++)
		{
			std::cout << p[n] << std::endl;
		}

		std::cout << "\nThe size of n is " << sizeof(n) << " bytes!" << std::endl;
		std::cout << "The size of *p is " << sizeof(p) << " bytes!" << std::endl;

		delete[] p;
		std::cout << "Memory has been freed!\n";
	}
}