all: licm.ll

clean:
	rm -f *.ll

tapirpre.ll: norm.c
	../dockerscript.sh clang -O0 /host/norm.c -o /host/tapirpre.ll -S -emit-llvm
	../dockerscript.sh sed -i 's/optnone//g' /host/tapirpre.ll

mem2reg.ll: tapirpre.ll
	../dockerscript.sh opt -mem2reg -loop-rotate -simplifycfg /host/tapirpre.ll -S -o /host/mem2reg.ll

licm.ll: mem2reg.ll
	../dockerscript.sh opt -licm /host/mem2reg.ll -S -o /host/licm.ll

tapirpost.ll: norm.c
	../dockerscript.sh clang -O3 /host/norm.c -o /host/tapirpost.ll -S -emit-llvm
