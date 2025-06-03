#include <unity.h>
#include "../lib/logic/logic.h"

void setUp(void) {
  extern unsigned long count;
  count = 0;
  debouncedPress(0UL, false);
}

void tearDown(void) {}

void test_no_press_before_debounce(void) {
  bool pressDetected = debouncedPress(1UL, true);
  TEST_ASSERT_FALSE(pressDetected);
}

void test_single_press_after_debounce(void) {
  bool pressDetected = debouncedPress(51UL, true);
  TEST_ASSERT_TRUE(pressDetected);

  bool immediateRepeat = debouncedPress(52UL, true);
  TEST_ASSERT_FALSE(immediateRepeat);
}

void test_press_after_release_and_wait(void) {
  bool firstPress = debouncedPress(60UL, true);
  TEST_ASSERT_TRUE(firstPress);

  bool releaseEdge = debouncedPress(111UL, false);
  TEST_ASSERT_FALSE(releaseEdge);

  bool tooSoon = debouncedPress(151UL, true);
  TEST_ASSERT_FALSE(tooSoon);

  bool secondPress = debouncedPress(162UL, true);
  TEST_ASSERT_TRUE(secondPress);
}

void test_press_handler_increments_count(void) {
  extern unsigned long count;
  TEST_ASSERT_EQUAL_UINT32(0, count);

  pressHandler();
  TEST_ASSERT_EQUAL_UINT32(1, count);

  pressHandler();
  pressHandler();
  TEST_ASSERT_EQUAL_UINT32(3, count);
}

int main(int argc, char** argv) {
  UNITY_BEGIN();

  RUN_TEST(test_no_press_before_debounce);
  RUN_TEST(test_single_press_after_debounce);
  RUN_TEST(test_press_after_release_and_wait);
  RUN_TEST(test_press_handler_increments_count);

  return UNITY_END();
}
