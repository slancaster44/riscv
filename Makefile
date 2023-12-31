CC = gcc
CFLAGS = -O3 -o riscv -march=native -Wno-pointer-arith 
INCLUDE = -Iinclude
SOURCE = `find ./src -name *.c ! -name test.c ! -name demo.c ! -name benchmark.c`
LDFLAGS = -lm

test:
	clear
	$(CC) $(CFLAGS) -g -Wall -Wswitch-enum -Wno-unused-variable -Wno-unused-parameter $(INCLUDE) $(SOURCE) src/test.c $(LDFLAGS)
	./riscv
	rm -r ./riscv 

benchmark:
	clear
	$(CC) $(CFLAGS) $(INCLUDE) $(SOURCE) src/benchmark.c $(LDFLAGS)
	./riscv
	rm -r ./riscv

demo:
	clear
	$(CC) $(CFLAGS) -g $(INCLUDE) $(SOURCE) src/demo.c $(LDFLAGS)
	./riscv
	rm -r ./riscv