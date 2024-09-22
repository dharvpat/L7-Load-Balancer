# Makefile for C Backend Load Balancer

CC = gcc
CFLAGS = -Wall -Wextra -I./include -g
LDFLAGS = -lmicrohttpd -ljson-c -lpthread

SRC = src/main.c src/load_balancer.c src/server.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = build/load_balancer

TEST_SRC = tests/test_load_balancer.c tests/test_server.c tests/test_utils.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXEC = build/run_tests

.PHONY: all clean test

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/*
	rm -f src/*.o tests/*.o

test: $(TEST_OBJ) $(EXEC)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(TEST_OBJ) $(LDFLAGS)
	./$(TEST_EXEC)

# Add more rules as needed
