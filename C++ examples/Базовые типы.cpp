//Ðàáîòà ñ áàçîâûìè òèïàìè c++.
//Íàïèøèòå êîðîòêóþ ïðîãðàììó, êîòîðàÿ çàïðàøèâàåò øèðîòó â ãðàäóñàõ, ìèíóòàõ è ñåêóíäàõ
//ïëîñëå ÷åãî îòîáðàæàåò øèðîòó â äåñÿòè÷íîì ôîðìàòå. Â îäíîé ìèíóòå 60 óãëîâûõ ñåêóíä, 
//à â îäíîì ãðàäóñå 60 óãëîâûõ ìèíóò; ïðåäñòàâüòå ýòè çíà÷åíèÿ ñ ïîìîùüþ ñèìâîëè÷åñêèõ êîíñòàíò.
// Äëÿ êàæäîãî ââîäèìîãî çíà÷åíèÿ äîëæíà èñïîëüçîâàòüñÿ îòäåëüíàÿ ïåðåìåííàÿ.
//
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
