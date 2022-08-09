#include <iostream>

using namespace std;

void FillArr(int* arr, int size);
void ShowArr(int* arr, int size);
void insert(int*& arr, int& size, const int value, const int pos);

int main()
{
	int size = 5;
	int* arr = new int[size];
	FillArr(arr, size);
	ShowArr(arr, size);
	int value;
	int pos;
	cout << "Enter the new value: ";
	cin >> value;
	cout << "Enter position: ";
	cin >> pos;
	while (pos > (size + 1) || pos <= 0)
	{
		cout << "Enter position: ";
		cin >> pos;
	}
	insert(arr, size, value, pos);
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

void insert(int*& arr, int& size, const int value, const int pos)
{
	if (pos == 1)
	{
		size++;
		int* arr2 = new int[size];
		arr2[0] = value;
		for (int i = 1; i < size; i++)
		{
			arr2[i] = arr[i - 1];
		}
		delete[] arr;
		arr = arr2;
	}
	else if (pos == (size + 1))
	{
		size++;
		int* arr2 = new int[size];
		arr2[size - 1] = value;
		for (int i = 0; i < size - 1; i++)
		{
			arr2[i] = arr[i];
		}
		delete[] arr;
		arr = arr2;
	}
	else if (pos > 1 || pos < (size + 1))
	{
		size++;
		int* arr2 = new int[size];
		for (int i = 0; i < size; i++)
		{
			if (i < (pos - 1))
			{
				arr2[i] = arr[i];
			}
			else if (i == (pos - 1))
			{
				arr2[i] = value;
			}
			else if (i > (pos - 1))
			{
				arr2[i] = arr[i - 1];
			}
		}
		delete[] arr;
		arr = arr2;
	}
}