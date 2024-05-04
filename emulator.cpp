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
    SensorEmulator temperatureSensor(-30, 100); // температура
    SensorEmulator precipitationSensor(0, 70); // Датчик количества осадков
    SensorEmulator windSpeedSensor(0, 50); // Скорость ветра
    SensorEmulator Rd(0, 100); //dynamicstability
    SensorEmulator Pd(0, 100);
    SensorEmulator k(0, 100);
    SensorEmulator V(0, 100);
    SensorEmulator C(0, 100); //safetyfactor
    SensorEmulator F(0, 100);
    SensorEmulator h(0, 100); //slopeangle 
    SensorEmulator L(0, 100);
    SensorEmulator c(0, 100); //slopesafety
    SensorEmulator A(0, 100);
    SensorEmulator sigma(0, 100);
    SensorEmulator N(0, 100);
    SensorEmulator y(0, 100);
    SensorEmulator T(0, 100); //stability
    SensorEmulator R(0, 100);
    SensorEmulator H(0, 100); //staticstability


    int row = 0;

    int choice = 0;
    std::ofstream outputFile;
    do {
        std::cout << "Меню:\n";
        std::cout << "1. Показать данные с датчиков\n";
        std::cout << "2. Обновить данные с датчиков\n";
        std::cout << "3. Значения переменных\n";
        std::cout << "4. Обновить значение переменных\n";
        std::cout << "5. Сохранить полученные данные в БД\n";
        std::cout << "6. Выход\n";
        std::cout << "Введите свой выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Влажность: " << humiditySensor.getCurrentValue() << "°C, Температура: " << temperatureSensor.getCurrentValue()
            << "°, Количество осадков: " << precipitationSensor.getCurrentValue() << "mm, Скорость ветра: " << windSpeedSensor.getCurrentValue() << "m/s" << std::endl;
            break;
        case 2:
            humiditySensor.updateValue();
            temperatureSensor.updateValue();
            precipitationSensor.updateValue();
            windSpeedSensor.updateValue();
            break;
        case 3:
            std::cout << " Rd: " << Rd.getCurrentValue() << " Pd: " << Pd.getCurrentValue() << " k: " << k.getCurrentValue() << " V: " << V.getCurrentValue() << std::endl;
            std::cout << " C: " << C.getCurrentValue() << " F: " << F.getCurrentValue() << std::endl;
            std::cout << " h: " << h.getCurrentValue() << " L: " << L.getCurrentValue() << std::endl;
            std::cout << " c: " << c.getCurrentValue() << " A: " << A.getCurrentValue() << " sigma: " << sigma.getCurrentValue() << " N: " << N.getCurrentValue() << " y: " << y.getCurrentValue() << " V: " << V.getCurrentValue() << std::endl;
            std::cout << " T: " << T.getCurrentValue() << " R: " << R.getCurrentValue() << std::endl;
            std::cout << " c: " << c.getCurrentValue() << " A: " << A.getCurrentValue() << " N: " << N.getCurrentValue() << " y: " << y.getCurrentValue() << " H: " << H.getCurrentValue() << std::endl;
            std::cout << " A: " << A.getCurrentValue() << " F: " << F.getCurrentValue() << std::endl;
            break;
        case 4:
            Rd.updateValue();
            Pd.updateValue();
            k.updateValue();
            V.updateValue();
            C.updateValue();
            F.updateValue();
            h.updateValue();
            L.updateValue();
            c.updateValue();
            A.updateValue();
            sigma.updateValue();
            N.updateValue();
            y.updateValue();
            T.updateValue();
            R.updateValue();
            H.updateValue();
            break;
        case 5:
            outputFile.open("БД.txt", std::ios::app);
            if (outputFile.is_open()) {
            outputFile.close();
            std::cout << "Данные успешно сохранены в файл БД.txt" << std::endl;
            }
            break;
        default:
            std::cout << "Неверный выбор. Попробуйте еще раз.\n";
        }
    } while (choice != 6);

    return 0;
}
