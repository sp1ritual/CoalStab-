#include <iostream>
#include <cmath>


class SlopeSafetyCalculator {
public:
    SlopeSafetyCalculator(double cohesion, double unitWeight, double area, double baseStress)
    : cohesion_(cohesion), unitWeight_(unitWeight), area_(area), baseStress_(baseStress) {}

    double calculateNormalStress() const {
        return unitWeight_ * area_;
    }

    double calculateSoilResistance() const {
        double normalStress = calculateNormalStress();
        return cohesion_ * normalStress * area_;
    }

    double calculateSafetyFactor() const {
        double soilResistance = calculateSoilResistance();
        if (baseStress_ != 0) {
            return soilResistance / baseStress_;
        } else {
            std::cerr << "Error: Division by zero in calculating safety factor." << std::endl;
            return 0;
        }
    }

private:
    double cohesion_;    // Когезия почвы
    double unitWeight_;  // Удельный вес почвы
    double area_;        // Площадь сечения склона
    double baseStress_;  // Основное напряжение для расчета фактора безопасности
};
extern "C" {
    SlopeSafetyCalculator* createSlopeSafetyCalculator(double cohesion, double unitWeight, double area, double baseStress) {
        return new SlopeSafetyCalculator(cohesion, unitWeight, area, baseStress);
    }

    void destroySlopeSafetyCalculator(SlopeSafetyCalculator* obj) {
        delete obj;
    }

    double calculateSafetyFactor(SlopeSafetyCalculator* obj) {
        return obj->calculateSafetyFactor();
    }
}
