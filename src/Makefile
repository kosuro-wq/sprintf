CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

OS := $(shell uname)
ifeq ($(OS), Linux)
    CHECK_FLAGS = -lcheck -pthread -lrt -lm -lsubunit
else
      Пути для macOS (Apple Silicon и Intel)
    CFLAGS += -I/opt/homebrew/include
    LDFLAGS += -L/opt/homebrew/lib
    CHECK_FLAGS = -lcheck -lm
endif

SRC = s21_sprintf.c \
      s21_sprintf_parser.c \
      s21_sprintf_string.c \
      s21_sprintf_integer.c \
      s21_sprintf_float.c

OBJ = $(SRC:.c=.o)

TEST_SRC = s21_sprintf_test.c
TEST_OBJ = $(TEST_SRC:.c=.o)

TARGET = test_s21_sprintf

all: $(TARGET)

$(TARGET): $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(TEST_OBJ) -o $(TARGET) $(LDFLAGS) $(CHECK_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET)

re: clean all

.PHONY: all test clean re