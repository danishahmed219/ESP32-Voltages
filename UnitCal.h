#ifndef UNITCAL_H
#define UNITCAL_H

#include <Arduino.h>

class UnitCal {

public:
    UnitCal(double _consumedUnit);
    void recordUnit(double _watts);
    double getUnits();

private:
    int offset = 0;
    unsigned long start_time = 0;
    unsigned long current_time = 0;
    unsigned long watts = 0000;
    double units = 0;
    double consumedUnit = 0;
    double measuredUnits = 0;
};

#endif
