#include <iostream>
#include <dlfcn.h>

// Типы функций для DynamicStability
typedef void* (*create_t)(double, double, double, double);
typedef void (*destroy_t)(void*);
typedef double (*calculate_t)(void*);

// Типы функций для библиотеки SafetyFactor
typedef void* (*createSafetyFactor_t)();
typedef void (*destroySafetyFactor_t)(void*);
typedef double (*calculateSafety_t)(void*, double, double);

// Типы функций для библиотеки SlopeAngle
typedef void* (*createSlopeAngle_t)(double, double);
typedef void (*destroySlopeAngle_t)(void*);
typedef double (*calculateAngle_t)(void*);

// Типы функций для библиотеки Stability
typedef void* (*createStability_t)(double,double);
typedef void (*destroyStability_t)(void*);
typedef double (*calcStability_t)(void*);

int main() {
    // Загружаем первую библиотеку
    void* handle1 = dlopen("./libdlyb.dylib", RTLD_LAZY);
    if (!handle1) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // Загружаем функции из первой библиотеки
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
        dlclose(handle1);
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
        dlclose(handle1);
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
    //мб ошибка
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
        return 1;
    }

    // Работа с DynamicStability
    void* Dstability = create(10.0, 20.0, 5.0, 2.0);
    double Dstability_result = calculate(Dstability);
    std::cout << "Dynamic Stability Factor: " << Dstability_result << std::endl;
    destroy(Dstability);

    // Работа с библиотекой SafetyFactor
    void* safetyFactorObj = createSafetyFactor();
    double safety_result = calculateSafety(safetyFactorObj, 100.0, 50.0);
    std::cout << "Safety Factor: " << safety_result << std::endl;
    destroySafetyFactor(safetyFactorObj);

    // Работа с библиотекой SlopeAngle
    void* slope = createSlopeAngle(32.5, 65.7);
    double slope_result = calculateAngle(slope);
    std::cout << "Slope Angle: " << slope_result << std::endl;
    destroySlopeAngle(slope);

    // Работа с библиотекой Stability
    void* stability = createStability(10.0,5.0);
    double stability_result = calcStability(stability);
    std::cout << "Stability: " << stability_result << std::endl;
    destroyStability(stability);
    
    // Закрываем библиотеки

    dlclose(handle1);
    dlclose(handle2);
    dlclose(handle3);
    dlclose(handle4);

    return 0;
}