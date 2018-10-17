all: fast.o slow.o fast.ll slow.ll

clean:
	rm -f fast.o slow.o fast.ll slow.ll

slow.ll: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -fdetach -o /host/slow.ll -S -emit-llvm

fast.ll: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -o /host/fast.ll -S -emit-llvm

slow.o: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -fdetach -o /host/slow.o

fast.o: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -o /host/fast.o
