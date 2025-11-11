#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Gerado por IA, só pra testes

static size_t sizes[] = {10,25,50,100,250,500,1000,13,37,77};

int main(void) {
    /* exemplo: pool de 16 KB */
    size_t pool_size = 16 * 1024;
    /* troque estrategia para FIRST_FIT, BEST_FIT, WORST_FIT */
    mymemory_t *m = mymemory_init(pool_size, FIRST_FIT);
    if (!m) {
        fprintf(stderr, "Falha ao criar pool\n");
        return 1;
    }

    /* aloca várias vezes com padrões variados para testar fragmentação */
    void *allocated[200];
    int alloc_count = 0;

    srand((unsigned) time(NULL));
    for (int i = 0; i < 50; ++i) {
        size_t s = sizes[rand() % (sizeof(sizes)/sizeof(sizes[0]))];
        void *p = mymemory_alloc(m, s);
        if (p) {
            allocated[alloc_count++] = p;
        }
    }

    printf("Depois de 50 alocações:\n");
    mymemory_display(m);
    mymemory_stats(m);

    /* libera meia-dúzia aleatoriamente */
    for (int i = 0; i < alloc_count/2; ++i) {
        int idx = rand() % alloc_count;
        void *p = allocated[idx];
        if (p) {
            mymemory_free(m, p);
            allocated[idx] = NULL;
        }
    }

    printf("\nApós liberar ~metade:\n");
    mymemory_display(m);
    mymemory_stats(m);

    /* tenta alocar blocos maiores para testar coalescência via gaps */
    void *big = mymemory_alloc(m, 2000);
    printf("\nTentativa de alocar bloco grande (2000 bytes) -> %p\n", big);
    mymemory_display(m);
    mymemory_stats(m);

    mymemory_cleanup(m);
    return 0;
}
