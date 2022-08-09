#include <iostream>
#include <string>

using namespace std;

void finder_letter(string, char);
void show_menu(void);

int main()
{ 
    string str;
    char ch;
    cout << "Enter the text you want to parse: ";
    getline(cin, str);
    cout << "Words with which letter do you want to find: ";
    cin >> ch;
    finder_letter(str, ch);
    return 0;
}

void finder_letter(string str, char ch)
{
    int word_counter = 1; // ѕробелов всегда на 1 меньше, чем слов.

    string s_temp; // буфер, сохран€ющий слово прочитанное до пробела

    for (int i = 0; i < size(str); i++) // —читаем количество пробелов дл€ подсчета необходимого значени€ массива
    {
        if (str[i] == ' ')
            word_counter++;
    }

    string* ps = new string[word_counter]; // ¬ыдел€ем динамический массив на основании подсчитанного количества пробелов. ѕробелы = слова + 1
    int words = 0;
    for (int i = 0; str[i] != ' '; ++i)
    {
        s_temp += str[i];
        if (str[i + 1] == ' ')
        {
            for (int j = 0; j < size(s_temp); j++)
            {
                if (s_temp[j] == ch)
                {
                    for (int z = 0; z < size(s_temp); z++)
                    {
                        if (ispunct(s_temp[z]))
                        {
                            s_temp[z] = NULL;
                        }
                    }
                    ps[words] = s_temp;
                    words++;
                    break;
                }
            }
            i = i + 1;
            s_temp = "";
        }
        if (str[i + 1] == '\0')
        {
            for (int j = 0; j < size(s_temp); j++)
            {
                if (s_temp[j] == 'a')
                {
                    for (int z = 0; z < size(s_temp); z++)
                    {
                        if (ispunct(s_temp[z]))
                        {
                            s_temp[z] = NULL;
                        }
                    }
                    ps[words] = s_temp;
                    words++;
                    break;
                }
            }
            break;
        }
    }

    for (int i = 0; i < words; i++)
    {
        cout << ps[i] << endl;
    }

    delete[] ps;
}

//void show_menu(void)
//{
    //int choice;
   // cout << "Hello, what actions do you want to do?";
   // cout << "1) Searching words with a given letter;" << endl;
   // cout << "2) Searching words with a given letters;" << endl;
    //cout << "3) Parsing txt file;" << endl;
   // cout << "4) Parsing html.";

   // switch (choice)
           //case 1:
//}

