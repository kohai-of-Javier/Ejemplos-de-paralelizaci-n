#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
    if(argc < 3) {
        perror("ERROR\n");
        return 1;
    }

    int n = atoi(argv[1]), p = atoi(argv[2]);

    Args arg[p];
    pthread_t th[p];
    int step = n/p, base = 1;
    for(int i=0;i<p-1;i++) {
        arg[i].i = base;
        if(i < n%p) {
            arg[i].j = base+step;
            base += step+1;
        }
        else {
            arg[i].j = base+step-1;
            base += step;
        }
        pthread_create(&th[i], NULL, f, &arg[i]);
    }
    arg[p-1].i = base;
    arg[p-1].j = n;
    pthread_create(&th[p-1], NULL, f, &arg[p-1]);

    double ans = 1.0;
    for(int i=0;i<p;++i) {
        pthread_join(th[i], NULL);
        ans *= arg[i].res;
    }

    printf("%.0lf\n", ans);
}
