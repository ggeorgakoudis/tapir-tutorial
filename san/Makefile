all: fib.ll fib.o fibbug.ll fibbug.o

clean:
	rm -f fib.ll fib.o fibbug.ll fibbug.o

fib.ll: fib.c
	../dockerscript.sh clang -g -ggdb -O3 -fcilkplus /host/fib.c -fsanitize=cilk -o /host/fib.ll -S -emit-llvm

fib.o: fib.c
	../dockerscript.sh clang -g -ggdb -O3 -fcilkplus /host/fib.c -fsanitize=cilk -o /host/fib.o

run: fib.o
	../dockerrunscript.sh /host/fib.o

fibbug.ll: fibbug.c
	../dockerscript.sh clang -g -ggdb -O3 -fcilkplus /host/fibbug.c -fsanitize=cilk -o /host/fibbug.ll -S -emit-llvm

fibbug.o: fibbug.c
	../dockerscript.sh clang -g -ggdb -O3 -fcilkplus /host/fibbug.c -fsanitize=cilk -o /host/fibbug.o

runbug: fibbug.o
	../dockerrunscript.sh /host/fibbug.o

kill:
	docker kill tapirdocker
