CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -O2
LDFLAGS :=
LDLIBS := -lm -lgsl -lgslcblas

TARGET := classifier
SRC := main.c network.c
OBJ := $(SRC:.c=.o)

.PHONY: all clean fclean re run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all
