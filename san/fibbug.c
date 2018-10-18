#include <stdio.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

int z = 0;

int fib(int n) {
    if (n < 2) return 1;
    z = z + 1;
    int x, y;
    x = cilk_spawn fib(n-1);
    y = fib(n-2);
    cilk_sync;
    return x + y;
}

int main() {
    printf("number workers: %d\n", __cilkrts_get_nworkers());
    fflush(0);
    for(int i=0; i<20; i++) {
        printf("fib(%d)=%d\n", i, fib(i));
        fflush(0);
    }
}
