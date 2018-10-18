all: fast.o slow.o fast.ll slow.ll tapirpre.ll tapirpost.ll

clean:
	rm -f fast.o slow.o fast.ll slow.ll tapirpre.ll tapirpost.ll

tapirpre.ll: norm.c
	../dockerscript.sh clang -O0 /host/norm.c -o /host/tapirpre.ll -S -emit-llvm

tapirpost.ll: norm.c
	../dockerscript.sh clang -O3 /host/norm.c -o /host/tapirpost.ll -S -emit-llvm

tapir: tapirpre.ll tapirpost.ll

slow.ll: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -fdetach -o /host/slow.ll -S -emit-llvm

fast.ll: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -o /host/fast.ll -S -emit-llvm

slow.o: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -fdetach -o /host/slow.o

fast.o: norm.c
	../dockerscript.sh clang -O3 -fcilkplus /host/norm.c -o /host/fast.o

run-fast: fast.o
	../dockerscript.sh /host/fast.o 2000

run-slow: slow.o
	../dockerscript.sh /host/slow.o 2000
