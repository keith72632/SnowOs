#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct dynamic_mem_node {
    uint32_t size;
    bool used;
    struct dynamic_mem_node *next;
    struct dynamic_mem_node *prev;
} dynamic_mem_node_t;

void init_dynamic_mem();
void *find_best_mem_block(dynamic_mem_node_t *dynamic_mem, size_t size);
void *mem_alloc(size_t size);
void mem_free(void *p);
void *merge_next_node_into_current(dynamic_mem_node_t *current_mem_node);
void *merge_current_node_into_previous(dynamic_mem_node_t *current_mem_node);
