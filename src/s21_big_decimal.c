#include "s21_decimal.h"

// Считывает знак числа
int s21_get_bsign(s21_big_decimal value) { return (value.bits[6] >> 31 & 1); }

// Устанавливает знак числа
int s21_set_bsign(s21_big_decimal *value, int sign) {
  int num = value->bits[6];
  value->bits[6] = (sign) ? (1u << 31 | num) : num & (~(1u << 31));
  return 0;
}

// Считывает положение десятичной точки
int s21_get_bscale(s21_big_decimal value) {
  return (value.bits[6] & 0x00FF0000) >> 16;
}

// Записывает положение десятичной точки
int s21_set_bscale(s21_big_decimal *value, int scale) {
  int res = 0;
  if (scale > 28 || scale < 0) {
    res = 1;
  } else {
    value->bits[6] &= 0xFF00FFFF;
    value->bits[6] |= (scale << 16);
  }
  return res;
}

// Получает произвольный бит числа по индексу
int s21_get_bbit(s21_big_decimal s_bdecimal, int n) {
  int res = -1, tmp, i;
  if (n < 224 && n >= 0) {
    if (n > 191) {
      tmp = s_bdecimal.bits[6];
      i = n - 192;
    } else if (n > 159) {
      tmp = s_bdecimal.bits[5];
      i = n - 160;
    } else if (n > 127) {
      tmp = s_bdecimal.bits[4];
      i = n - 128;
    } else if (n > 95) {
      tmp = s_bdecimal.bits[3];
      i = n - 96;
    } else if (n > 63) {
      tmp = s_bdecimal.bits[2];
      i = n - 64;
    } else if (n > 31) {
      tmp = s_bdecimal.bits[1];
      i = n - 32;
    } else {
      tmp = s_bdecimal.bits[0];
      i = n;
    }
    res = (tmp >> i) & 1;
  }
  return res;
}

// Записывает бит в число по индексу
void s21_set_bbit(s21_big_decimal *s_bgdecimal, int pos, int bit) {
  int num = s_bgdecimal->bits[pos / 32];
  s_bgdecimal->bits[pos / 32] =
      (bit) ? (1u << (pos % 32) | num) : num & (~(1u << (pos % 32)));
}

// Обнуляет все число
void s21_binit_zeros(s21_big_decimal *value) {
  value->bits[0] = 0;
  value->bits[1] = 0;
  value->bits[2] = 0;
  value->bits[3] = 0;
  value->bits[4] = 0;
  value->bits[5] = 0;
  value->bits[6] = 0;
}

// Проверяет число на 0
int s21_is_bdec_zero(s21_big_decimal value) {
  int res = 0;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
      value.bits[3] == 0 && value.bits[4] == 0 && value.bits[5] == 0)
    res = 1;
  return res;
}

// Копирует число
void s21_copy_big_decimal(s21_big_decimal source, s21_big_decimal *dest) {
  dest->bits[0] = source.bits[0];
  dest->bits[1] = source.bits[1];
  dest->bits[2] = source.bits[2];
  dest->bits[3] = source.bits[3];
  dest->bits[4] = source.bits[4];
  dest->bits[5] = source.bits[5];
  dest->bits[6] = source.bits[6];
}

// Сравнивает два big decimal побитово
int s21_compare_big_decimal(s21_big_decimal s_bgdecimal1,
                            s21_big_decimal s_bgdecimal2) {
  int ret = 0;
  int bgdecimal1_sign = s21_get_bsign(s_bgdecimal1);
  int bgdecimal2_sign = s21_get_bsign(s_bgdecimal2);
  if (bgdecimal1_sign == bgdecimal2_sign) {
    for (int i = 191; i >= 0; i--) {
      int value_bit1 = s21_get_bbit(s_bgdecimal1, i);
      int value_bit2 = s21_get_bbit(s_bgdecimal2, i);
      if (value_bit1 != value_bit2) {
        if (bgdecimal1_sign == 1) {
          ret = (value_bit1 > value_bit2) ? 2 : 1;
        } else {
          ret = (value_bit1 > value_bit2) ? 1 : 2;
        }
        break;
      }
    }
  } else {
    if (bgdecimal1_sign == 1 && bgdecimal2_sign == 0) ret = 2;
    if (bgdecimal1_sign == 0 && bgdecimal2_sign == 1) ret = 1;
  }
  return ret;
}

// Нормализация степени через умножение числа на 10 (n раз)
void s21_normalize_scale(s21_big_decimal *s_bgdecimal, int n) {
  int carry = 0, temp = 0;
  s21_big_decimal s_bgdecimal_result = {0};
  while (n > 0) {
    s21_big_decimal s_bgdecimal_temp1 = {0};
    s21_big_decimal s_bgdecimal_temp2 = {0};
    for (int i = 3; i < 192; i++) {
      s21_set_bbit(&s_bgdecimal_temp1, i, s21_get_bbit(*s_bgdecimal, i - 3));
    }
    for (int i = 1; i < 192; i++) {
      s21_set_bbit(&s_bgdecimal_temp2, i, s21_get_bbit(*s_bgdecimal, i - 1));
    }
    s_bgdecimal_result.bits[6] = s_bgdecimal->bits[6];
    for (int i = 0; i < 192; i++) {
      temp = s21_get_bbit(s_bgdecimal_temp1, i) +
             s21_get_bbit(s_bgdecimal_temp2, i) + carry;
      if (temp >= 2) {
        carry = 1;
        temp = temp % 2;
      } else {
        carry = 0;
      }
      s21_set_bbit(&s_bgdecimal_result, i, temp);
    }
    s21_copy_big_decimal(s_bgdecimal_result, s_bgdecimal);
    n--;
  }
}

// Переводит decimal в big decimal
void s21_dec2bdec(s21_decimal s_decimal, s21_big_decimal *s_bdecimal) {
  s_bdecimal->bits[0] = s_decimal.bits[0];
  s_bdecimal->bits[1] = s_decimal.bits[1];
  s_bdecimal->bits[2] = s_decimal.bits[2];
  s_bdecimal->bits[3] = 0;
  s_bdecimal->bits[4] = 0;
  s_bdecimal->bits[5] = 0;
  s_bdecimal->bits[6] = s_decimal.bits[3];
}

// Выдает номер самого старшего ненулевого бита
int s21_last_bbit(s21_big_decimal value) {
  int i;
  for (i = 191; i >= 0; i--)
    if (s21_get_bbit(value, i) == 1) break;
  if (i == 0 && s21_get_bbit(value, i) == 0) i = -1;
  return i;
}

/* Переводит big decimal в decimal. При этом производится обрезание мантиссы,
если она не влезает (с банковским округлением) а также отрабатываются
ошибки переполнения или слишком маленького числа. */
int s21_bdec2dec(s21_big_decimal s_bdecimal, s21_decimal *s_decimal,
                 int scale) {
  int mod, res = 0;
  int sign = s21_get_bsign(s_bdecimal);
  s21_big_decimal tmp1 = {0}, tmp2 = {0};
  s21_big_decimal bten = BIG_DEC_TEN, bone = BIG_DEC_ONE;
  s21_init_zeros(s_decimal);
  // Если число не влезает в decimal - уменьшаем его и округляем
  while ((s21_last_bbit(s_bdecimal) > 95 && scale > 0) || (scale >= 29)) {
    s21_div_bin(s_bdecimal, bten, &tmp1, &tmp2);
    s21_copy_big_decimal(tmp1, &s_bdecimal);
    mod = tmp2.bits[0];
    if ((mod > 5) || (mod == 5 && s21_get_bbit(s_bdecimal, 0) == 1)) {
      s21_add_bin(s_bdecimal, bone, &tmp1);
      s21_copy_big_decimal(tmp1, &s_bdecimal);
    } else if (s21_is_bdec_zero(s_bdecimal)) {
      res = 2;
    }
    scale--;
  }
  // Удаляем конечные нули
  while (scale > 0) {
    s21_div_bin(s_bdecimal, bten, &tmp1, &tmp2);
    mod = tmp2.bits[0];
    if (mod == 0) {
      s21_copy_big_decimal(tmp1, &s_bdecimal);
      scale--;
    } else {
      break;
    }
  }
  // Если все-равно не влезает - переполнение
  if (s21_last_bbit(s_bdecimal) > 95) {
    s21_binit_zeros(&s_bdecimal);
    res = 1;
  } else {
    s_decimal->bits[0] = s_bdecimal.bits[0];
    s_decimal->bits[1] = s_bdecimal.bits[1];
    s_decimal->bits[2] = s_bdecimal.bits[2];
    s21_set_scale(s_decimal, scale);
    s21_set_sign(s_decimal, sign);
  }
  return res;
}

int s21_big_equalize_scale(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int scale1 = s21_get_bscale(*value_1);
  int scale2 = s21_get_bscale(*value_2);
  int res = scale1;
  if (scale1 > scale2) {
    s21_normalize_scale(value_2, scale1 - scale2);
    s21_set_bscale(value_2, scale1);
  } else if (scale1 < scale2) {
    s21_normalize_scale(value_1, scale2 - scale1);
    s21_set_bscale(value_1, scale2);
    res = scale2;
  }
  return res;
}

int s21_is_greater_or_equal_big(s21_big_decimal value_1,
                                s21_big_decimal value_2) {
  int res = 1, j;
  for (j = 191; j >= 0; j--) {
    if (s21_get_bbit(value_1, j) < s21_get_bbit(value_2, j)) {
      res = 0;
      break;
    } else if (s21_get_bbit(value_1, j) > s21_get_bbit(value_2, j)) {
      break;
    }
  }
  return res;
}
