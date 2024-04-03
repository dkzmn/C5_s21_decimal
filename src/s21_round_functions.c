#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_big_decimal bvalue, bresult_mod, bresult_div, bten = BIG_DEC_TEN,
                                                    bone = BIG_DEC_ONE;
  int res, fraction = 0, scale, sign;
  if (result) {
    scale = s21_get_scale(value);
    sign = s21_get_sign(value);
    s21_dec2bdec(value, &bvalue);
    s21_init_zeros(result);
    while (scale > 0) {
      s21_div_bin(bvalue, bten, &bresult_div, &bresult_mod);
      if (!s21_is_bdec_zero(bresult_mod)) fraction++;
      s21_copy_big_decimal(bresult_div, &bvalue);
      scale--;
    }
    if (sign == 1 && fraction > 0) {
      s21_add_bin(bvalue, bone, &bresult_div);
      s21_copy_big_decimal(bresult_div, &bvalue);
    }
    res = s21_bdec2dec(bvalue, result, scale);
    s21_set_sign(result, sign);
  } else {
    res = 1;
  }
  return res;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_big_decimal bvalue, bresult_mod, bresult_div, bten = BIG_DEC_TEN,
                                                    bone = BIG_DEC_ONE;
  int res, mod, scale, sign;
  if (result) {
    scale = s21_get_scale(value);
    sign = s21_get_sign(value);
    s21_dec2bdec(value, &bvalue);
    s21_init_zeros(result);
    while (scale > 0) {
      s21_div_bin(bvalue, bten, &bresult_div, &bresult_mod);
      mod = bresult_mod.bits[0];
      s21_copy_big_decimal(bresult_div, &bvalue);
      if (mod >= 5) {
        s21_add_bin(bvalue, bone, &bresult_div);
        s21_copy_big_decimal(bresult_div, &bvalue);
      }
      scale--;
    }
    res = s21_bdec2dec(bvalue, result, scale);
    s21_set_sign(result, sign);
  } else {
    res = 1;
  }
  return res;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  s21_big_decimal bvalue, bresult_mod, bresult_div, bten = BIG_DEC_TEN;
  int res = 0, scale, sign;
  if (result) {
    sign = s21_get_sign(value);
    scale = s21_get_scale(value);
    s21_dec2bdec(value, &bvalue);
    s21_init_zeros(result);
    while (scale > 0) {
      s21_div_bin(bvalue, bten, &bresult_div, &bresult_mod);
      s21_copy_big_decimal(bresult_div, &bvalue);
      scale--;
    }
    res = s21_bdec2dec(bvalue, result, scale);
    s21_set_sign(result, sign);
  } else {
    res = 1;
  }
  return res;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = 0;
  if (result) {
    int sign = s21_get_sign(value);
    s21_copy_decimal(value, result);
    if (sign)
      s21_set_sign(result, 0);
    else
      s21_set_sign(result, 1);
  } else {
    res = 1;
  }
  return res;
}
