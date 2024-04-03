#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DECIMAL_MAX pow(2, 96)
#define DECIMAL_MIN 1e-28
#define FLOAT_ACCURACY 7
#define BIG_DEC_TEN                  \
  {                                  \
    .bits = { 10, 0, 0, 0, 0, 0, 0 } \
  }
#define BIG_DEC_ONE                 \
  {                                 \
    .bits = { 1, 0, 0, 0, 0, 0, 0 } \
  }

#define MAX_DECIMAL "79228162514264337593543950335"

#define DEBUG 0

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int s21_set_sign(s21_decimal *value, int sign);
int s21_get_sign(s21_decimal value);
int s21_set_bsign(s21_big_decimal *value, int sign);
int s21_get_bsign(s21_big_decimal value);
int s21_get_scale(s21_decimal value);
int s21_set_scale(s21_decimal *value, int scale);
int s21_get_bscale(s21_big_decimal value);
int s21_set_bscale(s21_big_decimal *value, int scale);
int s21_get_bit(s21_decimal value, int n);
void s21_set_bit(s21_decimal *value, int n, int bit);
int s21_get_bbit(s21_big_decimal s_bdecimal, int n);
void s21_set_bbit(s21_big_decimal *s_bgdecimal, int pos, int bit);

int s21_add_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
void s21_sub_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result);
int s21_mul_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);
void s21_div_bin(s21_big_decimal value_1, s21_big_decimal value_2,
                 s21_big_decimal *result_div, s21_big_decimal *result_mod);

void s21_get_bits(s21_big_decimal src, s21_big_decimal *tgt, int start,
                  int end);
void s21_get_one_bit(s21_big_decimal src, s21_big_decimal *tgt, int n);

int s21_last_bbit(s21_big_decimal value);

int s21_is_greater_or_equal_big(s21_big_decimal value_1,
                                s21_big_decimal value_2);

int s21_equalize_scale(s21_decimal s_decimal_1, s21_decimal s_decimal_2);
int s21_big_equalize_scale(s21_big_decimal *value_1, s21_big_decimal *value_2);
int s21_compare_big_decimal(s21_big_decimal s_bgdecimal1,
                            s21_big_decimal s_bgdecimal2);
void s21_normalize_scale(s21_big_decimal *s_bgdecimal, int n);
void s21_dec2bdec(s21_decimal s_decimal, s21_big_decimal *s_bdecimal);
int s21_bdec2dec(s21_big_decimal s_bdecimal, s21_decimal *s_decimal, int scale);

void s21_copy_decimal(s21_decimal source, s21_decimal *dest);
void s21_copy_big_decimal(s21_big_decimal source, s21_big_decimal *dest);

void s21_init_zeros(s21_decimal *s_decimal);
void s21_binit_zeros(s21_big_decimal *value);

int s21_is_dec_zero(s21_decimal value);
int s21_is_bdec_zero(s21_big_decimal value);

char *s21_bins2decs(char *binary_string);
void s21_bins2decimal(char *bin_s, s21_decimal *s_decimal);

char *s21_decimal2bins(s21_decimal s_decimal);
char *s21_decimal2decs(s21_decimal s_decimal);

char *s21_decs2bins(char *str);
void s21_decs2decimal(char *dec_s, s21_decimal *s_decimal);

char *s21_stringzeros(int i);
void s21_reverse_string(char *str);
int s21_twodigits(int i, char c);

#endif  // SRC_S21_DECIMAL_H_
