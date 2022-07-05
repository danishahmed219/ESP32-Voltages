#include "UnitCal.h"

UnitCal::UnitCal(double _consumedUnit) {
    consumedUnit = _consumedUnit;
    start_time = millis();
}

void UnitCal::recordUnit(double _watts) {
    current_time = millis();

    if(current_time - start_time > 1000) {
        start_time = millis();
        watts += _watts; //This _watts will change depends on the variation
        units = watts/3600000.0;

        measuredUnits = consumedUnit + units;
  }
}

double UnitCal::getUnits() {
    return measuredUnits;
}
