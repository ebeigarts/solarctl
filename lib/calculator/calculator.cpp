#include "calculator.h"

void Calculator::setT1(float value) { t1 = value; }
void Calculator::setT2(float value) { t2 = value; }
void Calculator::setT3(float value) { t3 = value; }
void Calculator::setM1(float value) { m1 = value; }
void Calculator::setM2(float value) { m2 = value; }
void Calculator::setM3(float value) { m3 = value; }
void Calculator::setM4(float value) { m4 = value; }
void Calculator::setQ1(float value) { q1 = value; }
void Calculator::setS1(float value) { s1 = value; }

float Calculator::getT1() { return t1; }
float Calculator::getT2() { return t2; }
float Calculator::getT3() { return t3; }
float Calculator::getTDelta() { return t3 - t2; }
bool Calculator::getM1() { return m1; }
bool Calculator::getM2() { return m2; }
bool Calculator::getM3() { return m3; }
bool Calculator::getM4() { return m4; }
bool Calculator::getQ1() { return q1; }
bool Calculator::getS1() { return q1; }

#define SOLAR_MIN_TEMPERATURE         25.0 // T1, pie kuras mēģināt palaist sistēmu
#define SOLAR_DELTA_MIN_TEMPERATURE    3.0 // T3-T2, pie kura ir jādarbina sistēma
#define FLOOR_COMFORT_MIN_TEMPERATURE 40.0 // T2, pie kuras pieslēgt grīdas apkuri komforta režīmā
#define FLOOR_STANDBY_MIN_TEMPERATURE 30.0 // T2, pie kuras pieslēgt grīdas apkuri standby režīmā
#define HEATING_DELTA_MIN_TEMPERATURE  7.0 // T3-T2, pie kura pārsniegšanas jāieslēdz boilera sildītājs

void Calculator::calculate() {
  bool tDelta = getTDelta();
  bool solarEnable = t1 > SOLAR_MIN_TEMPERATURE && t1 > t2;
  bool heatingDiffOk = tDelta > SOLAR_DELTA_MIN_TEMPERATURE;
  bool solarActive = solarEnable || heatingDiffOk;

  m1 = solarEnable;
  m2 = solarEnable;
  m3 = solarEnable;
  m4 = (
    (!s1 && t2 > FLOOR_STANDBY_MIN_TEMPERATURE) ||
    (s1 && t2 > FLOOR_COMFORT_MIN_TEMPERATURE)
  );
  q1 = (
    (t2 < FLOOR_STANDBY_MIN_TEMPERATURE) ||
    (s1 && (
        (!solarActive) ||
        (solarActive && (tDelta > HEATING_DELTA_MIN_TEMPERATURE))
      )
    )
  );
}
