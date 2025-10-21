#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n;

double mult(int i, int j) {
    double ans = 1.0;
    for(int k=i;k<=j;k++)
        ans *= k;
    return ans;
}

double factorial(int n) {
    return mult(1,n);
}

typedef struct {
    double res;
    int i,j;
} Args;

void *f(void *arg) {
    Args* param = (Args*)arg;
    param->res = mult(param->i, param->j);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    n = atoi(argv[1]);
    pthread_t t1, t2;
    Args arg1, arg2;
    arg1.i = 1, arg1.j = n/2;
    arg2.i = n/2+1, arg2.j = n;

    pthread_create(&t1, NULL, f, &arg1);
    pthread_create(&t2, NULL, f, &arg2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%.0lf\n", arg1.res * arg2.res);
}
