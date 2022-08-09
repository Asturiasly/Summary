#include <iostream> // // Подключаем поток ввода-вывода.
 
using namespace std; //По-ленивому подключаем пространство имен на весь файл.
 
int main()
{
    double avg_mark = 0; // Средний балл
    int w_exp = 0; // Переменная, отвечающая за ввод опыта работы
    double coeff = 0; // Коэффициент, основанный на опыте работы
 
    cout << "Eenter your average of the diploma (your average must be from 3 to 5)" << endl;
    while (avg_mark > 5 || avg_mark < 3) // Поскольку в условии существует необходимый диапозон среднего балла, то организуем цикл, требующий его соблюдения
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
                    //Отображаем меню и вводим необходимое значение
    switch (w_exp)
    {
    case 1: coeff = 1;
        break;
    case 2: coeff = 13;
        break;
    case 3: coeff = 16;
        break;
    }
                //Задаем коэффициент в соответствии с введенным значением
 
    cout << "Your rating is " << coeff * avg_mark << endl; // Отображаем среднее
 
    if ((coeff * avg_mark) >= 45)
        cout << "Your rating is enough for us. Congratulations!";
    else
        cout << "Unfortunately, your rating is not enough.";
                                                            // Информация о прохождении или непрохождении минимального порога
    return 0;
}
