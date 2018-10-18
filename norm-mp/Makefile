all: norm.o norm.ll normmp.o normmp.ll

clean:
	rm -f norm.o norm.ll normmp.o normmp.ll

norm.ll: norm.c
	../dockerscript.sh clang -O3 -fopenmp /host/norm.c -o /host/norm.ll -S -emit-llvm

norm.o: norm.c
	../dockerscript.sh clang -O3 -fopenmp /host/norm.c -o /host/norm.o

normmp.ll: normmp.c
	../dockerscript.sh clang -O3 -fopenmp /host/normmp.c -o /host/normmp.ll -S -emit-llvm

normmp.o: normmp.c
	../dockerscript.sh clang -O3 -fopenmp /host/normmp.c -o /host/normmp.o

run: norm.o
	../dockerscript.sh /host/norm.o 2000

run-mp: normmp.o
	../dockerscript.sh /host/normmp.o 2000

run-all: run run-mp
