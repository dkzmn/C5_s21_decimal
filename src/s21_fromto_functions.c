#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = 0;
  if (dst) {
    s21_init_zeros(dst);
    if (src < 0) {
      dst->bits[3] = (1u << 31 | dst->bits[3]);
      src = -src;
    }
    dst->bits[0] = src;
  } else {
    res = 1;
  }
  return res;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 0, scale = 0;
  if (dst) {
    s21_init_zeros(dst);
    long double tmp = src;
    if (tmp < 0) {
      tmp = -tmp;
      s21_set_sign(dst, 1);
    }
    if (fabsl(tmp) > DECIMAL_MAX) {
      res = 1;
    } else if (fabsl(tmp) < DECIMAL_MIN) {
      s21_from_int_to_decimal(0, dst);
      res = 1;
    } else {
      if (tmp > powl(10, FLOAT_ACCURACY - 1)) {
        while (tmp > powl(10, FLOAT_ACCURACY)) {
          tmp /= 10;
          scale--;
        }
      } else {
        long double c;
        while (tmp < powl(10, FLOAT_ACCURACY - 1) && modfl(tmp, &c) > 0 &&
               scale < 28) {
          tmp *= 10;
          scale++;
        }
      }
      tmp = roundl(tmp);
      if (scale < 0) {
        tmp = tmp * powl(10, -scale);
        scale = 0;
      }
      for (int i = 0; i < 96; i++) {
        int bit = fmodl(tmp, 2.);
        s21_set_bit(dst, i, bit);
        tmp = floorl(tmp / 2);
      }
      s21_set_scale(dst, scale);
    }
  } else {
    res = 1;
  }
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int scale = s21_get_scale(src), sign = s21_get_sign(src), res = 0;
  long long int tmp = 0, base = 1;
  if (dst) {
    for (int i = 0; i < 96 && res == 0; i++) {
      tmp = tmp + s21_get_bit(src, i) * base;
      if (tmp > INT_MAX) res = 1;
      base = base * 2;
    }
    tmp = tmp / powl(10, scale);
    if (res == 0) {
      if (sign == 1) tmp = -tmp;
      *dst = tmp;
    } else {
      *dst = 0;
    }
  } else {
    res = 1;
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale, sign, res = 0;
  scale = s21_get_scale(src);
  sign = s21_get_sign(src);
  long double tmp = 0, base = 1;
  if (dst) {
    for (int i = 0; i < 96; i++) {
      tmp = tmp + s21_get_bit(src, i) * base;
      base = base * 2;
    }
    tmp = tmp / powl(10, scale);
    if (sign == 1) tmp = -tmp;
    *dst = tmp;
  } else {
    res = 1;
  }
  return res;
}
