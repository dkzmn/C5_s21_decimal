#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"

START_TEST(s21_fromto_test_1) {
  printf("Start from/to tests.\n");
  int x1 = 0, x2 = 0, res;
  s21_decimal a;
  for (x1 = -10000; x1 <= 10000; x1++) {
    s21_from_int_to_decimal(x1, &a);
    res = s21_from_decimal_to_int(a, &x2);
    ck_assert_int_eq(x1, x2);
    ck_assert_int_eq(res, 0);
  }
  for (x1 = INT_MIN + 1; x1 <= INT_MIN + 10000; x1++) {
    s21_from_int_to_decimal(x1, &a);
    res = s21_from_decimal_to_int(a, &x2);
    ck_assert_int_eq(x1, x2);
    ck_assert_int_eq(res, 0);
  }
  for (x1 = INT_MAX - 10000; x1 < INT_MAX; x1++) {
    s21_from_int_to_decimal(x1, &a);
    res = s21_from_decimal_to_int(a, &x2);
    ck_assert_int_eq(x1, x2);
    ck_assert_int_eq(res, 0);
  }
}

END_TEST

START_TEST(s21_fromto_test_2) {
  float x1 = 0, x2 = 0, res;
  char sx1[15], sx2[15];
  s21_decimal a;
  for (x1 = 1e-27; x1 <= 1e+28; x1 *= 10) {
    s21_from_float_to_decimal(x1, &a);
    res = s21_from_decimal_to_float(a, &x2);
    sprintf(sx1, "%.6e", x1);
    sprintf(sx2, "%.6e", x2);
    ck_assert_pstr_eq(sx1, sx2);
    ck_assert_int_eq(res, 0);
  }
  for (x1 = -1.1e-27; x1 > -1.1e+28; x1 *= 10) {
    s21_from_float_to_decimal(x1, &a);
    res = s21_from_decimal_to_float(a, &x2);
    sprintf(sx1, "%.6e", x1);
    sprintf(sx2, "%.6e", x2);
    ck_assert_pstr_eq(sx1, sx2);
    ck_assert_int_eq(res, 0);
  }
  for (x1 = 1.234567e-21; x1 <= 1.234567e+28; x1 *= 10) {
    s21_from_float_to_decimal(x1, &a);
    res = s21_from_decimal_to_float(a, &x2);
    sprintf(sx1, "%.6e", x1);
    sprintf(sx2, "%.6e", x2);
    ck_assert_pstr_eq(sx1, sx2);
    ck_assert_int_eq(res, 0);
  }
  for (x1 = 1.23456789e-21; x1 <= 1.23456789e+28; x1 *= 10) {
    s21_from_float_to_decimal(x1, &a);
    res = s21_from_decimal_to_float(a, &x2);
    // s21_print(a);
    sprintf(sx1, "%.6e", x1);
    sprintf(sx2, "%.6e", x2);
    ck_assert_pstr_eq(sx1, sx2);
    ck_assert_int_eq(res, 0);
  }
}
END_TEST

START_TEST(s21_fromto_test_3) {
  s21_decimal a;
  ck_assert_int_eq(s21_from_int_to_decimal(0, NULL), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(1e-29, &a), 1);
  ck_assert_int_eq(s21_from_float_to_decimal(1e+29, &a), 1);
}
END_TEST

START_TEST(s21_add_test_1) {
  printf("Start add tests.\n");
  int x1 = 0, x2 = 0, x3 = 0, res;
  s21_decimal a1, a2, a3;
  for (x1 = -50; x1 <= 50; x1++) {
    for (x2 = -50; x2 <= 50; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_add(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 + x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = -10050; x1 <= -9950; x1++) {
    for (x2 = -10050; x2 <= -9950; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_add(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 + x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = 9950; x1 <= 10050; x1++) {
    for (x2 = 9950; x2 <= 10050; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_add(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 + x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
}
END_TEST

START_TEST(s21_add_test_2) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("12345678901234567890", &value_1);
  s21_decs2decimal("55555555555555555555", &value_2);
  s21_set_scale(&value_2, 20);
  s21_decs2decimal("12345678901234567890555555556", &result_control);
  s21_set_scale(&result_control, 9);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_add_test_3) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal("1", &value_2);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_add_test_4) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_sign(&value_1, 1);
  s21_set_sign(&value_2, 1);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_add_test_5) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_2, 1);
  s21_decs2decimal(MAX_DECIMAL, &result_control);
  int res = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_sub_test_1) {
  printf("Start sub tests.\n");
  int x1 = 0, x2 = 0, x3 = 0, res;
  s21_decimal a1, a2, a3;
  for (x1 = -50; x1 <= 50; x1++) {
    for (x2 = -50; x2 <= 50; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_sub(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 - x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = -10050; x1 <= -9950; x1++) {
    for (x2 = -10050; x2 <= -9950; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_sub(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 - x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = 9950; x1 <= 10050; x1++) {
    for (x2 = 9950; x2 <= 10050; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_sub(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 - x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
}
END_TEST

START_TEST(s21_sub_test_2) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("12345678901234567890", &value_1);
  s21_decs2decimal("55555555555555555555", &value_2);
  s21_set_scale(&value_2, 20);
  s21_decs2decimal("12345678901234567889444444444", &result_control);
  s21_set_scale(&result_control, 9);
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_sub_test_3) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_set_sign(&value_1, 1);
  s21_decs2decimal("1", &value_2);
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_sub_test_4) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("9999999999999999999999999", &value_1);
  s21_decs2decimal("5555555555555555555555555", &value_2);
  s21_set_sign(&value_1, 1);
  s21_set_scale(&value_2, 25);
  s21_decs2decimal("9999999999999999999999999556", &result_control);
  s21_set_scale(&result_control, 3);
  s21_set_sign(&result_control, 1);
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_sub_test_5) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("1", &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_2, 28);
  s21_decs2decimal("9999999999999999999999999999", &result_control);
  s21_set_scale(&result_control, 28);
  int res = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_mul_test_1) {
  printf("Start mul tests.\n");
  int x1 = 0, x2 = 0, x3 = 0, res;
  s21_decimal a1, a2, a3;
  for (x1 = -50; x1 <= 50; x1++) {
    for (x2 = -50; x2 <= 50; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_mul(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      // printf("%d %d %d\n",x1,x2,x3);
      ck_assert_int_eq(x1 * x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = -10050; x1 <= -9950; x1++) {
    for (x2 = -10050; x2 <= -9950; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_mul(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 * x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = 9950; x1 <= 10050; x1++) {
    for (x2 = 9950; x2 <= 10050; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_mul(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      ck_assert_int_eq(x1 * x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
}
END_TEST

START_TEST(s21_mul_test_2) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("1000000000000000000000000000", &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_2, 28);
  s21_decs2decimal("1", &result_control);
  s21_set_scale(&result_control, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_mul_test_3) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_set_sign(&value_1, 1);
  s21_decs2decimal(MAX_DECIMAL, &value_2);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_mul_test_4) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal("1", &value_1);
  s21_set_sign(&value_1, 1);
  s21_set_scale(&value_1, 14);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_2, 15);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(s21_mul_test_5) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("1111111111111111111111111111", &value_1);
  s21_decs2decimal("6", &value_2);
  s21_set_sign(&value_2, 1);
  s21_set_scale(&value_1, 10);
  s21_decs2decimal("6666666666666666666666666666", &result_control);
  s21_set_scale(&result_control, 10);
  s21_set_sign(&result_control, 1);
  int res = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_div_test_1) {
  printf("Start div tests.\n");
  int x1 = 0, x2 = 0, res;
  float x3;
  s21_decimal a1, a2, a3;
  for (x1 = -5; x1 <= 5; x1++) {
    for (x2 = -5; x2 <= 5; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_div(a1, a2, &a3);
      s21_from_decimal_to_float(a3, &x3);
      if (x2 != 0) {
        ck_assert_float_eq((float)x1 / x2, x3);
        ck_assert_int_eq(res, 0);
      } else {
        ck_assert_int_eq(res, 3);
      }
    }
  }
  for (x1 = -12345; x1 <= -12340; x1++) {
    for (x2 = -12345; x2 <= -12340; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_div(a1, a2, &a3);
      s21_from_decimal_to_float(a3, &x3);
      ck_assert_float_eq((float)x1 / x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
  for (x1 = 54321; x1 <= 54326; x1++) {
    for (x2 = 54321; x2 <= 54326; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_div(a1, a2, &a3);
      s21_from_decimal_to_float(a3, &x3);
      ck_assert_float_eq((float)x1 / x2, x3);
      ck_assert_int_eq(res, 0);
    }
  }
}
END_TEST

START_TEST(s21_div_test_2) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("1", &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_2, 28);
  s21_decs2decimal("10000000000000000000000000000", &result_control);
  int res = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_div_test_3) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal(MAX_DECIMAL, &value_2);
  s21_decs2decimal("1", &result_control);
  int res = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_div_test_4) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal("1", &value_1);
  s21_decs2decimal("100", &value_2);
  s21_set_scale(&value_1, 28);
  int res = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(res, 2);
}
END_TEST

START_TEST(s21_div_test_5) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_1, 2);
  s21_set_scale(&value_2, 5);
  int res = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_div_test_6) {
  s21_decimal value_1, value_2, result;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal("1", &value_2);
  s21_set_scale(&value_2, 28);
  int res = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_mod_test_1) {
  printf("Start mod tests.\n");
  int x1 = 0, x2 = 0, x3 = 0, res, res_control;
  s21_decimal a1, a2, a3;
  for (x1 = -5; x1 <= 5; x1++) {
    for (x2 = -5; x2 <= 5; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_mod(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      if (x2 != 0) {
        res_control = x1 % x2;
        ck_assert_int_eq(res_control, x3);
        ck_assert_int_eq(res, 0);
      } else {
        ck_assert_int_eq(res, 3);
      }
    }
  }
  for (x1 = INT_MIN + 1; x1 <= INT_MIN + 10; x1++) {
    for (x2 = -5; x2 <= 5; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_mod(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      if (x2 != 0) {
        res_control = x1 % x2;
        ck_assert_int_eq(res_control, x3);
        ck_assert_int_eq(res, 0);
      } else {
        ck_assert_int_eq(res, 3);
      }
    }
  }
  for (x1 = INT_MAX - 10; x1 <= INT_MAX - 1; x1++) {
    for (x2 = -5; x2 <= 5; x2++) {
      s21_from_int_to_decimal(x1, &a1);
      s21_from_int_to_decimal(x2, &a2);
      res = s21_mod(a1, a2, &a3);
      s21_from_decimal_to_int(a3, &x3);
      if (x2 != 0) {
        res_control = x1 % x2;
        ck_assert_int_eq(res_control, x3);
        ck_assert_int_eq(res, 0);
      } else {
        ck_assert_int_eq(res, 3);
      }
    }
  }
}
END_TEST

START_TEST(s21_mod_test_2) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal("3", &value_2);
  s21_set_scale(&value_2, 2);
  s21_decs2decimal("0", &result_control);
  int res = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_mod_test_3) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal("100000000000000", &value_1);
  s21_decs2decimal("3", &value_2);
  s21_set_scale(&value_2, 13);
  s21_decs2decimal("1", &result_control);
  s21_set_scale(&result_control, 13);
  int res = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_mod_test_4) {
  s21_decimal value_1, value_2, result, result_control;
  s21_decs2decimal(MAX_DECIMAL, &value_1);
  s21_decs2decimal(MAX_DECIMAL, &value_2);
  s21_decs2decimal("0", &result_control);
  int res = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(s21_is_equal(result, result_control), 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_0) {
  printf("Start compare tests.\n");
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("0", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("0", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  // 1 > 1 = false (0)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // 1 >= 1 = true (1)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // 1 == 1 = true (1)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // 1 < 1 = false (0)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // 1 <= 1 = true (1)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // 1 != 1 = false (0)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 1);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 0);
}
END_TEST

START_TEST(s21_decimal_is_greater_1) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // 200 / 10^2 = 2
  s21_decs2decimal("200", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 2);
  s21_set_sign(&s_decimal_1, 0);
  // 2
  s21_decs2decimal("2", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  // 2 > 2 = false (0)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // 2 >= 2 = true (1)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // 2 == 2 = true (1)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // 2 < 2 = false (0)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // 2 <= 2 = true (1)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // 2 != 2 = false (0)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 1);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 0);
}
END_TEST

START_TEST(s21_decimal_is_greater_2) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // 100
  s21_decs2decimal("100", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  // 50
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  // 100 > 50 = true (1)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // 100 >= 50 = true (1)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // 100 == 50 = false (0)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // 100 < 50 = false (0)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // 100 <= 50 = false (0)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // 100 != 50 = true (1)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 1);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 0);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_3) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // -100
  s21_decs2decimal("100", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 1);
  // -50
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 1);
  // -100 > -50 = false (0)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // -100 >= -50 = false (0)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // -100 == -50 = false (0)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // -100 < -50 = true (1)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // -100 <= -50 = true (1)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // -100 != -50 = true (1)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 0);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 1);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_4) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // 79228162514264337593543950335 (MAX_DECIMAL)
  s21_decs2decimal("79228162514264337593543950335", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  // 1
  s21_decs2decimal("1", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  // MAX_DECIMAL > 1 = true (1)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // MAX_DECIMAL >= 1 = true (1)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // MAX_DECIMAL == 1 = false (0)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // MAX_DECIMAL < 1 = false (0)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // MAX_DECIMAL <= 1 = false (0)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // MAX_DECIMAL != 1 = true (1)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 1);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 0);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_5) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // 7.9228162514264337593543950335
  s21_decs2decimal("79228162514264337593543950335", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 28);
  s21_set_sign(&s_decimal_1, 0);
  // 1
  s21_decs2decimal("8", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  // 7.9 > 8 = false (0)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // 7.9 >= 8 = false (0)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // 7.9 = 8 = false (0)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // 7.9 < 8 = true (1)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // 7.9 <= 8 = true (1)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // 7.9 != 8 = true (1)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 0);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 1);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_6) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // 0
  s21_decs2decimal("0", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  // -0
  s21_decs2decimal("0", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 1);
  // 0 > -0 = true (1)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // 0 >= -0 = true (1)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // 0 = -0 = false (0)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // 0 < -0 = false (0)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // 0 <= -0 = false (0)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // 0 != -0 = true (1)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 1);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 0);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_7) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  // 0.5
  s21_decs2decimal("5000", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 4);
  s21_set_sign(&s_decimal_1, 1);
  // -5
  s21_decs2decimal("500", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 2);
  s21_set_sign(&s_decimal_2, 1);
  // 0.5 > -5 = true (1)
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  // 0.5 >= -5 = true (1)
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  // 0.5 = -5 = false (0)
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  // 0.5 < -5 = false (0)
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  // 0.5 <= -5 = false (0)
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  // 0.5 != -5 = true (1)
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 1);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 0);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_8) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("5000", &s_decimal_1);
  s_decimal_1.bits[2] = 1;
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 1);
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 0);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 1);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_9) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("5000", &s_decimal_1);
  s_decimal_1.bits[1] = 1;
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 1);
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 0);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 1);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_10) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("5000", &s_decimal_1);
  s_decimal_1.bits[1] = 1;
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 1);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 0);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_11) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("50", &s_decimal_1);
  s_decimal_1.bits[1] = 1;
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 0);
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 0);
  ck_assert_int_eq(res2, 0);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 1);
  ck_assert_int_eq(res5, 1);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_decimal_is_greater_12) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("50", &s_decimal_1);
  s_decimal_1.bits[1] = 1;
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("50", &s_decimal_2);
  s21_set_scale(&s_decimal_2, 0);
  s21_set_sign(&s_decimal_2, 1);
  int res1 = s21_is_greater(s_decimal_1, s_decimal_2);
  int res2 = s21_is_greater_or_equal(s_decimal_1, s_decimal_2);
  int res3 = s21_is_equal(s_decimal_1, s_decimal_2);
  int res4 = s21_is_less(s_decimal_1, s_decimal_2);
  int res5 = s21_is_less_or_equal(s_decimal_1, s_decimal_2);
  int res6 = s21_is_not_equal(s_decimal_1, s_decimal_2);
  ck_assert_int_eq(res1, 1);
  ck_assert_int_eq(res2, 1);
  ck_assert_int_eq(res3, 0);
  ck_assert_int_eq(res4, 0);
  ck_assert_int_eq(res5, 0);
  ck_assert_int_eq(res6, 1);
}
END_TEST

START_TEST(s21_truncate_1) {
  printf("Start truncate tests\n");
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("0", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("0", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 0);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}

START_TEST(s21_truncate_2) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("5", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 1);
  s21_decs2decimal("0", &s_decimal_3);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_decs2decimal("1", &s_decimal_3);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}

START_TEST(s21_truncate_3) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("85123", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 3);
  s21_decs2decimal("85", &s_decimal_3);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}

START_TEST(s21_truncate_4) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("1005555000", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 7);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("100", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_decs2decimal("101", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}

START_TEST(s21_truncate_5) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("199", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 2);
  s21_decs2decimal("1", &s_decimal_3);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_decs2decimal("2", &s_decimal_3);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}

START_TEST(s21_truncate_6) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("79228162514264337593543950335", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 1);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("7922816251426433759354395033", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 0);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_decs2decimal("7922816251426433759354395034", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 0);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}
END_TEST

START_TEST(s21_truncate_7) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("50000", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 2);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("500", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 0);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}
END_TEST

START_TEST(s21_truncate_8) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("79228162514264337593543950335", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 28);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("7", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_decs2decimal("8", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}
END_TEST

START_TEST(s21_truncate_9) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("1", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 28);
  s21_set_sign(&s_decimal_1, 0);
  s21_decs2decimal("0", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 0);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}
END_TEST

START_TEST(s21_truncate_10) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("12345", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 2);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("123", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_decs2decimal("124", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}
END_TEST

START_TEST(s21_truncate_11) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decimal s_decimal_3 = {0};
  s21_decs2decimal("100000000", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 7);
  s21_set_sign(&s_decimal_1, 1);
  s21_decs2decimal("10", &s_decimal_3);
  s21_set_scale(&s_decimal_3, 0);
  s21_set_sign(&s_decimal_3, 1);
  s21_truncate(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_floor(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
  s21_round(s_decimal_1, &s_decimal_2);
  ck_assert_int_eq(s21_is_equal(s_decimal_2, s_decimal_3), 1);
}

START_TEST(s21_truncate_12) {
  s21_decimal s_decimal_1 = {0};
  s21_decs2decimal("100000000", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 7);
  s21_set_sign(&s_decimal_1, 1);
  ck_assert_int_eq(s21_truncate(s_decimal_1, NULL), 1);
  ck_assert_int_eq(s21_floor(s_decimal_1, NULL), 1);
  ck_assert_int_eq(s21_round(s_decimal_1, NULL), 1);
}

START_TEST(s21_neg_1) {
  printf("Start neg tests\n");
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("79228162514264337593543950335", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 28);
  s21_set_sign(&s_decimal_1, 0);
  s21_negate(s_decimal_1, &s_decimal_2);
  int res = s21_get_sign(s_decimal_2);
  ck_assert_int_eq(res, 1);
  s21_negate(s_decimal_2, &s_decimal_1);
  res = s21_get_sign(s_decimal_1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_neg_2) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("0", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 0);
  s21_set_sign(&s_decimal_1, 0);
  s21_negate(s_decimal_1, &s_decimal_2);
  int res = s21_get_sign(s_decimal_2);
  ck_assert_int_eq(res, 1);
  s21_negate(s_decimal_2, &s_decimal_1);
  res = s21_get_sign(s_decimal_1);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(s21_neg_3) {
  s21_decimal s_decimal_1 = {0};
  s21_decimal s_decimal_2 = {0};
  s21_decs2decimal("1", &s_decimal_1);
  s21_set_scale(&s_decimal_1, 1);
  s21_set_sign(&s_decimal_1, 1);
  s21_negate(s_decimal_1, &s_decimal_2);
  int res = s21_get_sign(s_decimal_2);
  ck_assert_int_eq(res, 0);
  s21_negate(s_decimal_2, &s_decimal_1);
  res = s21_get_sign(s_decimal_1);
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(s21_neg_4) {
  s21_decimal s_decimal_1 = {0};
  ck_assert_int_eq(s21_negate(s_decimal_1, NULL), 1);
}
END_TEST

START_TEST(s21_other_1) {
  printf("Start other tests\n");
  s21_big_decimal sbd1 = {
      .bits = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX}};
  s21_big_decimal sbd2 = {
      .bits = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX}};
  s21_big_decimal sbd3 = {0};
  int res = s21_add_bin(sbd1, sbd2, &sbd3);
  res = s21_set_bscale(&sbd1, -1);
  ck_assert_int_eq(res, 1);
  res = s21_set_bscale(&sbd1, 30);
  ck_assert_int_eq(res, 1);
  res = 1;
  s21_set_bbit(&sbd1, 192, res);
  ck_assert_int_eq(res, s21_get_bbit(sbd1, 192));
  ck_assert_int_eq(-1, s21_get_bbit(sbd1, 250));
  ck_assert_int_eq(-1, s21_get_bbit(sbd1, -1));
  s21_set_bscale(&sbd1, 5);
  s21_set_bscale(&sbd2, 1);
  ck_assert_int_eq(s21_big_equalize_scale(&sbd1, &sbd2), 5);
}
END_TEST

START_TEST(s21_other_2) {
  s21_decimal sd1 = {.bits = {UINT_MAX, UINT_MAX, UINT_MAX}};
  s21_decimal sd2 = {.bits = {UINT_MAX, UINT_MAX, UINT_MAX}};
  s21_decimal sd3 = {0};
  int res = s21_set_scale(&sd1, -1);
  ck_assert_int_eq(res, 1);
  res = s21_set_scale(&sd1, 30);
  ck_assert_int_eq(res, 1);
  res = 1;
  s21_set_bit(&sd1, 100, res);
  ck_assert_int_eq(res, s21_get_bit(sd1, 100));
  ck_assert_int_eq(-1, s21_get_bit(sd1, -100));
  ck_assert_int_eq(-1, s21_get_bit(sd1, 200));
  res = s21_mul(sd1, sd2, &sd3);
  ck_assert_int_eq(res, 1);
  res = s21_add(sd1, sd2, &sd3);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(1, s21_add(sd1, sd2, NULL));
  ck_assert_int_eq(1, s21_sub(sd1, sd2, NULL));
  ck_assert_int_eq(1, s21_mul(sd1, sd2, NULL));
  ck_assert_int_eq(1, s21_div(sd1, sd2, NULL));
  ck_assert_int_eq(1, s21_mod(sd1, sd2, NULL));
  ck_assert_int_eq(1, s21_from_decimal_to_float(sd1, NULL));
  ck_assert_int_eq(1, s21_from_decimal_to_int(sd1, NULL));
  ck_assert_int_eq(1, s21_from_float_to_decimal(1.1, NULL));
}
END_TEST

START_TEST(s21_other_3) {
  s21_decimal sd1 = {.bits = {UINT_MAX, UINT_MAX, UINT_MAX}};
  int a, res;
  res = s21_from_decimal_to_int(sd1, &a);
  ck_assert_int_eq(res, 1);
  s21_reverse_string(NULL);
  char s[1];
  s[0] = 0;
  s21_reverse_string(s);
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *suite = suite_create("s21_decimal");
  TCase *tcase_core_fromto = tcase_create("fromto");
  tcase_add_test(tcase_core_fromto, s21_fromto_test_1);
  tcase_add_test(tcase_core_fromto, s21_fromto_test_2);
  tcase_add_test(tcase_core_fromto, s21_fromto_test_3);
  suite_add_tcase(suite, tcase_core_fromto);

  TCase *tcase_core_add = tcase_create("add");
  tcase_add_test(tcase_core_add, s21_add_test_1);
  tcase_add_test(tcase_core_add, s21_add_test_2);
  tcase_add_test(tcase_core_add, s21_add_test_3);
  tcase_add_test(tcase_core_add, s21_add_test_4);
  tcase_add_test(tcase_core_add, s21_add_test_5);
  suite_add_tcase(suite, tcase_core_add);

  TCase *tcase_core_sub = tcase_create("sub");
  tcase_add_test(tcase_core_sub, s21_sub_test_1);
  tcase_add_test(tcase_core_sub, s21_sub_test_2);
  tcase_add_test(tcase_core_sub, s21_sub_test_3);
  tcase_add_test(tcase_core_sub, s21_sub_test_4);
  tcase_add_test(tcase_core_sub, s21_sub_test_5);
  suite_add_tcase(suite, tcase_core_sub);

  TCase *tcase_core_mul = tcase_create("mul");
  tcase_add_test(tcase_core_mul, s21_mul_test_1);
  tcase_add_test(tcase_core_mul, s21_mul_test_2);
  tcase_add_test(tcase_core_mul, s21_mul_test_3);
  tcase_add_test(tcase_core_mul, s21_mul_test_4);
  tcase_add_test(tcase_core_mul, s21_mul_test_5);
  suite_add_tcase(suite, tcase_core_mul);

  TCase *tcase_core_div = tcase_create("div");
  tcase_add_test(tcase_core_div, s21_div_test_1);
  tcase_add_test(tcase_core_div, s21_div_test_2);
  tcase_add_test(tcase_core_div, s21_div_test_3);
  tcase_add_test(tcase_core_div, s21_div_test_4);
  tcase_add_test(tcase_core_div, s21_div_test_5);
  tcase_add_test(tcase_core_div, s21_div_test_6);
  suite_add_tcase(suite, tcase_core_div);

  TCase *tcase_core_mod = tcase_create("mod");
  tcase_add_test(tcase_core_mod, s21_mod_test_1);
  tcase_add_test(tcase_core_div, s21_mod_test_2);
  tcase_add_test(tcase_core_div, s21_mod_test_3);
  tcase_add_test(tcase_core_div, s21_mod_test_4);
  suite_add_tcase(suite, tcase_core_mod);

  TCase *tcase_core_gre = tcase_create("is_greater");
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_0);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_1);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_2);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_3);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_4);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_5);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_6);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_7);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_8);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_9);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_10);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_11);
  tcase_add_test(tcase_core_gre, s21_decimal_is_greater_12);
  suite_add_tcase(suite, tcase_core_gre);

  TCase *tcase_core_trunc = tcase_create("s21_truncate");
  tcase_add_test(tcase_core_trunc, s21_truncate_1);
  tcase_add_test(tcase_core_trunc, s21_truncate_2);
  tcase_add_test(tcase_core_trunc, s21_truncate_3);
  tcase_add_test(tcase_core_trunc, s21_truncate_4);
  tcase_add_test(tcase_core_trunc, s21_truncate_5);
  tcase_add_test(tcase_core_trunc, s21_truncate_6);
  tcase_add_test(tcase_core_trunc, s21_truncate_7);
  tcase_add_test(tcase_core_trunc, s21_truncate_8);
  tcase_add_test(tcase_core_trunc, s21_truncate_9);
  tcase_add_test(tcase_core_trunc, s21_truncate_10);
  tcase_add_test(tcase_core_trunc, s21_truncate_11);
  tcase_add_test(tcase_core_trunc, s21_truncate_12);
  suite_add_tcase(suite, tcase_core_trunc);

  TCase *tcase_core_neg = tcase_create("s21_neg");
  tcase_add_test(tcase_core_neg, s21_neg_1);
  tcase_add_test(tcase_core_neg, s21_neg_2);
  tcase_add_test(tcase_core_neg, s21_neg_3);
  tcase_add_test(tcase_core_neg, s21_neg_4);
  suite_add_tcase(suite, tcase_core_neg);

  TCase *tcase_core_other = tcase_create("s21_other");
  tcase_add_test(tcase_core_other, s21_other_1);
  tcase_add_test(tcase_core_other, s21_other_2);
  tcase_add_test(tcase_core_other, s21_other_3);
  // tcase_add_test(tcase_core_other, s21_neg_4);
  suite_add_tcase(suite, tcase_core_other);

  return suite;
}

int main(void) {
  int failed_count;
  Suite *suite = s21_decimal_suite();
  SRunner *suite_runner = srunner_create(suite);
  srunner_set_fork_status(suite_runner, CK_NOFORK);
  srunner_run_all(suite_runner, CK_NORMAL);
  failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);
  return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
