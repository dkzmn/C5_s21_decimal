#include "s21_decimal.h"

// Функция для уравнивания показателя степени - через умножение на scale-раз
int s21_equalize_scale(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int ret;
  int scale_decimal_1 = s21_get_scale(s_decimal_1);
  int scale_decimal_2 = s21_get_scale(s_decimal_2);
  int diff_scale = (scale_decimal_1 > scale_decimal_2)
                       ? scale_decimal_1 - scale_decimal_2
                       : scale_decimal_2 - scale_decimal_1;
  s21_big_decimal s_bgdecimal_1 = {0};
  s21_big_decimal s_bgdecimal_2 = {0};
  s21_dec2bdec(s_decimal_1, &s_bgdecimal_1);
  s21_dec2bdec(s_decimal_2, &s_bgdecimal_2);
  if (scale_decimal_1 > scale_decimal_2)
    s21_normalize_scale(&s_bgdecimal_2, diff_scale);
  else if (scale_decimal_1 < scale_decimal_2)
    s21_normalize_scale(&s_bgdecimal_1, diff_scale);
  ret = s21_compare_big_decimal(s_bgdecimal_1, s_bgdecimal_2);
  return ret;
}

// 1 если первое больше второго, иначе 0
int s21_is_greater(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int res = 0;
  int sign_decimal_1 = s21_get_sign(s_decimal_1);
  int sign_decimal_2 = s21_get_sign(s_decimal_2);
  int scale_decimal_1 = s21_get_scale(s_decimal_1);
  int scale_decimal_2 = s21_get_scale(s_decimal_2);
  if (sign_decimal_1 == sign_decimal_2) {
    if (scale_decimal_1 == scale_decimal_2) {
      if (s_decimal_1.bits[2] != s_decimal_2.bits[2]) {
        if (sign_decimal_1 == 1) {
          res = (s_decimal_1.bits[2] > s_decimal_2.bits[2]) ? 0 : 1;
        } else {
          res = (s_decimal_1.bits[2] > s_decimal_2.bits[2]) ? 1 : 0;
        }
      } else {
        if (s_decimal_1.bits[1] != s_decimal_2.bits[1]) {
          if (sign_decimal_1 == 1) {
            res = (s_decimal_1.bits[1] > s_decimal_2.bits[1]) ? 0 : 1;
          } else {
            res = (s_decimal_1.bits[1] > s_decimal_2.bits[1]) ? 1 : 0;
          }
        } else {
          if (s_decimal_1.bits[0] != s_decimal_2.bits[0]) {
            if (sign_decimal_1 == 1) {
              res = (s_decimal_1.bits[0] > s_decimal_2.bits[0]) ? 0 : 1;
            } else {
              res = (s_decimal_1.bits[0] > s_decimal_2.bits[0]) ? 1 : 0;
            }
          }
        }
      }
    } else {
      // тут будет возвращаться код
      // 0 - оба числа после уравнивания scale оказались равны
      // 1 - первое число больше второго
      // 2 - второе число больше первого
      int equalize = s21_equalize_scale(s_decimal_1, s_decimal_2);
      if (equalize == 1) res = 1;
    }
  } else {
    if (sign_decimal_1 == 0 && sign_decimal_2 == 1) res = 1;
    if (sign_decimal_1 == 1 && sign_decimal_2 == 0) res = 0;
  }
  return res;
}

int s21_is_equal(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int equalize = s21_equalize_scale(s_decimal_1, s_decimal_2);
  if (equalize == 0) return 1;
  return 0;
}

int s21_is_greater_or_equal(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int equalize = s21_equalize_scale(s_decimal_1, s_decimal_2);
  if (equalize == 0 || equalize == 1) return 1;
  return 0;
}

int s21_is_less(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int equalize = s21_equalize_scale(s_decimal_1, s_decimal_2);
  if (equalize == 2) return 1;
  return 0;
}

int s21_is_less_or_equal(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int equalize = s21_equalize_scale(s_decimal_1, s_decimal_2);
  if (equalize == 2 || equalize == 0) return 1;
  return 0;
}

int s21_is_not_equal(s21_decimal s_decimal_1, s21_decimal s_decimal_2) {
  int equalize = s21_equalize_scale(s_decimal_1, s_decimal_2);
  if (equalize != 0) return 1;
  return 0;
}
