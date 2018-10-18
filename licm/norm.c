#include <cilk/cilk.h>

__attribute__((const,noinline)) double mag(const double *A, int n);

__attribute__((noinline))
void parallel_normalize(double *restrict out, const double *restrict in, int n) {
 cilk_for(int i = 0; i < n; ++i)
    out[i] = in[i] / mag(in, n);
}
