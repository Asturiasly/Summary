#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

ofstream fout;
ifstream fin;

struct patrons {
	string name;
	int value;
};

int main()
{
	int count;
	bool flag_1 = true;
	bool flag_2 = true;

	fin.open("C:\\Lorem_Ipsum.txt");
	if (!fin.is_open())
	{
		std::cout << "Couldn't open the file" << endl;
		std::cout << "Program terminanting.\n";
		exit(EXIT_FAILURE);
	}

	fin >> count;
	patrons* pp = new patrons[count];
	fin.get();
	for (int i = 0; i < count; i++)
	{
		getline(fin, pp[i].name);
		cout << pp[i].name << endl;
		fin >> pp[i].value;
		cout << pp[i].value << endl;
		fin.get();
	}


	cout << "----------GRAND PATRONS----------" << endl << endl;
	for (int i = 0; i < count; i++)
	{
		if (pp[i].value > 10000)
		{
			cout << pp[i].name << "\t" << pp[i].value << endl;
		}
	}

	for (int i = 0; i < count; i++)
	{
		if (pp[i].value >= 10000)
		{
			flag_1 = false;
			break;
		}
	}
	if (flag_1)
		cout << "None\n";

	std::cout << "----------PATRONS----------" << endl << endl;
	for (int i = 0; i < count; i++)
	{
		if (pp[i].value < 10000)
		{
			cout << pp[i].name << "\t" << pp[i].value << endl;
		}
	}
	for (int i = 0; i < count; i++)
	{
		if (pp[i].value > 10000)
		{
			flag_2 = false;
			break;
		}
	}
	if (flag_2)
		cout << "None";

	fin.close();
	delete[] pp;
	return 0;
}


