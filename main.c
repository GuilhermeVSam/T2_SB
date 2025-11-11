#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int help();
int menu();
int caso_chatgpt();

int printando_valores(){
    size_t pool_size = 1024;

    mymemory_t *m = mymemory_init(pool_size);
    char *str = mymemory_alloc(m, sizeof(char)*16);
    
    if(str == NULL){
        printf("ERRO AO ALOCAR STRING");
    }

    sprintf(str, "ola mundo");
    printf("%s\n", str);
}

int main() {
    //menu();
    size_t pool_size = 2048;

    mymemory_t *m = mymemory_init(pool_size);
    if (!m) {
        fprintf(stderr, "Falha ao criar pool\n");
        return 1;
    }

    void *a = mymemory_alloc(m, 100);
    void *b = mymemory_alloc(m, 200);
    void *c = mymemory_alloc(m, 50);
    void *d = mymemory_alloc(m, 25);

    printf("Depois de 5 alocações:\n");
    mymemory_display(m);
    mymemory_stats(m);

    mymemory_free(m, b);
    mymemory_free(m, c);

    printf("\nApós liberar os blocos B e C:\n");
    mymemory_display(m);
    mymemory_stats(m);

    mymemory_alloc(m, 55);
    
    printf("\nApós alocar um novo bloco\n");
    mymemory_display(m);
    mymemory_stats(m);

    mymemory_cleanup(m);
    //menu();

    caso_chatgpt();
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

int caso_chatgpt() {
    size_t pool_size = 8192; // maior, para caber 50 blocos variados

    mymemory_t *m = mymemory_init(pool_size);
    if (!m) {
        fprintf(stderr, "Falha ao criar pool\n");
        return 1;
    }

    void *blocks[50];
    size_t sizes[50] = {
        100, 200, 50, 25, 300,
        128, 64, 512, 256, 32,
        16, 90, 450, 380, 210,
        75, 87, 140, 155, 175,
        190, 205, 218, 234, 260,
        275, 290, 310, 320, 330,
        345, 360, 375, 390, 405,
        420, 435, 450, 465, 480,
        495, 510, 525, 540, 555,
        570, 585, 600, 615, 630
    };

    printf("=== Alocando 50 blocos ===\n");

    // aloca 50 blocos
    for (int i = 0; i < 50; i++) {
        blocks[i] = mymemory_alloc(m, sizes[i]);
        if (!blocks[i]) {
            printf("Falha ao alocar bloco %d (tamanho %zu)\n", i, sizes[i]);
        }
    }

    printf("\n--- Após 50 alocações ---\n");
    mymemory_display(m);
    mymemory_stats(m);

    // Libera alguns blocos (escolha arbitrária)
    printf("\n=== Liberando blocos aleatórios ===\n");
    int free_list[] = { 3, 7, 12, 18, 22, 27, 33, 45 };
    int free_count = sizeof(free_list) / sizeof(free_list[0]);

    for (int i = 0; i < free_count; i++) {
        int idx = free_list[i];
        if (blocks[idx]) {
            mymemory_free(m, blocks[idx]);
            blocks[idx] = NULL; // opcional, para evitar uso futuro
        }
    }

    printf("\n--- Após liberar blocos selecionados ---\n");
    mymemory_display(m);
    mymemory_stats(m);

    // tenta alocar blocos de tamanhos aleatórios para testar fragmentação
    printf("\n=== Testando novas alocações ===\n");

    void *extra1 = mymemory_alloc(m, 350);
    void *extra2 = mymemory_alloc(m, 90);
    void *extra3 = mymemory_alloc(m, 180);

    if (!extra1) printf("Falha ao alocar extra1\n");
    if (!extra2) printf("Falha ao alocar extra2\n");
    if (!extra3) printf("Falha ao alocar extra3\n");

    printf("\n--- Após novas alocações ---\n");
    mymemory_display(m);
    mymemory_stats(m);

    printf("\n=== Finalizando e limpando ===\n");
    mymemory_cleanup(m);

    return 0;
}
