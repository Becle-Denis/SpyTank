
#include <iostream>

void q4() {
	float arr[5] = { 12.5, 10.0, 13.5, 90.5, 0.5 };

	int* p1;

	float * ptr1 = &arr[0];
	float* ptr2 = ptr1 + 3;

	std::cout << sizeof(arr)/sizeof(float) << std::endl; 

	std::cout << *ptr2 << std::endl;
	std::cout << (ptr2 - ptr1) << std::endl;
 
}

void q5(int *px, int *py) {
	*px = *px - *py;
	*py = *px + *py;
	*px = *py - *px;
}

void q5Main() {
	int x = 10, y = 20;
	q5(&x, &y);
	std::cout << "x = " << x << std::endl;
	std::cout << "y = " << y << std::endl;
}

void processArray(int* pBase, int const SIZE) {
	for (int i = 0; i < SIZE; i++) {
		std::cout << pBase[i] << std::endl;
	}
}

int main() {
	int arr[] = { 1,2,3 };
	processArray(arr, sizeof(arr) / sizeof(int));
	return 0;
}