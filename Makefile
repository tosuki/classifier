CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS :=
LDLIBS := -lm -lgsl -lgslcblas

TARGET := classifier
SRC := main.c network.c training.c error.c
OBJ := $(SRC:.c=.o)
TEST_TARGET := param_test
TEST_SRC := param_test.c param.c error.c
TEST_LDLIBS := -lm

.PHONY: all clean fclean re run test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(TEST_LDLIBS)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET) $(TEST_TARGET)

re: fclean all
