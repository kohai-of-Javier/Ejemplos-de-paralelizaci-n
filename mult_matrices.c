#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef double** Matriz;

Matriz nuevaMatriz(int n, int m) {
    double **filas = malloc(n*sizeof(double*));
    for(int i=0; i<n; i++)
        filas[i] = malloc(m*sizeof(double));
    return filas;
}

void destruirMatriz(Matriz a, int n, int m) {
    for(int i=0;i<n;++i)
        free(a[i]);
    free(a);
}

void mult(Matriz a, Matriz b, Matriz c, int n, int ini, int fin) {
    for(int i=ini;i<fin;++i) {
        for(int j=0;j<n;++j) {
            c[i][j] = 0.0;
            for(int k=0;k<n;++k)
                c[i][j] += a[i][k]*b[k][j];
        }
    }
}

typedef struct {
    Matriz a, b, c;
    int n, ini, fin;
} Args;

void *mult_thread(void *ptr) {
    Args* arg = (Args*)ptr;
    mult(arg->a, arg->b, arg->c, arg->n, arg->ini, arg->fin);
    return NULL;
}

Matriz par_mult(Matriz a, Matriz b, int n, int p) {
    pthread_t t[p];
    int l = 0, k = n%p;
    Matriz c = nuevaMatriz(n,n);
    Args** arg = malloc(p*sizeof(Args*));
    for(int s=0;s<p;++s) {
        arg[s] = malloc(sizeof(arg));
        arg[s]->a = a;
        arg[s]->b = b;
        arg[s]->c = c;
        arg[s]->n = n;
        arg[s]->ini = l;
        arg[s]->fin = l+(n/p);
        if(s<k) ++arg[s]->fin;

        pthread_create(&t[s], NULL, mult_thread, arg[s]);

    }
    for(int i=0;i<p;++i) {
        pthread_join(t[i], NULL);
        free(arg[i]);
    }
    free(arg);
}

void leer(Matriz x, int tam) {
    for(int i=0;i<tam;++i) {
        for(int j=0;j<tam;++j)
            scanf("%lf", &x[i][j]);
    }
}

int main() {
    int n;
    scanf("%i", &n);

    Matriz a = nuevaMatriz(n,n), b = nuevaMatriz(n,n);
    leer(a, n);
    leer(b, n);
    par_mult(a, b, n, 4);

    destruirMatriz(a,n,n);
    destruirMatriz(b,n,n);
}
