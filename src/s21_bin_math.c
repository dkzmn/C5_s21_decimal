#include "s21_decimal.h"

// Побитовое сложение двух big decimal, без учета знака и степени,
// при переполнении - возвращает 1
int s21_add_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int i, newbit = 0, transfer = 0, res = 0;
  s21_binit_zeros(result);
  for (i = 0; i < 192; i++) {
    newbit = s21_get_bbit(value_1, i) + s21_get_bbit(value_2, i) + transfer;
    transfer = 0;
    if (newbit > 1) {
      newbit -= 2;
      transfer = 1;
    }
    s21_set_bbit(result, i, newbit);
  }
  if (transfer > 0) {
    res = 1;
  }
  return res;
}

// Побитовое вычитание двух big decimal, без учета знака и степени
// Функция ничего не возвращает, так как предполагается? что value_1 >= value_2
void s21_sub_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result) {
  int i, newbit = 0, transfer = 0;
  s21_binit_zeros(result);
  for (i = 0; i < 192; i++) {
    newbit = s21_get_bbit(value_1, i) - s21_get_bbit(value_2, i) - transfer;
    transfer = 0;
    if (newbit < 0) {
      newbit += 2;
      transfer = 1;
    }
    s21_set_bbit(result, i, newbit);
  }
}

// Побитовое умножение двух big decimal, без учета знака и степени,
// при переполнении - возвращает 1
int s21_mul_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int i, j, newbit = 0, res = 0;
  int len1 = s21_last_bbit(value_1);
  int len2 = s21_last_bbit(value_2);
  int tmp[200] = {0};
  s21_binit_zeros(result);
  for (j = 0; j <= len2 && res == 0; j++) {
    for (i = j; i <= len1 + j && res == 0; i++) {
      newbit = tmp[i];
      newbit += s21_get_bbit(value_2, j) * s21_get_bbit(value_1, i - j);
      while (newbit > 1) {
        tmp[i + 1] += newbit / 2;
        newbit %= 2;
      }
      tmp[i] = newbit;
      newbit = 0;
    }
  }
  if (tmp[192] > 0) {
    res = 1;
  } else {
    for (i = 0; i < 192; i++) s21_set_bbit(result, i, tmp[i]);
  }
  return res;
}

// Побитовое деление двух big decimalс остатком, без учета знака и степени
// Функция ничего не возвращает, так как предполагается, что value_2 != 0
void s21_div_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result_div, s21_big_decimal *result_mod) {
  int i, idx_res = 0;
  int len1 = s21_last_bbit(value_1);
  int len2 = s21_last_bbit(value_2);
  s21_big_decimal tmp = {0}, tmp2 = {0};
  s21_binit_zeros(result_div);
  s21_binit_zeros(result_mod);
  i = len1 - len2;
  int tmp_res[192] = {0};
  s21_get_bits(value_1, &tmp, len1, i);
  if (i >= 0) {
    while (i >= 0) {
      i--;
      if (s21_is_greater_or_equal_big(tmp, value_2)) {
        s21_sub_bin(tmp, value_2, &tmp2);
        s21_copy_big_decimal(tmp2, &tmp);
        tmp_res[idx_res] = 1;
      } else {
        tmp_res[idx_res] = 0;
      }
      if (i >= 0) s21_get_one_bit(value_1, &tmp, i);
      idx_res++;
    }
    s21_copy_big_decimal(tmp, result_mod);
    for (i = idx_res - 1; i >= 0; i--)
      s21_set_bbit(result_div, idx_res - i - 1, tmp_res[i]);
  } else {
    s21_copy_big_decimal(value_1, result_mod);
  }
}

// Записывает в tgt указанные биты из числа src (с позиции start до end)
void s21_get_bits(s21_big_decimal src, s21_big_decimal *tgt, int start,
                  int end) {
  s21_binit_zeros(tgt);
  if (start >= end && start >= 0 && end >= 0 && start < 192 && end < 192) {
    for (int i = end; i <= start; i++)
      s21_set_bbit(tgt, i - end, s21_get_bbit(src, i));
  }
}

// Сдвигает все биты чиста на 1 и записывай в младший бит n
void s21_get_one_bit(s21_big_decimal src, s21_big_decimal *tgt, int n) {
  for (int i = 191; i > 0; i--) s21_set_bbit(tgt, i, s21_get_bbit(*tgt, i - 1));
  s21_set_bbit(tgt, 0, s21_get_bbit(src, n));
}
