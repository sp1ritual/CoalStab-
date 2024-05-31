#include <iostream>
#include <dlfcn.h>

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
typedef void* (*createStability_t)(double,double);
typedef void (*destroyStability_t)(void*);
typedef double (*calcStability_t)(void*);

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

typedef void* (*createSlopeSafety_t)(double,double,double,double,double,double);
typedef void* (*destroySlopeSafety_t)(void*);
typedef double (*calculateResistance_t)(void*);
typedef double (*calculateHorizontalFor_t)(void*);
typedef double (*calculateSafetyFactor_t)(void*);


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

    void* handle7 = dlopen("./libslopesafety.dylib", RTLD_LAZY);
    if (!handle7) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }
    createSlopeSafety_t createSlopeSafety = (createSlopeSafety_t)dlsym(handle7,"createSlopeSafety");
    destroySlopeSafety_t destroySlopeSafety = (destroySlopeSafety_t)dlsym(handle7,"destroySlopeSafety");
    calculateResistance_t calculateResistance = (calculateResistance_t)dlsym(handle7,"calculateResistance");
    calculateHorizontalFor_t calculateHorizontalFor = (calculateHorizontalFor_t)dlsym(handle7,"calculateHorizontalFor");
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
    void* Dstability = create(10.0, 20.0, 5.0, 2.0);
    double Dstability_result = calculate(Dstability);
    std::cout<<"------------------"<<std::endl;
    std::cout << "Dynamic Stability Factor: " << Dstability_result << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroy(Dstability);

    // Работа с библиотекой SafetyFactor
    void* safetyFactorObj = createSafetyFactor();
    double safety_result = calculateSafety(safetyFactorObj, 100.0, 50.0);
    std::cout << "Safety Factor: " << safety_result << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroySafetyFactor(safetyFactorObj);

    // Работа с библиотекой SlopeAngle
    void* slope = createSlopeAngle(32.5, 65.7);
    double slope_result = calculateAngle(slope);
    std::cout << "Slope Angle: " << slope_result << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroySlopeAngle(slope);

    // Работа с библиотекой Stability
    void* stability = createStability(10.0,5.0);
    double stability_result = calcStability(stability);
    std::cout << "Stability: " << stability_result << std::endl; // Проблемы в расчетах
    std::cout<<"------------------"<<std::endl;
    destroyStability(stability);

    // Работа с библиотекой Tensiondumps
    void* tension = createTension();
    double tension_result = calculateTension(tension,25.6,43.5);
    std::cout<<"Tension: "<<tension_result<<std::endl;
    destroyTension(tension);

    // Работа с либой StaticStability
    void* statstability = createStaticStability(10.0, 5.0, 2.0, 3.0, 4.0);
    double DelaminationR_result = calculateDelaminationResistance(statstability);
    double HorizontalF_result = calculateHorizontalForce(statstability);
    double StabilityFa = calculateStabilityFactor(statstability);
    std::cout<<"------------------"<<std::endl;
    std::cout << "Delamination Resistance: " << calculateDelaminationResistance(statstability) << std::endl;
    std::cout << "Horizontal Force: " << calculateHorizontalForce(statstability) << std::endl;
    std::cout << "Stability Factor: " << calculateStabilityFactor(statstability) << std::endl;
    std::cout<<"------------------"<<std::endl;
    destroyStaticStability(statstability);

    //Работа с либой SlopeSafety
    void* slopesafety = createSlopeSafety(2.0,3.0,1.5,2.6,4.7,5.3);
    double Resistantse_result = calculateResistance(slopesafety);
    double HorizontalForce = calculateHorizontalForce(slopesafety);
    double SafetyFac_result = calculateSafetyFactor(slopesafety);
    std::cout<<"Resistance: "<<calculateResistance(slopesafety)<<std::endl;
    std::cout<<"HorizontalForce: "<<calculateHorizontalForce(slopesafety)<<std::endl;
    std::cout<<"SafetyFactor: "<<calculateSafetyFactor(slopesafety)<<std::endl; //Проблемы в расчетах
    std::cout<<"------------------"<<std::endl;
    destroySlopeSafety(slopesafety);

    // Закрываем библиотеки

    dlclose(handle1);
    dlclose(handle2);
    dlclose(handle3);
    dlclose(handle4);
    dlclose(handle5);
    dlclose(handle6);
    dlclose(handle7);

    return 0;
}
