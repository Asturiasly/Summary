//������������ ��������� �� ����� car, ������� ����� ������� //��������� ���������� �� ����������: �������� ������������� � 
//���� ������ � ���������� ������� ��� � ������� string, � �����
//��� ������� ���������� � ���� ������ �����. �������� ���������,
//������� �������� ������������, ������� ����������� ���������� //�������� � �������. ����� ��������� ������ ��������� new ��� //�������� ������������� ������� �������� car ���������� //������������� �������. ����� ��� ������ ���������� ������������ ������ �������� ������������� � ��� ������� ��� ���������� ������� ������ ��������� � �������

#include <iostream>

using namespace std;

struct car {
    string name;
    int year;
};
int main()
{
    int ArSize;
    cout << "How many cars do you want to put in array? ";
    cin >> ArSize;
    car * pc = new car[ArSize];
    for (int i = 0; i < ArSize; i++)
    {
        cin.get();
        cout << "The car #" << i + 1 << endl;
        cout << "Enter the maker: ";
        getline(cin, pc[i].name);
        cout << "Enter the year: ";
        cin >> pc[i].year;
    }
    cout << "Here is your collection: " << endl;
    for (int i = 0; i < ArSize; i++)
    {
        cout << pc[i].year << " " << pc[i].name << endl;
    }
}