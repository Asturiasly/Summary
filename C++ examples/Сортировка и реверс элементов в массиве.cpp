#include <iostream>

using namespace std;

void FillArr(int* arr, int size);
void ShowArr(int* arr, int size);
void a_sort(int*& arr, int size);
void d_sort(int*& arr, int size);
void reverse(int*& arr, int size);

int main()
{
	int size = 15;
	int* arr = new int[size];
	FillArr(arr, size);
	cout << "This is your array: \n";
	ShowArr(arr, size);
	int choice;
	cout << "What do you want to do?\n";
	cout << "1) Reverse elements;\n"; cout << "2) Ascending sort;\n"; cout << "3) Descending sort;\n";

	cin >> choice;
	while (choice < 1 || choice > 3)
	{
		cout << "Enter your choice: ";
		cin >> choice;
	}
	switch (choice)
	{
	case 1: reverse(arr, size);
		break;
	case 2:  a_sort(arr, size);
		break;
	case 3: d_sort(arr, size);
		break;
	}
	ShowArr(arr, size);
	delete[] arr;
}

void FillArr(int* arr, int const size)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % 10;
	}
}

void ShowArr(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << endl;
	}
}

void reverse(int*& arr, int size)
{
	int counter = size - 1;
	int* arr2 = new int[size];
	for (int i = 0; i < size; i++)
	{
		arr2[i] = arr[counter];
		counter--;
	}
	delete[] arr;
	arr = arr2;
}

void a_sort(int*& arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		int temp;
		for (int j = 0; j < size; j++)
		{
			if (arr[j] > arr[i])
			{
				temp = arr[j];
				arr[j] = arr[i];
				arr[i] = temp;
			}
		}
	}
}

void d_sort(int*& arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		int temp;
		for (int j = 0; j < size; j++)
		{
			if (arr[j] < arr[i])
			{
				temp = arr[j];
				arr[j] = arr[i];
				arr[i] = temp;
			}
		}
	}
}

