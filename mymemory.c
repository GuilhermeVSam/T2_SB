#include "mymemory.h"
#include <stdio.h>
#include <stdlib.h>

mymemory_t* mymemory_init(size_t size){
	void* pool = malloc(size);
	mymemory_t* memory = malloc(sizeof(mymemory_t));

	if(pool == NULL || memory == NULL){
		return NULL;
	}

	memory->pool = pool;
	memory->total_size = size;
	memory->head = NULL;

	return memory;
}

void* mymemory_alloc(mymemory_t *memory, size_t size) {
	// Cria espaço de alocação para o novo bloco
    allocation_t* new_block = malloc(sizeof(allocation_t));
    if (new_block == NULL) return NULL;		// Caso falhe para criar o novo bloco, retorna NULL

    allocation_t* current = memory->head;	// Para fazer a leitura dos blocos já alocados, aponta para o HEAD
    allocation_t* prev = NULL;				// Ponteiro para o bloco de memória anterior
    allocation_t* bestFit = NULL;
    while (current != NULL) {
        if (current->free && current->size >= size) {	// Se o bloco atual estiver vazio e tiver capacidade para ser utilizado, retorna
            if(bestFit == NULL){
                bestFit = current;
            } else {
                if(bestFit->size > current->size){
                    bestFit = current;
                }
            }
        }
        prev = current;
        current = current->next;
    }

    if(bestFit != NULL){
        bestFit->free = 0;
        return bestFit;
    }

    size_t offset = 0;
    if (prev != NULL) {
        allocation_t* last = memory->head;
        while (last->next != NULL) last = last->next;
        offset = (char*)last->start - (char*)memory->pool + last->size;		// Define um offset para alocar o novo bloco
    }

    if (offset + size > memory->total_size) {		// Verifica se a memória total tem capacidade para alocar o novo bloco
        free(new_block);
        return NULL;
    }

    new_block->start = (char*)memory->pool + offset;	// Guarda o endereço onde está localizado o novo bloco
    new_block->size = size;
    new_block->free = 0;
    new_block->next = NULL;

    if (memory->head == NULL) {			// Caso não exista blocos no sistema, define o bloco criado como HEAD
        memory->head = new_block;
    } else {
        allocation_t* last = memory->head;
        while (last->next != NULL) last = last->next;
        last->next = new_block;
    }

    return new_block->start;			// Retorna o endereço do bloco alocado
}

void mymemory_free(mymemory_t *memory, void *ptr) {
    if (memory == NULL || ptr == NULL) {
        return;
    }

    allocation_t *current = memory->head;
    allocation_t *prev = NULL;

    while (current != NULL) {
        if (current->start == ptr) {
            current->free = 1;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void mymemory_display(mymemory_t *memory){
	allocation_t* current = memory->head;
	int id = 0;
	while(current != NULL){
		printf("============================================\n");
		printf("ID Bloco: %d\n", id++);
		printf("Início do Bloco: %p\n", (char*)current->start);
		printf("Tamanho do Bloco: %d\n", current->size);
		printf("Bloco Vazio: %s\n", current->free ? "True" : "False");
		printf("Próximo Bloco: %p\n", (char*)current->next);
		current = current->next;
	}
	printf("============================================\n");
	printf("Dados da Memória\n");
	printf("Pool Base: %p\n", memory->pool);
	printf("Tamanho Total do Pool: %zu bytes\n", memory->total_size);
	printf("Blocos Alocados: %d\n\n", id);
	return;
}

void mymemory_stats(mymemory_t *memory) {
    if (memory == NULL)
        return;

    size_t total_alocados = 0;
    size_t total_bytes_alocados = 0;
    size_t total_bytes_livres = 0;
    size_t maior_livre = 0;
    size_t fragmentos_livres = 0;

    allocation_t *atual = memory->head;

    // Primeira passagem: contar blocos alocados e bytes livres nos blocos marcados como free
    while (atual != NULL) {
        if (!atual->free) {  // Bloco está alocado
            total_alocados++;
            total_bytes_alocados += atual->size;
        } else {  // Bloco está livre
            total_bytes_livres += atual->size;
        }
        atual = atual->next;
    }

    // Segunda parte: calcular fragmentação considerando blocos livres
    if (memory->head == NULL) {
        // Pool vazio - toda memória está livre
        maior_livre = memory->total_size;
        fragmentos_livres = 1;
        total_bytes_livres = memory->total_size;
    } else {
        // Para calcular fragmentos contíguos, precisamos verificar a memória entre blocos
        allocation_t *p = memory->head;
        size_t inicio_pool = (size_t)memory->pool;
        size_t fim_pool = inicio_pool + memory->total_size;
        
        // Verificar espaço antes do primeiro bloco
        size_t primeiro_inicio = (size_t)memory->head->start;
        if (primeiro_inicio > inicio_pool) {
            size_t tamanho_livre = primeiro_inicio - inicio_pool;
            fragmentos_livres++;
            if (tamanho_livre > maior_livre) maior_livre = tamanho_livre;
            total_bytes_livres += tamanho_livre;
        }

        // Verificar espaços entre blocos
        allocation_t *current = memory->head;
        while (current != NULL && current->next != NULL) {
            size_t fim_current = (size_t)current->start + current->size;
            size_t inicio_next = (size_t)current->next->start;
            
            if (fim_current < inicio_next) {
                size_t tamanho_livre = inicio_next - fim_current;
                fragmentos_livres++;
                if (tamanho_livre > maior_livre) maior_livre = tamanho_livre;
                total_bytes_livres += tamanho_livre;
            }
            current = current->next;
        }

        // Verificar espaço após o último bloco
        allocation_t *last = memory->head;
        while (last->next != NULL) last = last->next;
        size_t fim_last = (size_t)last->start + last->size;
        if (fim_last < fim_pool) {
            size_t tamanho_livre = fim_pool - fim_last;
            fragmentos_livres++;
            if (tamanho_livre > maior_livre) maior_livre = tamanho_livre;
            total_bytes_livres += tamanho_livre;
        }

        // Agora adicionar os blocos que estão marcados como livres
        allocation_t *bloco = memory->head;
        while (bloco != NULL) {
            if (bloco->free) {
                // Cada bloco livre é um fragmento adicional
                fragmentos_livres++;
                if (bloco->size > maior_livre) maior_livre = bloco->size;
            }
            bloco = bloco->next;
        }
    }

    printf("Estatísticas de Memória:\n");
    printf("  Total de blocos alocados: %zu\n", total_alocados);
    printf("  Memória total alocada: %zu bytes\n", total_bytes_alocados);
    printf("  Memória total livre: %zu bytes\n", total_bytes_livres);
    printf("  Maior bloco contíguo livre: %zu bytes\n", maior_livre);
    printf("  Número de fragmentos livres: %zu\n", fragmentos_livres);
    printf("  Eficiência: %.2f%%\n", 
           (memory->total_size > 0) ? 
           ((float)total_bytes_alocados / memory->total_size * 100) : 0.0f);
}

void mymemory_cleanup(mymemory_t *memory) {
	if (memory == NULL)
		return;
	allocation_t *atual = memory->head;

	while (atual != NULL) {
		allocation_t *proximo = atual->next;
		free(atual);
		atual = proximo;
	}
	free(memory->pool);
	free(memory);

    atual = NULL;
}

