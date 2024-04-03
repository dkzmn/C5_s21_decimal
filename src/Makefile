FLAGS=-Wall -Wextra -Werror -std=c11# -fsanitize=address -pedantic
LIBS=-lcheck
OS:=$(shell uname -s)
ifeq ($(OS), Darwin)
	FLAGS += -D MACOS
else
	LIBS += -lsubunit -lrt -lm -lpthread
endif

rebuild: clean all

all: test gcov_report

s21_decimal.a: s21_decimal.c s21_big_decimal.c s21_round_functions.c s21_bin_math.c s21_fromto_functions.c s21_compare_functions.c
	gcc $(FLAGS) -c s21_decimal.c -o s21_decimal.o
	gcc $(FLAGS) -c s21_big_decimal.c -o s21_big_decimal.o
	gcc $(FLAGS) -c s21_round_functions.c -o s21_round_functions.o
	gcc $(FLAGS) -c s21_bin_math.c -o s21_bin_math.o
	gcc $(FLAGS) -c s21_fromto_functions.c -o s21_fromto_functions.o
	gcc $(FLAGS) -c s21_compare_functions.c -o s21_compare_functions.o
	ar rc s21_decimal.a s21_decimal.o s21_big_decimal.o s21_round_functions.o s21_bin_math.o s21_fromto_functions.o s21_compare_functions.o
	ranlib s21_decimal.a

test: test_s21_decimal.c s21_decimal.a
	gcc $(FLAGS) -c test_s21_decimal.c -o test_s21_decimal.o
	gcc $(FLAGS) test_s21_decimal.o s21_decimal.a $(LIBS) -o test
	./test

gcov_report:
	gcc $(FLAGS) -c s21_decimal.c --coverage -o s21_decimal.o
	gcc $(FLAGS) -c s21_big_decimal.c --coverage -o s21_big_decimal.o
	gcc $(FLAGS) -c s21_round_functions.c --coverage -o s21_round_functions.o
	gcc $(FLAGS) -c s21_bin_math.c --coverage -o s21_bin_math.o
	gcc $(FLAGS) -c s21_fromto_functions.c --coverage -o s21_fromto_functions.o
	gcc $(FLAGS) -c s21_compare_functions.c --coverage -o s21_compare_functions.o
	gcc $(FLAGS) -c test_s21_decimal.c -o test_s21_decimal.o
	gcc $(FLAGS) test_s21_decimal.o s21_decimal.o s21_big_decimal.o s21_round_functions.o s21_bin_math.o s21_fromto_functions.o s21_compare_functions.o --coverage $(LIBS) -o test
	./test
	gcovr -r . --html --html-details -o s21_decimal.html
	rm -rf *.gcda *.gcno

clean:
	@rm -rf *.o *.a *.css *.html *.gcda *.gcno test localtest CPPLINT.cfg cpplint.py

clf:
	clang-format -n --style=Google *.[ch]

cpk:
	cppcheck *.[ch]

cpl:
	cp ../materials/linters/CPPLINT.cfg ../materials/linters/cpplint.py ./
	python3 cpplint.py --extensions=c *.[ch]
	rm -rf CPPLINT.cfg cpplint.py

style: clf cpk cpl