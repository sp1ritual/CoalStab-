#include <iostream>
#include <dlfcn.h>
#include <fstream>
#include <ctime>
#include <random>
#include <chrono>
#include <sqlite3.h>
#include <cstdlib>

typedef void* (*create_t)(double, double, double, double);
typedef void (*destroy_t)(void*);
typedef double (*calculate_t)(void*);

typedef void* (*createSafetyFactor_t)();
typedef void (*destroySafetyFactor_t)(void*);
typedef double (*calculateSafety_t)(void*, double, double);

typedef void* (*createSlopeAngle_t)(double, double);
typedef void (*destroySlopeAngle_t)(void*);
typedef double (*calculateAngle_t)(void*);

typedef void* (*createStability_t)();
typedef void (*destroyStability_t)(void*);
typedef double (*calcStability_t)(void*, double, double);

typedef void* (*createTension_t)();
typedef void (*destroyTension_t)(void*);
typedef double (*calculateTension_t)(void*, double, double);

typedef void* (*createStaticStability_t)(double, double, double, double, double);
typedef void (*destroyStaticStability_t)(void*);
typedef double (*calculateDelaminationResistance_t)(void*);
typedef double (*calculateHorizontalForce_t)(void*);
typedef double (*calculateStabilityFactor_t)(void*);

typedef void* (*createSlopeSafetyCalculator_t)(double, double, double, double);
typedef void (*destroySlopeSafetyCalculator_t)(void*);
typedef double (*calculateSafetyFactor_t)(void*);

double lastDstability_result = 0;
double lastSafety_result = 0;
double lastSlope_result = 0;
double lastStability_result = 0;
double lastTension_result = 0;
double lastStabilityFa = 0;
double lastSlopeSafety_result = 0;

double getRandomNumber(double min, double max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

void saveToFile(double Dstability_result, double safety_result, double slope_result, double stability_result, double tension_result, double StabilityFa, double slopeSafety_result) {
    std::ofstream file("CoalStab_Data.txt", std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    file << "Date and Time: " << std::ctime(&time_now);
    file << "Dynamic Stability Factor: " << Dstability_result << std::endl;
    file << "Safety Factor: " << safety_result << std::endl;
    file << "Slope Angle: " << slope_result << std::endl;
    file << "Stability: " << stability_result << std::endl;
    file << "Tension: " << tension_result << std::endl;
    file << "Stability Factor: " << StabilityFa << std::endl;
    file << "Slope Safety: " << slopeSafety_result << std::endl;
    file << "---------------------------------------------" << std::endl;
    file.close();
}

void saveToDatabase(double Dstability_result, double safety_result, double slope_result, double stability_result, double tension_result, double StabilityFa, double slopeSafety_result) {
    sqlite3* db;
    char* err_message = 0;

    int rc = sqlite3_open("CoalStab_Data.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    } else {
        std::cout << "Opened database successfully" << std::endl;
    }

    const char* sql_create = 
        "CREATE TABLE IF NOT EXISTS StabilityData ("
        "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "DynamicStability DOUBLE, "
        "SafetyFactor DOUBLE, "
        "SlopeAngle DOUBLE, "
        "Stability DOUBLE, "
        "Tension DOUBLE, "
        "StabilityFactor DOUBLE, "
        "SlopeSafety DOUBLE);";

    rc = sqlite3_exec(db, sql_create, 0, 0, &err_message);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << err_message << std::endl;
        sqlite3_free(err_message);
    } else {
        std::cout << "Table created successfully" << std::endl;
    }

    char sql_insert[512];
    snprintf(sql_insert, sizeof(sql_insert), "INSERT INTO StabilityData (DynamicStability, SafetyFactor, SlopeAngle, Stability, Tension, StabilityFactor, SlopeSafety) VALUES (%f, %f, %f, %f, %f, %f, %f);",
             Dstability_result, safety_result, slope_result, stability_result, tension_result, StabilityFa, slopeSafety_result);

    rc = sqlite3_exec(db, sql_insert, 0, 0, &err_message);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << err_message << std::endl;
        sqlite3_free(err_message);
    } else {
        std::cout << "Data inserted successfully" << std::endl;
    }

    sqlite3_close(db);
}

void performCalculations() {
    void* handle1 = dlopen("./libdlyb.dylib", RTLD_LAZY);
    if (!handle1) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return;
    }

    create_t create = (create_t)dlsym(handle1, "create");
    destroy_t destroy = (destroy_t)dlsym(handle1, "destroy");
    calculate_t calculate = (calculate_t)dlsym(handle1, "calculate");

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from first library: " << dlsym_error << '\n';
        dlclose(handle1);
        return;
    }

    void* handle2 = dlopen("./libsafetyfactor.dylib", RTLD_LAZY);
    if (!handle2) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        dlclose(handle1);
        return;
    }

    createSafetyFactor_t createSafetyFactor = (createSafetyFactor_t)dlsym(handle2, "createSafetyFactor");
    destroySafetyFactor_t destroySafetyFactor = (destroySafetyFactor_t)dlsym(handle2, "destroySafetyFactor");
    calculateSafety_t calculateSafety = (calculateSafety_t)dlsym(handle2, "calculateSafety");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from safety factor library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        return;
    }

    void* handle3 = dlopen("./libslopeangle.dylib", RTLD_LAZY);
    if (!handle3) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        dlclose(handle1);
        dlclose(handle2);
        return;
    }
    createSlopeAngle_t createSlopeAngle = (createSlopeAngle_t)dlsym(handle3, "createSlopeAngle");
    destroySlopeAngle_t destroySlopeAngle = (destroySlopeAngle_t)dlsym(handle3, "destroySlopeAngle");
    calculateAngle_t calculateAngle = (calculateAngle_t)dlsym(handle3, "calculateAngle");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from slope angle library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        return;
    }

    void* handle4 = dlopen("./libstability.dylib", RTLD_LAZY);
    if (!handle4) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        return;
    }
    createStability_t createStability = (createStability_t)dlsym(handle4, "createStability");
    destroyStability_t destroyStability = (destroyStability_t)dlsym(handle4, "destroyStability");
    calcStability_t calcStability = (calcStability_t)dlsym(handle4, "calcStability");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from stability library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        return;
    }

    void* handle5 = dlopen("./libtension.dylib", RTLD_LAZY);
    if (!handle5) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        return;
    }
    createTension_t createTension = (createTension_t)dlsym(handle5, "createTension");
    destroyTension_t destroyTension = (destroyTension_t)dlsym(handle5, "destroyTension");
    calculateTension_t calculateTension = (calculateTension_t)dlsym(handle5, "calculateTension");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from stability library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        dlclose(handle5);
        return;
    }

    void* handle6 = dlopen("./libstaticstability.dylib", RTLD_LAZY);
    if (!handle6) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        dlclose(handle5);
        return;
    }
    createStaticStability_t createStaticStability = (createStaticStability_t)dlsym(handle6, "createStaticStability");
    destroyStaticStability_t destroyStaticStability = (destroyStaticStability_t)dlsym(handle6, "destroyStaticStability");
    calculateDelaminationResistance_t calculateDelaminationResistance = (calculateDelaminationResistance_t)dlsym(handle6, "calculateDelaminationResistance");
    calculateHorizontalForce_t calculateHorizontalForce = (calculateHorizontalForce_t)dlsym(handle6, "calculateHorizontalForce");
    calculateStabilityFactor_t calculateStabilityFactor = (calculateStabilityFactor_t)dlsym(handle6, "calculateStabilityFactor");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from stability library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        dlclose(handle5);
        dlclose(handle6);
        return;
    }

    void* handle7 = dlopen("./libslopesafety.dylib", RTLD_LAZY);
    if (!handle7) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        dlclose(handle5);
        dlclose(handle6);
        return;
    }
    createSlopeSafetyCalculator_t createSlopeSafetyCalculator = (createSlopeSafetyCalculator_t)dlsym(handle7, "createSlopeSafetyCalculator");
    destroySlopeSafetyCalculator_t destroySlopeSafetyCalculator = (destroySlopeSafetyCalculator_t)dlsym(handle7, "destroySlopeSafetyCalculator");
    calculateSafetyFactor_t calculateSafetyFactor = (calculateSafetyFactor_t)dlsym(handle7, "calculateSafetyFactor");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from stability library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        dlclose(handle5);
        dlclose(handle6);
        dlclose(handle7);
        return;
    }
    void* Dstability = create(getRandomNumber(1.0, 1.5), getRandomNumber(1.3, 2.0), getRandomNumber(25.0, 45.0), getRandomNumber(1.0, 2.0));
    lastDstability_result = calculate(Dstability);
    destroy(Dstability);

    void* safetyFactorObj = createSafetyFactor();
    lastSafety_result = calculateSafety(safetyFactorObj, getRandomNumber(10, 100), getRandomNumber(25, 45));
    destroySafetyFactor(safetyFactorObj);

    void* slope = createSlopeAngle(getRandomNumber(1.0, 1.5), getRandomNumber(25.0, 45.0));
    lastSlope_result = calculateAngle(slope);
    destroySlopeAngle(slope);
    //Работа с библиотекой Stability
    void* stability = createStability();
    lastStability_result = calcStability(stability, getRandomNumber(1.0, 2.0), getRandomNumber(25.0, 45.0));
    destroyStability(stability);

    void* tension = createTension();
    lastTension_result = calculateTension(tension, getRandomNumber(10.0, 100.0), getRandomNumber(25.0, 45.0));
    destroyTension(tension);

    void* statstability = createStaticStability(getRandomNumber(1.0, 1.5), getRandomNumber(1.3, 2.0), getRandomNumber(25.0, 45.0), getRandomNumber(1.0, 2.0), getRandomNumber(10.0, 100.0));
    double DelaminationR_result = calculateDelaminationResistance(statstability);
    double HorizontalF_result = calculateHorizontalForce(statstability);
    lastStabilityFa = calculateStabilityFactor(statstability);
    destroyStaticStability(statstability);

    void* slopeSafety = createSlopeSafetyCalculator(getRandomNumber(1.0, 1.5), getRandomNumber(1.3, 2.0), getRandomNumber(25.0, 45.0), getRandomNumber(1.0, 2.0));
    lastSlopeSafety_result = calculateSafetyFactor(slopeSafety);
    destroySlopeSafetyCalculator(slopeSafety);

    // Закрытие всех динамических библиотек
    dlclose(handle1);
    dlclose(handle2);
    dlclose(handle3);
    dlclose(handle4);
    dlclose(handle5);
    dlclose(handle6);
    dlclose(handle7);

    // Сохранение результатов в файл и базу данных
    saveToFile(lastDstability_result, lastSafety_result, lastSlope_result, lastStability_result, lastTension_result, lastStabilityFa, lastSlopeSafety_result);
    saveToDatabase(lastDstability_result, lastSafety_result, lastSlope_result, lastStability_result, lastTension_result, lastStabilityFa, lastSlopeSafety_result);
}

void updateSensors() {
    
    performCalculations();
    std::cout << "Данные сенсоров обновлены.\n";
}

void displaySensorData() {
    std::time_t now = std::time(0);
    std::cout << "Date and Time: " << std::ctime(&now)
              << "Dynamic Stability Factor: " << lastDstability_result << std::endl
              << "Safety Factor: " << lastSafety_result << std::endl
              << "Slope Angle: " << lastSlope_result << std::endl
              << "Stability: " << lastStability_result << std::endl
              << "Tension: " << lastTension_result << std::endl
              << "Stability Factor: " << lastStabilityFa << std::endl
              << "Slope Safety: " << lastSlopeSafety_result << std::endl
              << "---------------------------------------------" << std::endl;
}
int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(0)));

    int choice = 0;
    do {
        std::cout << "Меню:\n";
        std::cout << "1. Обновить данные с датчиков\n";
        std::cout << "2. Показать данные с датчиков\n";
        std::cout << "3. Выполнить расчеты и сохранить данные в текстовый файл и бд\n";
        std::cout << "4. Выход\n";
        std::cout << "Введите свой выбор: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                updateSensors();
                break;
            case 2:
                displaySensorData();
                break;
            case 3:
                performCalculations();
                break;
            case 4:
                std::cout << "Выход из программы.\n";
                break;
            default:
                std::cout << "Неверный выбор. Попробуйте еще раз.\n";
        }
    } while (choice != 4);

    return 0;
}
