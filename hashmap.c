#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Link {
    struct Link *prev;
    struct Link *next;
    unsigned int key;
    void *item;
};

struct HashMap {
    struct Link **lookup;
    int size;
};

struct HashMap *new_hashmap(int size) {
    struct HashMap *hm = malloc(sizeof(struct HashMap));
    hm->lookup = malloc(sizeof(struct Link*) * size);
    for (int i = 0; i < size; i++) {
        hm->lookup[i] = NULL;
    }
    hm->size = size;
    return hm;
}

void *get_from_hashmap(unsigned int hash_key, struct HashMap *hm) {
    int offset = hash_key % hm->size;
    struct Link *cur_node = hm->lookup[offset];
    while (cur_node != NULL) {
        if (cur_node->key == hash_key) {
            return cur_node->item;
        }
    }

    return NULL;
}