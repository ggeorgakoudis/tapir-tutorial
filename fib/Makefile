all: fib.ll fib.o

clean:
	rm -f fib.ll fib.o

fib.ll: fib.c
	../dockerscript.sh clang -O3 -fcilkplus /host/fib.c -fdetach -o /host/fib.ll -S -emit-llvm

fib.o: fib.c
	../dockerscript.sh clang -O3 -fcilkplus /host/fib.c -fdetach -o /host/fib.o

run: fib.o
	../dockerrunscript.sh /host/fib.o

kill:
	docker kill tapirdocker
