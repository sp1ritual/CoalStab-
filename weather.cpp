#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* s) { // Функция обратного вызова для записи полученных данных
    size_t total_size = size * nmemb;
    s->append((char*)contents, total_size);
    return total_size;
}

int main() {
    CURL* curl; // Инициализация libcurl
    CURLcode res;
    curl = curl_easy_init();

    if (curl) {
        std::string city = "Kemerovo";
        std::string api_key = "3843c62f930c1dec6121067562c726aa";
        std::string url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        std::string response_string;

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            Json::Value root;
            Json::Reader reader;
            bool parsingSuccessful = reader.parse(response_string, root);
            if (parsingSuccessful) {

                float humidity = root["main"]["humidity"].asFloat();
                float temperature = root["main"]["temp"].asFloat();
                float precipitation = root["precipitation"]["value"].asFloat();
                float wind_speed = root["wind"]["speed"].asFloat();

                std::cout << "Влажность: " << humidity << "%" << std::endl;
                std::cout << "Температура: " << temperature << "K" << std::endl;
                std::cout << "Количество осадков: " << precipitation << "mm" << std::endl;
                std::cout << "Скорость ветра: " << wind_speed << " м/с" << std::endl;
            }
            else {
                std::cerr << "Ошибка при парсинге JSON\n";
            }
        }
        else {
            std::cerr << "Ошибка при выполнении запроса: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Не удалось инициализировать libcurl" << std::endl;
    }

    return 0;
}
