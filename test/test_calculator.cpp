#ifdef UNIT_TEST

#include <unity.h>
#include "calculator.h"

void test_get_tdelta() {
  Calculator calc;
  calc.setT2(3.0);
  calc.setT3(5.0);
  TEST_ASSERT_EQUAL(2.0, calc.getTDelta());
}

void test_calculate_with_t1_26_t2_25() {
  Calculator calc;
  calc.setT1(26.0);
  calc.setT2(25.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(true,  calc.getM1());
  TEST_ASSERT_EQUAL(true,  calc.getM2());
  TEST_ASSERT_EQUAL(true,  calc.getM3());
  TEST_ASSERT_EQUAL(false, calc.getM4());
  TEST_ASSERT_EQUAL(false, calc.getQ1());
}

void test_calculate_with_t1_26_t2_26() {
  Calculator calc;
  calc.setT1(26.0);
  calc.setT2(26.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(false, calc.getM1());
  TEST_ASSERT_EQUAL(false, calc.getM2());
  TEST_ASSERT_EQUAL(false, calc.getM3());
  TEST_ASSERT_EQUAL(false, calc.getM4());
  TEST_ASSERT_EQUAL(false, calc.getQ1());
}

void test_calculate_with_t1_20_t2_15() {
  Calculator calc;
  calc.setT1(20.0);
  calc.setT2(15.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(false, calc.getM1());
  TEST_ASSERT_EQUAL(false, calc.getM2());
  TEST_ASSERT_EQUAL(false, calc.getM3());
  TEST_ASSERT_EQUAL(false, calc.getM4());
  TEST_ASSERT_EQUAL(false, calc.getQ1());
}

void test_calculate_with_t1_31_t2_31_t3_31() {
  Calculator calc;
  calc.setT1(31.0);
  calc.setT2(31.0);
  calc.setT3(31.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(false, calc.getM1());
  TEST_ASSERT_EQUAL(false, calc.getM2());
  TEST_ASSERT_EQUAL(false, calc.getM3());
  TEST_ASSERT_EQUAL(false, calc.getM4());
  TEST_ASSERT_EQUAL(false, calc.getQ1());
}

void test_calculate_with_t1_31_t2_31_t3_31_s1_on() {
  Calculator calc;
  calc.setT1(31.0);
  calc.setT2(31.0);
  calc.setT3(31.0);
  calc.setS1(true);
  calc.calculate();
  TEST_ASSERT_EQUAL(false, calc.getM1());
  TEST_ASSERT_EQUAL(false, calc.getM2());
  TEST_ASSERT_EQUAL(false, calc.getM3());
  TEST_ASSERT_EQUAL(false, calc.getM4());
  TEST_ASSERT_EQUAL(true,  calc.getQ1());
}

void test_calculate_with_t1_41_t2_31_t3_31() {
  Calculator calc;
  calc.setT1(41.0);
  calc.setT2(31.0);
  calc.setT3(31.0);
  calc.calculate();
  TEST_ASSERT_EQUAL(true,  calc.getM1());
  TEST_ASSERT_EQUAL(true,  calc.getM2());
  TEST_ASSERT_EQUAL(true,  calc.getM3());
  TEST_ASSERT_EQUAL(true,  calc.getM4());
  TEST_ASSERT_EQUAL(false, calc.getQ1());
}

void test_calculate_with_t1_41_t2_31_t3_31_s1_on() {
  Calculator calc;
  calc.setT1(41.0);
  calc.setT2(31.0);
  calc.setT3(31.0);
  calc.setS1(true);
  calc.calculate();
  TEST_ASSERT_EQUAL(true,  calc.getM1());
  TEST_ASSERT_EQUAL(true,  calc.getM2());
  TEST_ASSERT_EQUAL(true,  calc.getM3());
  TEST_ASSERT_EQUAL(false, calc.getM4());
  TEST_ASSERT_EQUAL(false, calc.getQ1());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_get_tdelta);
  RUN_TEST(test_calculate_with_t1_26_t2_25);
  RUN_TEST(test_calculate_with_t1_26_t2_26);
  RUN_TEST(test_calculate_with_t1_20_t2_15);
  RUN_TEST(test_calculate_with_t1_31_t2_31_t3_31);
  RUN_TEST(test_calculate_with_t1_31_t2_31_t3_31_s1_on);
  RUN_TEST(test_calculate_with_t1_41_t2_31_t3_31);
  RUN_TEST(test_calculate_with_t1_41_t2_31_t3_31_s1_on);
  UNITY_END();
}

#endif
