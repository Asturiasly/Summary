#include <iostream> // ���������� ����� �����-������.
 
using namespace std; //��-�������� ���������� ������������ ���� �� ���� ����.
 
int main()
{
    double avg_mark = 0; // ������� ����
    int w_exp = 0; // ����������, ���������� �� ���� ����� ������
    double coeff = 0; // �����������, ���������� �� ����� ������
 
    cout << "Eenter your average of the diploma (your average must be from 3 to 5)" << endl;
    while (avg_mark > 5 || avg_mark < 3) // ��������� � ������� ���������� ����������� �������� �������� �����, �� ���������� ����, ��������� ��� ����������
    {
        cin >> avg_mark;
        if (avg_mark > 5 || avg_mark < 3)
            cout << "Please, enter value from 3 to 5" << endl;
    };
 
    cout << "1 - No experience;" << endl;
    cout << "2 - Less 2 years;" << endl;
    cout << "3 - From 2 to 5 years." << endl;
    cout << "Enter your work experience: ";
    cin >> w_exp;
                    //���������� ���� � ������ ����������� ��������
    switch (w_exp)
    {
    case 1: coeff = 1;
        break;
    case 2: coeff = 13;
        break;
    case 3: coeff = 16;
        break;
    }
                //������ ����������� � ������������ � ��������� ���������
 
    cout << "Your rating is " << coeff * avg_mark << endl; // ���������� �������
 
    if ((coeff * avg_mark) >= 45)
        cout << "Your rating is enough for us. Congratulations!";
    else
        cout << "Unfortunately, your rating is not enough.";
                                                            // ���������� � ����������� ��� ������������� ������������ ������
    return 0;
}