CC=gcc
CFLAGS=-Wall -Werror -Wextra
GCOV_FLAGS=-fprofile-arcs -ftest-coverage -lgcov
CHECK_FLAGS=-lcheck -lm -lpthread
ALL_FLAGS=$(CFLAGS) $(GCOV_FLAGS) $(CHECK_FLAGS)
SRC = s21_cat.c
OBJS = $(SRC:.c=.o)
NAME = s21_cat


all: s21_cat gcov_report

s21_cat: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(NAME)

$(OBJS) : $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC) s21_cat_test.c
	$(CC) $(ALL_FLAGS) $(SRC) -o $(NAME)
	$(CC) $(CFLAGS) $(CHECK_FLAGS) s21_cat_test.c -o test
	./test



clean:
	rm -rf $(OBJS) *.o *.gcno *.gcda *.info $(NAME) check_res test a b finish

gcov_report: test
#   checkmk test.check > s21_string_test.c
	geninfo *.gcda *.gcno -b . -o check_res 
	genhtml check_res -o finish
