#include <unity.h>
#include "calculator.h"

#ifdef UNIT_TEST

void test_get_tdelta() {
  Calculator calc;
  calc.setT2(3.0);
  calc.setT3(5.0);
  TEST_ASSERT_EQUAL(2.0, calc.getTDelta());
}

void test_calculate_m1_m2_m3_with_t1_26_t2_25() {
  Calculator calc;
  calc.setT1(26.0);
  calc.setT2(25.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(true, calc.getM1());
  TEST_ASSERT_EQUAL(true, calc.getM2());
  TEST_ASSERT_EQUAL(true, calc.getM3());
}

void test_calculate_m1_m2_m3_with_t1_26_t2_26() {
  Calculator calc;
  calc.setT1(26.0);
  calc.setT2(26.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(false, calc.getM1());
  TEST_ASSERT_EQUAL(false, calc.getM2());
  TEST_ASSERT_EQUAL(false, calc.getM3());
}

void test_calculate_m1_m2_m3_with_t1_20_t2_15() {
  Calculator calc;
  calc.setT1(20.0);
  calc.setT2(15.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(false, calc.getM1());
  TEST_ASSERT_EQUAL(false, calc.getM2());
  TEST_ASSERT_EQUAL(false, calc.getM3());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_get_tdelta);
  RUN_TEST(test_calculate_m1_m2_m3_with_t1_26_t2_25);
  RUN_TEST(test_calculate_m1_m2_m3_with_t1_26_t2_26);
  RUN_TEST(test_calculate_m1_m2_m3_with_t1_20_t2_15);
  UNITY_END();
}

#endif
