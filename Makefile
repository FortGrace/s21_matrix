CC=gcc -std=c11
CFLAGS=-c -Wall -Wextra -Werror
TEST_FLAGS = -lcheck -g

OS := $(shell uname -s)
ifeq ($(OS), Linux)
    D_ENV=__LINUX__
    TEST_FLAGS += -lrt -lpthread -lsubunit -lm
    OPEN_GCOV= xdg-open coverage/src/index.html
else
    D_ENV=__MAC_OS__
    OPEN_GCOV= open coverage/index.html
endif

TARGET = s21_matrix

C_FILES = s21_matrix.c
O_FILES = s21_matrix.o

all: clean s21_matrix.a test gcov_report

s21_matrix.a:
	@$(CC) $(CFLAGS) $(C_FILES) -D$(D_ENV)
	@ar rc lib$(TARGET).a $(O_FILES)
	@ranlib lib$@
	@cp lib$(TARGET).a $(TARGET).a
	@rm *.o

clean:
	@rm -rf *.out *.o *.gch  log.txt logTest.txt
	@rm -rf coverage *.a *.gcda *gcno *.info Test *.cfg
	@rm -rf report/
	@rm -rf temp/
	@rm -rf a.out.dSYM/
	@rm -rf Test.dSYM/
	@rm -rf s21_test.c

test:
	@checkmk clean_mode=1 s21_test.check > s21_test.c
	@clang-format -i --style=Google *.c *.h
	@$(CC) $(C_FILES) s21_test.c $(TEST_FLAGS) -D$(D_ENV) --coverage -o Test
	@./Test

gcov_report:
	@lcov -c -d . -o coverage.info
	@genhtml coverage.info -o coverage
#	@$(OPEN_GCOV)
