#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Gerado por IA, só pra testes

static size_t sizes[] = {10,25,50,100,250,500,1000,13,37,77};

int main(void) {
    size_t pool_size = 1024;

    mymemory_t *m = mymemory_init(pool_size);
    if (!m) {
        fprintf(stderr, "Falha ao criar pool\n");
        return 1;
    }

    void *a = mymemory_alloc(m, 100);
    void *b = mymemory_alloc(m, 200);
    void *c = mymemory_alloc(m, 50);
    void *d = mymemory_alloc(m, 300);
    void *e = mymemory_alloc(m, 150);

    printf("Depois de 5 alocações:\n");
    mymemory_display(m);
    mymemory_stats(m);

    // /* libera meia-dúzia aleatoriamente */
    // for (int i = 0; i < alloc_count/2; ++i) {
    //     int idx = rand() % alloc_count;
    //     void *p = allocated[idx];
    //     if (p) {
    //         mymemory_free(m, p);
    //         allocated[idx] = NULL;
    //     }
    // }

    mymemory_free(m, b);
    mymemory_free(m, d);

    printf("\nApós liberar os blocos:\n");
    mymemory_display(m);
    mymemory_stats(m);

    mymemory_alloc(m, 77);
    mymemory_alloc(m, 45);
    mymemory_alloc(m, 12);
    
    mymemory_display(m);
    mymemory_stats(m);

    mymemory_cleanup(m);
    return 0;
}
