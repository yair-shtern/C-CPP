CC = gcc

LDFLAGS = -lm

CCFLAGS = -c -Wall -Wextra -Wvla -Werror -g -std=c99

.PHONY : all clean

all: libhashmap.a libhashmap_tests.a

clean:
	rm -f *.o *.a

libhashmap.a: hashmap.o vector.o pair.o
	ar rcs $@ $^

libhashmap_tests.a: test_suite.o hashmap.o hash_funcs.o test_pairs.o pair.o
	ar rcs $@ $^

hashmap.o: hashmap.c hashmap.h vector.h pair.h
	$(CC) $(CCFLAGS) $< -o $@

vector.o: vector.c vector.h
	$(CC) $(CCFLAGS) $< -o $@

pair.o: pair.c pair.h
	$(CC) $(CCFLAGS) $< -o $@

test_suite.o: test_suite.c test_suite.h test_pairs.h hashmap.h vector.h hash_funcs.h
	$(CC) $(CCFLAGS) $< -o $@

test_pairs.o: test_pairs.h pair.h
	$(CC) $(CCFLAGS) $< -o $@

hash_funcs.o: hash_funcs.h
	$(CC) $(CCFLAGS) $< -o $@