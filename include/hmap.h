#ifndef __LIBHMAP_H
#define __LIBHMAP_H

#include <sys/types.h>
#include <stdbool.h>

struct __hmap_node {
	struct __hmap_node* prev;
	char *key;
	void *value;
	struct __hmap_node* next;
};
typedef struct __hmap_node hmap_node_t;

typedef struct {
	hmap_node_t **arr;
	u_int8_t cap;
	u_int32_t size;
} hmap_t;

/*
 * Index signifies index in map's arr
 * last signifies address of node where itr is at (before advancing)
 *
 * If there isn't any elements at index in the arr, then last is NULL.
 *
 */
typedef struct {
	hmap_t *map;
	u_int8_t index;
	hmap_node_t* last;
} hmap_itr_t;

#define __HMAP_DEFAULT_CAP 25

hmap_t* hmap_new(void);
hmap_t* hmap_new_cap(u_int8_t cap);

void* hmap_get(hmap_t *hmap, const char *key);
void hmap_push(hmap_t *hmap, const char *key, void *value);
void hmap_remove(hmap_t *hmap, const char *key);
bool hmap_empty(hmap_t *hmap);

hmap_itr_t* hmap_itr_new(hmap_t *hmap);
hmap_node_t* hmap_itr_adv(hmap_itr_t *itr);

void hmap_free(hmap_t *hmap);
void hmap_print(hmap_t *hmap);

#endif