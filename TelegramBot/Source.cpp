#include <stdio.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <ctime>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <tgbot/tgbot.h>

using namespace std;
using namespace TgBot;

string get_request(std::string link);
string rand_photo(void);
string rand_message(void);
float get_currency(char what);

int main() {
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);

    vector<InlineKeyboardButton::Ptr> buttons;

    InlineKeyboardButton::Ptr usd_button(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr eur_button(new InlineKeyboardButton);

    InlineKeyboardButton::Ptr photo_button(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr text_button(new InlineKeyboardButton);
    InlineKeyboardButton::Ptr food_button(new InlineKeyboardButton);

    ust_button->text = "USD";
    eur_button->text = "EUR";
    photo_button->text = "text";
    text_button->text = "text";
    food_button->text = "text";

    usd_button->callbackData = "USD";
    eur_button->callbackData = "EUR";
    photo_button->callbackData = "photo";
    text_button->callbackData = "text";
    food_button->callbackData = "food";

    food_button->url = "https://eda.yandex.ru/moscow?shippingType=delivery";

    buttons.push_back(usd_button);
    buttons.push_back(eur_button);
    buttons.push_back(photo_button);
    buttons.push_back(text_button);
    buttons.push_back(food_button);

    keyboard->inlineKeyboard.push_back(buttons);

    TgBot::Bot bot("TOKEN");
    bot.getEvents().onCommand("start", [&bot, keyboard](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "text", false, 0, keyboard);
        });

    bot.getEvents().onCommand("text", [&bot, keyboard](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, rand_message());
        });

    bot.getEvents().onCommand("currency", [&bot, keyboard](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "How currency?", false, 0, keyboard);
        });

    bot.getEvents().onCommand("photo", [&bot, keyboard](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "text", false, 0, keyboard);
        });




    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (query->data == "text") {
            bot.getApi().sendMessage(query->message->chat->id, rand_message(), false, 0, keyboard);
        }
        });

    bot.getEvents().onCallbackQuery([&bot](CallbackQuery::Ptr query) {
        if (query->data == "photo") {
            bot.getApi().sendPhoto(query->message->chat->id, InputFile::fromFile(rand_photo(), "image/jpg"));
        }
        });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (query->data == "food") {
            bot.getApi().sendMessage(query->message->chat->id, "text", false, 0, keyboard);
        }
        });

    bot.getEvents().onCallbackQuery([&bot, &keyboard](CallbackQuery::Ptr query) {
        if (query->data == "USD") {
            bot.getApi().sendMessage(query->message->chat->id, to_string(get_currency('u')));
        }
        else {
            bot.getApi().sendMessage(query->message->chat->id, to_string(get_currency('e')));
        }
        });





    bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        else if (StringTools::startsWith(message->text, "/text")) {
            return;
        }
        else if (StringTools::startsWith(message->text, "/currency")) {
            return;
        }
        else if (StringTools::startsWith(message->text, "/photo")) {
            return;
        }
        else if (StringTools::startsWith(message->text, "/food")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "I dont understand you.");
        });


    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}


static size_t Writer(char* buffer, size_t size, size_t nmemb, string* html) {
    int result = 0;

    if (buffer != NULL) {
        html->append(buffer, size * nmemb);
        result = size * nmemb;
    }
    return result;
}


string get_request(std::string link) {
    CURL* curl;
    string data;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, link.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return data;
}


float get_currency(char what) {
    auto js_obj = nlohmann::json::parse(get_request("https://www.cbr-xml-daily.ru/daily_json.js"));
    if (what == 'u') {
        return js_obj["Valute"]["USD"]["Value"].get<float>();
    }
    else {
        return js_obj["Valute"]["EUR"]["Value"].get<float>();
    }
    return -1;
}

string rand_message(void)
{
    srand(time(NULL));
    int const SIZE = 10;
    string text[SIZE] = {
        "1", "2", "3",
        "4", "5", "6", "7",
        "8", "9", "10"
    };
    return text[rand() % 10];
}

string rand_photo(void)
{
    srand(time(NULL));
    int const SIZE = 7;
    string jpg[SIZE] = {
        "/root/TelegramBot/photos/1.jpg", "/root/TelegramBot/photos/2.jpg", "/root/TelegramBot/photos/3.jpg",
        "/root/TelegramBot/photos/4.jpg", "/root/TelegramBot/photos/5.jpg", "/root/TelegramBot/photos/6.jpg", "/root/TelegramBot/photos/7.jpg"
    };
    return jpg[rand() % 7];
}
