CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS :=
LDLIBS := -lm -lgsl -lgslcblas

TARGET := classifier
SRC := main.c network.c training.c error.c
OBJ := $(SRC:.c=.o)
TEST_BINS := param_test error_test network_test training_test

.PHONY: all clean fclean re run test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

param_test: param_test.c param.c error.c
	$(CC) $(CFLAGS) -o $@ $^ -lm

error_test: error_test.c error.c
	$(CC) $(CFLAGS) -o $@ $^

network_test: network_test.c network.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

training_test: training_test.c training.c network.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

test: $(TEST_BINS)
	./param_test
	./error_test
	./network_test
	./training_test

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET) $(TEST_BINS)

re: fclean all
