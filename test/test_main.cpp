#include <Arduino.h>
#include <unity.h>

#ifdef UNIT_TEST

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    // RUN_TEST(test_led_builtin_pin_number);
}

void loop() {
  UNITY_END(); // stop unit testing
}

#endif
