CC = gcc
CFLAGS = -g -Wall -pedantic
LDFLAGS = -lm -shared
LDTESTFLAGS = -L. -lbigint -Wl,-rpath='$$ORIGIN'

.PHONY: clean

libbigint.so: bigint.c bigint.h utils.h
	$(CC) $(CFLAGS) $(LDFLAGS) bigint.c -o libbigint.so

factorial_test: test.c libbigint.so
	$(CC) $(CFLAGS) $(LDTESTFLAGS) test.c -o factorial_test

clean:
	rm -f *.o
	rm -f libbigint.so
	rm -f factorial_test
