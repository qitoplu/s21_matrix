CC = gcc -Wall -Werror -Wextra -std=c11
CLANG = clang-format -style=Google

GCOVFLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lcheck -lm -lpthread
TARGET = s21_matrix.a

USERNAME=$(shell whoami)

OS := $(shell uname -s)
ifeq ($(OS), Darwin)
  CC += -D OS_MAC
  TEST_FLAGS = $(LIBS)
endif
ifeq ($(OS), Linux)
  CC += -D OS_LINUX
  TEST_FLAGS = $(LIBS) -lpthread -lm -pthread -lrt -lsubunit
endif

all: $(TARGET)

$(TARGET): clean
	${CC} s21_*.c -c 
	ar rc $(TARGET) s21_*.o

test: $(TARGET)
	${CC} tests.c  $(TARGET) ${TEST_FLAGS} -o test
	./test

gcov_report: clean $(TARGET)
	@${CC} *.c $(TARGET) ${TEST_FLAGS} $(GCOVFLAGS) -o test_report $(LIBS)
	@./test_report
	@lcov -t test_report -o test.info -c -d .
	@genhtml -o report test.info
	@open ./report/index-sort-f.html

rebuild: all

clean:
	@-rm -rf $(TARGET)
	@-rm -rf *.gcda
	@-rm -rf *.gcno
	@-rm -rf *.a
	@-rm -rf *.o
	@-rm -rf test
	@-rm -rf test_report
	@-rm -rf test.info
	@-rm -rf report

style:
	$(CLANG) -n *.c *.h
	$(CLANG) -i *.c *.h

leaks:
	valgrind --tool=memcheck --leak-check=yes  ./test
