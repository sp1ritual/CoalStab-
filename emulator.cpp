#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

class SensorEmulator {
private:
    float min_value;
    float max_value;
    float current_value;

public:
    SensorEmulator(float min_val, float max_val) : min_value(min_val), max_value(max_val) {
        current_value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max_val - min_val) + min_val;
    }

    float getCurrentValue() {
        return current_value;
    }

    void setRange(float min_val, float max_val) {
        min_value = min_val;
        max_value = max_val;
    }

    void updateValue() {
        current_value = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max_value - min_value) + min_value;
    }
};

int main() {
    setlocale(LC_ALL, "Russian"); 
    srand(static_cast<unsigned int>(time(0)));

    SensorEmulator humiditySensor(0, 100); // влажность
    SensorEmulator temperatureSensor(-20, 100); // температура
    SensorEmulator frictionSensor(0, 30); // трение
    SensorEmulator dumpHeightSensor(0, 100); // Высота отвала
    SensorEmulator dumpAngleSensor(0, 90); // Угол наклона отвала
    SensorEmulator dumpLengthSensor(10, 100); // Длина отвала
    SensorEmulator dumpWidthSensor(5, 50); // Ширина отвала
    SensorEmulator dumpSurfaceAreaSensor(100, 500); // Площадь поверхности отвала
    SensorEmulator soilStrengthSensor(10, 50); // Прочность грунта
    SensorEmulator soilDensitySensor(1000, 2000); // Плотность грунта
    SensorEmulator soilFrictionAngleSensor(20, 45); // Угловой коэффициент внутреннего трения грунта
    SensorEmulator precipitationSensor(0, 50); // Датчик количества осадков
    SensorEmulator windSpeedSensor(0, 30); // Скорость ветра


    int row = 0;

    int choice = 0;
    std::ofstream outputFile;
    do {
        std::cout << "Меню:\n";
        std::cout << "1. Обновить данные с датчиков\n";
        std::cout << "2. Показать данные с датчиков\n";
        std::cout << "3. Сохранить полученные данные в БД\n";
        std::cout << "4. Выход\n";
        std::cout << "Введите свой выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            humiditySensor.updateValue();
            temperatureSensor.updateValue();
            frictionSensor.updateValue();
            dumpHeightSensor.updateValue();
            dumpAngleSensor.updateValue();
            dumpLengthSensor.updateValue();
            dumpWidthSensor.updateValue();
            dumpSurfaceAreaSensor.updateValue();
            soilStrengthSensor.updateValue();
            soilDensitySensor.updateValue();
            soilFrictionAngleSensor.updateValue();
            precipitationSensor.updateValue();
            windSpeedSensor.updateValue();
            break;
        case 2:
            std::cout << "Влажность: " << humiditySensor.getCurrentValue() << "%, Температура: " << temperatureSensor.getCurrentValue() << "°C, Трение: " << frictionSensor.getCurrentValue() << std::endl;
            std::cout << "Высота отвала: " << dumpHeightSensor.getCurrentValue() << "m, Угол наклона отвала: " << dumpAngleSensor.getCurrentValue() << "°, Длина отвала: " << dumpLengthSensor.getCurrentValue() << "m, Ширина отвала: " << dumpWidthSensor.getCurrentValue() << "m" << std::endl;
            std::cout << "Площадь поверхности отвала: " << dumpSurfaceAreaSensor.getCurrentValue() << "sq.m, Прочность грунта: " << soilStrengthSensor.getCurrentValue() << "MPa, Плотность грунта: " << soilDensitySensor.getCurrentValue() << "kg/m^3" << std::endl;
            std::cout << "Угловой коэффициент внутреннего трения грунта: " << soilFrictionAngleSensor.getCurrentValue() << "°, Количество осадков: " << precipitationSensor.getCurrentValue() << "mm, Скорость ветра: " << windSpeedSensor.getCurrentValue() << "m/s" << std::endl;
           
            break;
        case 3:
            outputFile.open("БД.txt", std::ios::app);
            if (outputFile.is_open()) {
            outputFile.close();
            std::cout << "Данные успешно сохранены в файл БД.txt" << std::endl;
            }
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз.\n";
        }
    } while (choice != 4);

    return 0;
}