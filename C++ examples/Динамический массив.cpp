//–азработайте структуру по имени car, котора€ будет хранить //следующую информацию об автомобиле: название производител€ в 
//виде строки в символьном массиве или в объекте string, а также
//год выпуска автомобил€ в виде целого числа. Ќапишите программу,
//котора€ запросит пользовател€, сколько автомобилей необходимо //включить в каталог. «атем программа должна применить new дл€ //создани€ динамического массива структур car указанного //пользователем размера. ƒалее она должна пригласить пользовател€ ввести название производител€ и год выпуска дл€ наполнени€ данными каждой структуры в массиве

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