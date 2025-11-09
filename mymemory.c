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
    
    while (current != NULL) {
        if (current->free && current->size >= size) {	// Se o bloco atual estiver vazio e tiver capacidade para ser utilizado, retorna
            current->free = 0;
            return current->start;
        }
        prev = current;
        current = current->next;
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

void mymemory_free(mymemory_t *memory, void *ptr){
	
	return;
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
void mymemory_stats(mymemory_t *memory){
	return;
}
void mymemory_cleanup(mymemory_t *memory){
	return;
}
