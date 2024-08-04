#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) { return x > 42; }

bool is_vowel(char c) {
  char *vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  // TODO: Implement this function.
  char test_tail_1 = 'w';
  bool output_1 = is_tail(test_tail_1);
  if (!assert_true("test_tail_1", output_1)) {
    return false;
  }
  
  char test_tail_2 = 'q';
  bool output_2 = is_tail(test_tail_2);
  if (!assert_false("test_tail_2", output_2)) {
    return false;
  }

  return true;
}

bool test_is_head() {
  // TODO: Implement this function.
  char test_head_1 = 'W';
  bool output_1 = is_head(test_head_1);
  if (!assert_true("test_head_1", output_1)) {
    return false;
  }
  
  char test_head_2 = 'q';
  bool output_2 = is_head(test_head_2);
  if (!assert_false("test_head_2", output_2)) {
    return false;
  }

  return true;
}

bool test_is_snake() {
  // TODO: Implement this function.
  char test_snake_1 = 'W';
  bool output_1 = is_head(test_snake_1);
  if (!assert_true("test_snake_1", output_1)) {
    return false;
  }

  char test_snake_2 = 'q';
  bool output_2 = is_head(test_snake_2);
  if (!assert_false("test_snake_2", output_2)) {
    return false;
  }
  return true;
}

bool test_body_to_tail() {
  // TODO: Implement this function.
  char test_body_to_tail_1 = '^';
  char tail_1 = 'w';
  char output_1 = body_to_tail(test_body_to_tail_1);
  if (!assert_true("test_body_to_tail_1", strcmp(&tail_1, &output_1))) {
    return false;
  }

  char test_body_to_tail_2 = '1';
  char tail_2 = 'w';
  char output_2 = body_to_tail(test_body_to_tail_2);
  if (!assert_false("test_body_to_tail_2", strcmp(&tail_2, &output_2) == 0)) {
    return false;
  }
  return true;
}

bool test_head_to_body() {
  // TODO: Implement this function.
  char test_head_to_body_1 = 'W';
  char body_1 = '^';
  char output_1 = head_to_body(test_head_to_body_1);
  if(!assert_true("test_head_to_body_1", strcmp(&body_1, &output_1))) {
    return false;
  }
  return true;
}

bool test_get_next_col() {
  // TODO: Implement this function.
  unsigned int cur_col = 12;
  char head = 'D';
  unsigned int next_col = get_next_col(cur_col, head);
  // printf("%d\n", next_col);
  if(!assert_true("test_get_next_col_1", next_col == 13)) {
    return false;
  }

  head = 'W';
  next_col = get_next_col(cur_col, head);
  if(!assert_true("test_get_next_col_2", next_col == 12)) {
    return false;
  }

  return true;
}

bool test_get_next_row() {
  // TODO: Implement this function.
  unsigned int cur_row = 12;
  char head = '^';
  unsigned int next_row = get_next_row(cur_row, head);
  // printf("%d\n", next_row);
  if(!assert_true("test_get_next_row_1", next_row == 11)) {
    return false;
  }

  head = 'D';
  next_row = get_next_row(cur_row, head);
  if(!assert_true("test_get_next_row_2", next_row == 12)) {
    return false;
  }

  return true;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col failed");
    return false;
  }
  return true;
}

int main(int argc, char *argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
