#include <iostream>

// Chapter 1 Exercices 
// BECLE Denis C00254311 
// 13 / 09 / 2019 


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
	float lenght = sqrt(inputVector[0] * inputVector[0] + inputVector[1] * inputVector[1]);

	resultVector[0] = inputVector[0] / length;
	resultVector[1] = inputVector[0] / length;
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


}
