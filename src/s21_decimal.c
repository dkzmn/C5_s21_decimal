#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, sign1, sign2, isgreater, scale;
  if (result) {
    sign1 = s21_get_sign(value_1);
    sign2 = s21_get_sign(value_2);
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    s21_big_decimal bvalue_1, bvalue_2, bresult;
    s21_dec2bdec(value_1, &bvalue_1);
    s21_dec2bdec(value_2, &bvalue_2);
    s21_binit_zeros(&bresult);
    s21_init_zeros(result);
    isgreater = s21_is_greater_or_equal(value_1, value_2);
    scale = s21_big_equalize_scale(&bvalue_1, &bvalue_2);
    if (sign1 != sign2) {
      if (isgreater == 1)
        s21_sub_bin(bvalue_1, bvalue_2, &bresult);
      else
        s21_sub_bin(bvalue_2, bvalue_1, &bresult);
      if ((isgreater && sign1) || (!isgreater && !sign1))
        s21_set_bsign(&bresult, 1);
      else
        s21_set_bsign(&bresult, 0);
    } else {
      s21_add_bin(bvalue_1, bvalue_2, &bresult);
      s21_set_bsign(&bresult, sign1);
    }
    res = s21_bdec2dec(bresult, result, scale);
  } else {
    res = 1;
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, sign1, sign2, isgreater, scale;
  if (result) {
    sign1 = s21_get_sign(value_1);
    sign2 = s21_get_sign(value_2);
    s21_big_decimal bvalue_1, bvalue_2, bresult;
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    isgreater = s21_is_greater_or_equal(value_1, value_2);
    s21_dec2bdec(value_1, &bvalue_1);
    s21_dec2bdec(value_2, &bvalue_2);
    s21_big_equalize_scale(&bvalue_1, &bvalue_2);
    scale = s21_get_bscale(bvalue_1);
    s21_binit_zeros(&bresult);
    s21_init_zeros(result);
    if (sign1 != sign2) {
      s21_add_bin(bvalue_1, bvalue_2, &bresult);
      s21_set_bsign(&bresult, sign1);
    } else {
      if (isgreater == 1)
        s21_sub_bin(bvalue_1, bvalue_2, &bresult);
      else
        s21_sub_bin(bvalue_2, bvalue_1, &bresult);
      if ((isgreater && sign1) || (!isgreater && !sign1))
        s21_set_bsign(&bresult, 1);
      else
        s21_set_bsign(&bresult, 0);
    }
    res = s21_bdec2dec(bresult, result, scale);
  } else {
    res = 1;
  }
  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, sign1, sign2, newscale;
  if (result) {
    sign1 = s21_get_sign(value_1);
    sign2 = s21_get_sign(value_2);
    newscale = s21_get_scale(value_1) + s21_get_scale(value_2);
    s21_init_zeros(result);
    s21_big_decimal bvalue_1, bvalue_2, bresult;
    s21_set_sign(&value_1, 0);
    s21_set_sign(&value_2, 0);
    s21_dec2bdec(value_1, &bvalue_1);
    s21_dec2bdec(value_2, &bvalue_2);
    if (s21_mul_bin(bvalue_1, bvalue_2, &bresult) > 0) {
      res = 1;
    } else {
      s21_set_bsign(&bresult, sign1 ^ sign2);
      res = s21_bdec2dec(bresult, result, newscale);
    }
  } else {
    res = 1;
  }
  return res;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, tmp_res, sign1, sign2, newscale;
  if (result) {
    sign1 = s21_get_sign(value_1);
    sign2 = s21_get_sign(value_2);
    newscale = s21_get_scale(value_1) - s21_get_scale(value_2);
    s21_big_decimal bvalue_1, bvalue_2, bresult_div, bresult_mod,
        bten = BIG_DEC_TEN, btemp;
    s21_init_zeros(result);
    if (s21_is_dec_zero(value_2)) {
      res = 3;
    } else if (s21_is_dec_zero(value_1)) {
      res = 0;
    } else {
      s21_set_sign(&value_1, 0);
      s21_set_sign(&value_2, 0);
      s21_dec2bdec(value_1, &bvalue_1);
      s21_dec2bdec(value_2, &bvalue_2);
      while (s21_last_bbit(bvalue_1) - s21_last_bbit(bvalue_2) < 96) {
        tmp_res = s21_mul_bin(bvalue_1, bten, &btemp);
        if (tmp_res == 0) {
          s21_copy_big_decimal(btemp, &bvalue_1);
          newscale++;
        } else {
          break;
        }
      }
      s21_div_bin(bvalue_1, bvalue_2, &bresult_div, &bresult_mod);
      s21_set_bsign(&bresult_div, sign1 ^ sign2);
      while (newscale < 0) {
        tmp_res = s21_mul_bin(bresult_div, bten, &btemp);
        if (tmp_res == 0) {
          s21_copy_big_decimal(btemp, &bresult_div);
          newscale++;
        } else {
          res = 1;
          break;
        }
      }
      if (res == 0) res = s21_bdec2dec(bresult_div, result, newscale);
    }
  } else {
    res = 1;
  }
  return res;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = 0, scale, sign1;
  s21_big_decimal bvalue_1, bvalue_2, bresult_div, bresult_mod,
      bten = BIG_DEC_TEN, btemp;
  s21_decimal tmp_dec;
  if (result) {
    sign1 = s21_get_sign(value_1);
    s21_init_zeros(result);
    if (s21_is_dec_zero(value_2)) {
      res = 3;
    } else if (s21_is_dec_zero(value_1)) {
      res = 0;
    } else {
      s21_set_sign(&value_1, 0);
      s21_set_sign(&value_2, 0);
      s21_dec2bdec(value_1, &bvalue_1);
      s21_dec2bdec(value_2, &bvalue_2);
      if (s21_is_greater_or_equal(value_1, value_2)) {
        scale = s21_big_equalize_scale(&bvalue_1, &bvalue_2);
        s21_div_bin(bvalue_1, bvalue_2, &bresult_div, &bresult_mod);
        s21_set_bscale(&bresult_mod, s21_get_bscale(bvalue_1));
        if (s21_div(value_1, value_2, &tmp_dec) == 1) {
          while (scale > 0) {
            s21_div_bin(bresult_mod, bten, &bresult_div, &btemp);
            s21_copy_big_decimal(bresult_div, &bresult_mod);
            scale--;
          }
          s21_set_bscale(&bresult_mod, scale);
        }
      } else {
        s21_copy_big_decimal(bvalue_1, &bresult_mod);
      }
      s21_bdec2dec(bresult_mod, result, s21_get_bscale(bresult_mod));
      if (!s21_is_dec_zero(*result)) s21_set_sign(result, sign1);
    }
  } else {
    res = 1;
  }
  return res;
}

int s21_get_sign(s21_decimal value) { return (value.bits[3] >> 31 & 1); }

int s21_set_sign(s21_decimal *value, int sign) {
  int num = value->bits[3];
  value->bits[3] = (sign) ? (1u << 31 | num) : num & (~(1u << 31));
  return 0;
}

int s21_get_scale(s21_decimal value) {
  return (value.bits[3] & 0x00FF0000) >> 16;
}

int s21_set_scale(s21_decimal *value, int scale) {
  int res = 0;
  if (scale > 28 || scale < 0) {
    res = 1;
  } else {
    value->bits[3] &= 0xFF00FFFF;
    value->bits[3] |= (scale << 16);
  }
  return res;
}

int s21_get_bit(s21_decimal value, int n) {
  int res = -1, tmp, i;
  if (n < 128 && n >= 0) {
    if (n > 95) {
      tmp = value.bits[3];
      i = n - 96;
    } else if (n > 63) {
      tmp = value.bits[2];
      i = n - 64;
    } else if (n > 31) {
      tmp = value.bits[1];
      i = n - 32;
    } else {
      tmp = value.bits[0];
      i = n;
    }
    res = (tmp >> i) & 1;
  }
  return res;
}

void s21_set_bit(s21_decimal *s_decimal, int pos, int bit) {
  int num = s_decimal->bits[pos / 32];
  s_decimal->bits[pos / 32] =
      (bit) ? (1u << (pos % 32) | num) : num & (~(1u << (pos % 32)));
}

// заполняет нулями структуру decimal
void s21_init_zeros(s21_decimal *value) {
  value->bits[0] = 0;
  value->bits[1] = 0;
  value->bits[2] = 0;
  value->bits[3] = 0;
}

int s21_is_dec_zero(s21_decimal value) {
  int res = 0;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) res = 1;
  return res;
}

// перевоид строку нулей/единиц в манстиссу структуры decimal
void s21_bins2decimal(char *bin_s, s21_decimal *s_decimal) {
  s_decimal->bits[3] = 0;
  for (int i = 95, j = 0; i >= 0; i--, j++) {
    s21_set_bit(s_decimal, i, bin_s[j] - '0');
  }
}

// переводит строку из десятичного представления в структуру decimal
void s21_decs2decimal(char *dec_s, s21_decimal *s_decimal) {
  char *bin_s = s21_decs2bins(dec_s);
  s21_bins2decimal(bin_s, s_decimal);
  free(bin_s);
}

// переводит строку из десятичного представления в строку нулей/единиц
char *s21_decs2bins(char *x) {
  int num = 96;
  char *res = s21_stringzeros(num);
  int j = 0;
  int len = strlen(x);
  char *zeros = s21_stringzeros(len);
  char *temp = calloc(len + 1, sizeof(char));
  temp[len] = '\0';
  memcpy(temp, x, len);
  while (strcmp(temp, zeros) != 0) {
    int ostatok = 0;
    for (int i = 0; i <= len - 1; i++) {
      int digit = s21_twodigits(ostatok, temp[i]);
      int chastnoe = digit / 2;
      ostatok = digit % 2;
      temp[i] = chastnoe + '0';
    }
    res[j++] = ostatok + '0';
    if (j >= num) break;
  }
  s21_reverse_string(res);
  free(temp);
  free(zeros);
  return res;
}

void s21_copy_decimal(s21_decimal source, s21_decimal *dest) {
  dest->bits[0] = source.bits[0];
  dest->bits[1] = source.bits[1];
  dest->bits[2] = source.bits[2];
  dest->bits[3] = source.bits[3];
}

// вспомогательные функции
// развернуть строку
void s21_reverse_string(char *str) {
  if (str == 0) return;
  if (*str == 0) return;
  char *start = str;
  char *end = start + strlen(str) - 1;
  char temp;
  while (end > start) {
    temp = *start;
    *start = *end;
    *end = temp;
    ++start;
    --end;
  }
}

// создает и возвращает строку длиной i заполненную нулями
char *s21_stringzeros(int i) {
  char *res = calloc(i + 1, sizeof(char));
  memset(res, '0', i);
  res[i] = '\0';
  return res;
}

// возвращает число из остатка и символа
int s21_twodigits(int i, char c) { return i * 10 + (c - '0'); }
