docker run --rm -v `pwd`:/host --name tapirdocker -e LD_PRELOAD="/usr/lib/llvm-5.0/lib/clang/5.0.0/lib/linux/libclang_rt.cilksan-x86_64.so" wsmoses/tapir-built "$@"
