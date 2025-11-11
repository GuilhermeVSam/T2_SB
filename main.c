#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int help();
int menu();

int main(void) {
    //menu();
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
    //menu();
    return 0;
}

int help(){
    printf("=============SISTEMA DE ALOCAÇÃO DE MEMÓRIA=============\n");
    printf("malloc <tamanho> - Aloca um bloco com tamanho <tamanho>\n");
    printf("free <id> - Libera o bloco com <id> especificado\n");
    printf("display - Exibe todos os blocos do sistema\n");
    printf("stats - Exibe o status do sistema\n");
    printf("cleanup - Limpa a memória do sistema\n");
    return 0;
}

int menu(){
    mymemory_t *m = NULL;

    while (1) {
        char input[100];

        printf("> ");
        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\n")] = '\0';

        char command[20];
        int value;

        if (sscanf(input, "%19s %d", command, &value) == 2) {
            if(strcmp(command, "init") == 0){
                m = mymemory_init(value);
            } else if(strcmp(command, "alloc") == 0){
                mymemory_alloc(m, value);
            } else if(strcmp(command, "free") == 0){
                //mymemory_free(m, value);
            }
        } else if (sscanf(input, "%19s", command) == 1) {
            if (strcmp(command, "help") == 0) {
                help();
            } else if(strcmp(command, "display") == 0) {
                mymemory_display(m);
            } else if(strcmp(command, "stats") == 0) {
                mymemory_stats(m);
            } else if(strcmp(command, "cleanup") == 0){
                mymemory_cleanup(m);
            } else {
                printf("Unknown command: %s\n", command);
            }
        }
    }
}
