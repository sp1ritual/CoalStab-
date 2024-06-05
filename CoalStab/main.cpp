#include <iostream>
#include <dlfcn.h>
#include <fstream>
#include <ctime>
#include <chrono>
#include <random>
#include <sqlite3.h>


// либа DynamicStability
typedef void* (*create_t)(double, double, double, double);
typedef void (*destroy_t)(void*);
typedef double (*calculate_t)(void*);

// либа SafetyFactor
typedef void* (*createSafetyFactor_t)();
typedef void (*destroySafetyFactor_t)(void*);
typedef double (*calculateSafety_t)(void*, double, double);

// либа SlopeAngle
typedef void* (*createSlopeAngle_t)(double, double);
typedef void (*destroySlopeAngle_t)(void*);
typedef double (*calculateAngle_t)(void*);

// либа Stability
typedef void* (*createStability_t)();
typedef void (*destroyStability_t)(void*);
typedef double (*calcStability_t)(void*,double,double);

// либа Tensiondumps
typedef void* (*createTension_t)();
typedef void (*destroyTension_t)(void*);
typedef double (*calculateTension_t)(void*,double,double);

// либа StaticStability
typedef void* (*createStaticStability_t)(double,double,double,double,double);
typedef void (*destroyStaticStability_t)(void*);
typedef double (*calculateDelaminationResistance_t)(void*);
typedef double (*calculateHorizontalForce_t)(void*);
typedef double (*calculateStabilityFactor_t)(void*);

// либа SlopeSafety
typedef void* (*createSlopeSafetyCalculator_t)(double,double,double,double);
typedef void* (*destroySlopeSafetyCalculator_t)(void*);
typedef double (*calculateSafetyFactor_t)(void*);

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
int main() {
    // Загружаем либу DynamicStability
    void* handle1 = dlopen("./libdlyb.dylib", RTLD_LAZY);
    if (!handle1) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // Загружаем функции из DynamicStability
    create_t create = (create_t)dlsym(handle1, "create");
    destroy_t destroy = (destroy_t)dlsym(handle1, "destroy");
    calculate_t calculate = (calculate_t)dlsym(handle1, "calculate");

    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from first library: " << dlsym_error << '\n';
        dlclose(handle1);
        return 1;
    }

    // Загружаем библиотеку SafetyFactor
    void* handle2 = dlopen("./libsafetyfactor.dylib", RTLD_LAZY);
    if (!handle2) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // Загружаем функции из библиотеки SafetyFactor
    createSafetyFactor_t createSafetyFactor = (createSafetyFactor_t)dlsym(handle2, "createSafetyFactor");
    destroySafetyFactor_t destroySafetyFactor = (destroySafetyFactor_t)dlsym(handle2, "destroySafetyFactor");
    calculateSafety_t calculateSafety = (calculateSafety_t)dlsym(handle2, "calculateSafety");

    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from safety factor library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        return 1;
    }

    // Загрузка библиотеки SlopeAngle
    void* handle3 = dlopen("./libslopeangle.dylib", RTLD_LAZY);
    if (!handle3) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
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
        return 1;
    }

    // Загрузка библиотеки Stability
    void* handle4 = dlopen("./libstability.dylib", RTLD_LAZY);
    if (!handle4) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    createStability_t createStability = (createStability_t)dlsym(handle4,"createStability");
    destroyStability_t destroyStability = (destroyStability_t)dlsym(handle4,"destroyStability");
    calcStability_t calcStability = (calcStability_t)dlsym(handle4, "calcStability");
    
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from stability library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        return 1;
    }
    // Загрузка либы Tensiondumps
    void* handle5 = dlopen("./libtension.dylib", RTLD_LAZY);
    if (!handle5) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
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
        return 1;
    }
    // Загрузка либы StaticStability
    void* handle6 = dlopen("./libstaticstability.dylib", RTLD_LAZY);
    if (!handle6) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    createStaticStability_t createStaticStability = (createStaticStability_t)dlsym(handle6,"createStaticStability");
    destroyStaticStability_t destroyStaticStability = (destroyStaticStability_t)dlsym(handle6,"destroyStaticStability");
    calculateDelaminationResistance_t calculateDelaminationResistance = (calculateDelaminationResistance_t)dlsym(handle6,"calculateDelaminationResistance");
    calculateHorizontalForce_t calculateHorizontalForce = (calculateHorizontalForce_t)dlsym(handle6,"calculateHorizontalForce");
    calculateStabilityFactor_t calculateStabilityFactor = (calculateStabilityFactor_t)dlsym(handle6,"calculateStabilityFactor");
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol from stability library: " << dlsym_error << '\n';
        dlclose(handle1);
        dlclose(handle2);
        dlclose(handle3);
        dlclose(handle4);
        dlclose(handle5);
        dlclose(handle6);
        return 1;
    }
    // Загружаем либы SlopeSafety
    void* handle7 = dlopen("./libslopesafety.dylib", RTLD_LAZY);
    if (!handle7) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    createSlopeSafetyCalculator_t createSlopeSafetyCalculator = (createSlopeSafetyCalculator_t)dlsym(handle7,"createSlopeSafetyCalculator");
    destroySlopeSafetyCalculator_t destroySlopeSafetyCalculator = (destroySlopeSafetyCalculator_t)dlsym(handle7,"destroySlopeSafetyCalculator");
    calculateSafetyFactor_t calculateSafetyFactor = (calculateSafetyFactor_t)dlsym(handle7,"calculateSafetyFactor");

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
        return 1;
    }
    // Работа с DynamicStability
    void* Dstability = create(getRandomNumber(0.1,10.0),getRandomNumber(0.4,10.0),getRandomNumber(0,15.0),getRandomNumber(1.0,15.0));
    double Dstability_result = calculate(Dstability);
    std::cout<<"------------------"<<std::endl;
    std::cout << "Dynamic Stability Factor: " << Dstability_result << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroy(Dstability);

    // Работа с библиотекой SafetyFactor
    void* safetyFactorObj = createSafetyFactor();
    double safety_result = calculateSafety(safetyFactorObj,getRandomNumber(0,100),getRandomNumber(20,50));
    std::cout << "Safety Factor: " << safety_result << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroySafetyFactor(safetyFactorObj);

    // Работа с библиотекой SlopeAngle
    void* slope = createSlopeAngle(getRandomNumber(0.1,3.9),getRandomNumber(0.119,12.7));
    double slope_result = calculateAngle(slope);
    std::cout << "Slope Angle: " << slope_result << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroySlopeAngle(slope);

    // Работа с библиотекой Stability
    void* stability = createStability();
    double stability_result = calcStability(stability,getRandomNumber(1.0,100.0),getRandomNumber(6.0,47.0));
    std::cout << "Stability: " << stability_result << std::endl; // Проблемы в расчетах
    std::cout<<"------------------"<<std::endl;
    destroyStability(stability);

    // Работа с библиотекой Tensiondumps
    void* tension = createTension();
    double tension_result = calculateTension(tension,getRandomNumber(0.1,27.0),getRandomNumber(0.2,12.0));
    std::cout<<"Tension: "<<tension_result<<std::endl;
    destroyTension(tension);

    // Работа с либой StaticStability
    void* statstability = createStaticStability(getRandomNumber(0.1,14.3),getRandomNumber(0.3,4.5),getRandomNumber(0.1,13.5),getRandomNumber(0.1,2.7),getRandomNumber(0.2,3.5));
    double DelaminationR_result = calculateDelaminationResistance(statstability);
    double HorizontalF_result = calculateHorizontalForce(statstability);
    double StabilityFa = calculateStabilityFactor(statstability);
    std::cout<<"------------------"<<std::endl;
    std::cout << "Stability Factor: " << calculateStabilityFactor(statstability) << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroyStaticStability(statstability);

    // Работа с библиотекой SlopeSafety
    void* slopeSafety = createSlopeSafetyCalculator(getRandomNumber(0.1,4.5),getRandomNumber(0.1,9.8),getRandomNumber(0.2,5.6),getRandomNumber(0.1,2.5));
    double slopeSafety_result = calculateSafetyFactor(slopeSafety);
    std::cout<<"Slope safety: "<<slopeSafety_result<<std::endl;
    std::cout<<"------------------"<<std::endl;
    
    // Закрываем библиотеки
    
    dlclose(handle1);
    dlclose(handle2);
    dlclose(handle3);
    dlclose(handle4);
    dlclose(handle5);
    dlclose(handle6);
    dlclose(handle7);

    saveToFile(Dstability_result, safety_result, slope_result, stability_result, tension_result, StabilityFa, slopeSafety_result);
    saveToDatabase(Dstability_result, safety_result, slope_result, stability_result, tension_result, StabilityFa, slopeSafety_result);
    return 0;
}
