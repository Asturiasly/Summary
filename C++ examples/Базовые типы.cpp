//������ � �������� ������ c++.
//�������� �������� ���������, ������� ����������� ������ � ��������, ������� � ��������
//������ ���� ���������� ������ � ���������� �������. � ����� ������ 60 ������� ������, 
//� � ����� ������� 60 ������� �����; ����������� ��� �������� � ������� ������������� ��������.
// ��� ������� ��������� �������� ������ �������������� ��������� ����������.

#include <iostream>

using namespace std;

int main()
{
    double degrees; double minutes; double seconds; double latitude;
    const int Sec_in_Min = 60;
    const int Min_in_Hour = 60;
    cout << "Enter a latitude in degrees, minutes, and seconds:" << endl;
    cout << "First, enter the degrees: ";
    cin >> degrees; cout << endl;
    cout << "Next, enter the minutes of arc: ";
    cin >> minutes; cout << endl;
    cout << "Finally, enter the seconds of arc: ";
    cin >> seconds; cout << endl;
    latitude = degrees + (minutes / Min_in_Hour) + (seconds / (Sec_in_Min * Min_in_Hour));

    cout << degrees << " degrees, " << minutes << " minutes, " << seconds << " seconds, = " << latitude << " degrees";
    return 0;
}