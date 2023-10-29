#include <SPI.h>
#include <Ethernet.h>
#include <string.h>
#include <Array.h>

const uint8_t max_commands = 4;
Array<String, max_commands> server_1_commands;
//server_1_commands[0] = Включение/выключение
//server_1_commands[1] = Перезагрузка
//server_1_commands[2] = Принудительное выключение
//server_1_commands[3] = Статус

//Номера пинов для первого сервера
uint8_t SERVER_1_PIN_PWR = 2;
uint8_t SERVER_1_PIN_RESET = 3;
uint8_t SERVER_1_SERVER = 4;
uint8_t SERVER_1_READ_STATUS = 5;
//Номера пинов для второго сервера
uint8_t SERVER_2_PIN_PWR = 6;
uint8_t SERVER_2_PIN_RESET = 7;
uint8_t SERVER_2_SERVER = 8;
uint8_t SERVER_2_READ_STATUS = 9;
//Сохраняют статус, включен ли сервер
bool IS_TURNED_ON_1 = false;
bool IS_TURNED_ON_2 = false;
//Мак и айпи адрес, 80-й порт.
byte mac[] = {
  0xDE, 0xAD, 0xC0, 0xA8, 0x01, 0x34 };
IPAddress ip(192, 168, 100, 40);
EthernetServer server(80);
EthernetClient client;
//Текстовые переменные для обработки post
const uint8_t postMax = 128;
int postSize;
String readString;
String status;
char buffer;
char post[postMax];

uint16_t WAIT = 300;

void setup() {
    delay(500);
    Serial.begin(9600);
    FillCommands(server_1_commands);
    // Создание подключения
    setupCommunications();
}
void loop() {
    // если клиент подключен
    client = server.available();
    // принемаем POST запрос
    getPostRequest();
}
void setupCommunications() {
    Ethernet.begin(mac, ip);
    // Пауза, чтобы обеспечить успешное соединение
    delay(WAIT);
    // старт сервер
    server.begin();
}
void getPostRequest() {
    // если клиент подключен....
    if (client) {
        boolean currentLineIsBlank = true;
        postSize = 0;
        while (client.connected()) {
            if (client.available()) {
                char is_empty = client.read();
                // если вы получили символ новой строки
                // и символ пустой строки, то POST запрос закончился
                // и вы можете отправить ответ
                if (is_empty == '\n' && currentLineIsBlank) {
                    uint8_t counter = 0;
                    while (client.available()) {
                        // Здесь содержатся данные POST запроса
                        buffer = client.read();
                        if (postSize < postMax)
                            post[postSize++] = buffer;  // сохраняем новый символ в буфере и создаем приращение bufferSize 
                        ++counter;
                    }

                    readString = post;
                    readString.remove(counter);
                    uint8_t server_number = ParsePost(post);
                    // Выполнение команд
                    switch (server_number)
                    {
                    case 1:
                        SetupServer(SERVER_1_PIN_PWR, SERVER_1_PIN_RESET, SERVER_1_SERVER, SERVER_1_READ_STATUS, IS_TURNED_ON_1, server_number);
                        break;
                    case 2:
                        SetupServer(SERVER_2_PIN_PWR, SERVER_2_PIN_RESET, SERVER_2_SERVER, SERVER_2_READ_STATUS, IS_TURNED_ON_2, server_number);
                        break;
                    }
                    // Отправка ответа
                    sendResponse();
                }
                else if (is_empty == '\n') {
                    currentLineIsBlank = true;
                }
                else if (is_empty != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
    }
}
void sendResponse() {
    client.stop();
    delay(WAIT);
}

int ParsePost(const char* post)
{
    int server_num = 0;
    !(isdigit(post[1])) ? server_num = post[0] - '0' : server_num = (10 * (post[0] - '0')) + (post[1] - '0');
    return server_num;
}

void SetupServer(uint8_t PWR, uint8_t RESET, uint8_t SERVER, uint8_t READ_STATUS, bool& IS_TURNED_ON, int serv_num)
{
    pinMode(PWR, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(SERVER, OUTPUT);
    pinMode(READ_STATUS, INPUT);

    String command = readString.substring(1);

    if (command == server_1_commands[0])
    {
        digitalWrite(PWR, HIGH);
        delay(300);
        digitalWrite(PWR, LOW);

        IS_TURNED_ON ? IS_TURNED_ON = false : IS_TURNED_ON = true;
        //Тестовая часть для лампочек, не для прода
        if (IS_TURNED_ON)
        {
            digitalWrite(SERVER, HIGH);
        }
        else
        {
            digitalWrite(SERVER, LOW);
        }
    }
    else if (command == server_1_commands[1] && (IS_TURNED_ON))
    {
        IS_TURNED_ON = true;
        digitalWrite(SERVER, LOW);
        digitalWrite(RESET, HIGH);
        delay(300);
        digitalWrite(RESET, LOW);

        //Тестовая часть для лампочек, не для прода
        delay(1000);
        digitalWrite(SERVER, HIGH);
    }
    else if (command == server_1_commands[2] && (IS_TURNED_ON))
    {
        if (IS_TURNED_ON)
        {
            IS_TURNED_ON = false;
            digitalWrite(PWR, HIGH);
            delay(6000);
            digitalWrite(PWR, LOW);

            //Тестовая часть для лампочек, не для прода
            digitalWrite(SERVER, LOW);
        }
    }

    else if (command == server_1_commands[3])
    {
        if (digitalRead(READ_STATUS))
        {
            Serial.print(serv_num);
            Serial.print(" ");
            status = "Server is ON";
            Serial.println(status);
            //IS_TURNED_ON = true;
        }
        else
        {
            Serial.print(serv_num);
            Serial.print(" ");
            status = "Server is OFF";
            Serial.println(status);
            //IS_TURNED_ON = false;
        }
    }
}

void FillCommands(Array<String, max_commands>& container)
{
    for (int i = 0; i < max_commands; ++i)
    {
        switch (i)
        {
        case 0:
            container.push_back("server=on");
            break;
        case 1:
            container.push_back("server=reset");
            break;
        case 2:
            container.push_back("server=forced");
            break;
        case 3:
            container.push_back("server=status");
            break;
        }
    }
}

void DiodTest(uint8_t PWR, uint8_t RESET, uint8_t SERVER)
{
    pinMode(PWR, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(SERVER, OUTPUT);
    digitalWrite(PWR, HIGH);
    digitalWrite(RESET, HIGH);
    digitalWrite(SERVER, HIGH);
}
