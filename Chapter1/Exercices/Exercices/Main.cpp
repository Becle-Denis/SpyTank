#include <iostream>
#include <string>

// Chapter 1 Exercices 
// BECLE Denis C00254311 
// 16 / 09 / 2019 


// Q1 
int roundFloat(float number) 
{
	int x = (int) number; 
	
	if (number >= x + 0.5 && number > 0) 
	{
		x++;
	} 
	
	else if (number < x - 0.5 && number < 0) 
	{
		x--;
	}
	
	return x;

	// Solution  
	// return number < 0 ? number - 0.5f ; number + 0.5f ; 
}

//Q2 
double deg2rad(double degrees) 
{
	double radian;
	double const PI = acos(-1.0);

	radian = degrees * PI / 180; 

	return radian;
}


//Q3 
void vectorNormalize(float inputVector[], float resultVector[])
{
	float length = sqrt(inputVector[0] * inputVector[0] + inputVector[1] * inputVector[1]);

	resultVector[0] = inputVector[0] / length;
	resultVector[1] = inputVector[1] / length;
}

//Q4
void sort(std::string* pNames[]) 
{
	int length = 10;
	for (int i = 0; i < length; i++)
	{
		for (int j = i; j < length; j++)
		{
			if (*pNames[j] < *pNames[i]) {
				std::string* temp = pNames[i];
				pNames[i] = pNames[j];
				pNames[j] = temp;
			}
		}
	}
}

int main() 
{

	//RoundFloatTest
	/*  
	float userNumber; 

	std::cout << "Number ? " << std::endl;
	std::cin >> userNumber;

	std::cout << roundFloat(userNumber);

	return 0;

	*/


	// deg2radTest 
	/*
	double userDegrees; 

	std::cout << "Degrees ?" << std::endl;
	std::cin >> userDegrees;

	std::cout << "Radian = " << deg2rad(userDegrees) << std::endl;
	*/

	//vectorMormalizeTest 
	/*
	float inputVector[] = { 1, 1 };
	float resultVector[2];

	vectorNormalize(inputVector, resultVector);

	std::cout << "input vector\nX = " << inputVector[0] << "\nY = " << inputVector[1] << "\n\n";
	std::cout << "result vector\nX = " << resultVector[0] << "\nY = " << resultVector[1] << "\n\n";
	*/

	//Sort 
	std::string names[10];
	std::string* pNames[10];

	for (int i = 0; i < sizeof(names) / sizeof(std::string); i++) 
	{
		std::cout << "Name " << i << " ? " << std::endl;
		std::cin >> names[i];
		std::cout << std::endl;

		pNames[i] = &names[i];
	}

	std::cout << "Original : \n";

	for (int i = 0; i < sizeof(pNames) / sizeof(std::string*); i++)
	{
		std::cout << i << " : " << pNames[i] << " = " << *pNames[i] << std::endl;
	}

	sort(pNames);

	std::cout << "\nSorted : \n";

	for (int i = 0; i < sizeof(pNames) / sizeof(std::string*); i++)
	{
		std::cout << i << " : " << pNames[i] << " = " << *pNames[i] << std::endl;
	}



}
