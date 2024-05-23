#include "mainheader.h"

class DynamicStability {
public:
    // Конструктор для инициализации значений
    DynamicStability(double cohesion, double area, double normalStress, double velocity) :
        resistance(cohesion), pressure(area), stressCoefficient(normalStress), velocity(velocity) {}

    double calculateStability() {
        double temporaryResistance = stressCoefficient * velocity;  // Используем временную переменную для вычисления сопротивления
        double stabilityFactor = temporaryResistance / pressure;    // Вычисление коэффициента устойчивости
        return stabilityFactor;
    }

private:
    double resistance, pressure, stressCoefficient, velocity;
};
