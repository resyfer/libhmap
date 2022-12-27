#include <sys/types.h>
#include <stdbool.h>

#ifndef __LIBHMAP_H
#define __LIBHMAP_H

struct node {
	struct node* prev;
	char *key;
	char *value;
	struct node* next;
};

struct map {
	struct node **arr;
	u_int8_t cap;
	u_int32_t size;
};

/*
 * Index signifies index in map's arr
 * last signifies address of node where itr is at (before advancing)
 *
 * If there isn't any elements at index in the arr, then last is NULL.
 *
 */
struct map_itr {
	struct map *map;
	u_int8_t index;
	struct node* last;
};

#define DEFAULT_CAP 25

struct map* new_map();
struct map* new_map_cap(u_int8_t cap);

char* map_get(struct map *map, const char *key);
void map_push(struct map *map, const char *key, const char *value);
void map_remove(struct map *map, const char *key);
bool map_empty(struct map *map);

struct map_itr* new_map_itr(struct map *map);
struct node* itr_adv(struct map_itr *itr);

void free_map(struct map *map);
void print_map(struct map *map);

#endif