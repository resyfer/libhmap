#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <include/libhmap.h>

struct map*
new_map_cap(u_int8_t cap)
{
	struct map *new_map = malloc(sizeof(struct map));
	if(!new_map) {
		printf("Allocation Problem\n");
		exit(1);
	}
	memset(new_map, 0, sizeof(struct map));

	struct node **map_arr = calloc(cap, sizeof(struct node*));
	if(!map_arr) {
		printf("Allocation Problem\n");
		exit(1);
	}

	new_map->arr = map_arr;
	new_map->cap = cap;
	new_map->size = 0;

	return new_map;
}

struct map*
new_map()
{
	return new_map_cap(DEFAULT_CAP);
}

int
hash_key(const char *key, u_int8_t cap)
{
	int hash = 0;
	for(int i = 0; key[i] != 0; i++) {
		hash += (key[i] % cap);
	}
	hash %= cap;
	return hash;
}

/* Map Methods */
struct node*
find_node(struct node* head, const char *key)
{
	int n = strlen(key);

	struct node *temp = head;
	while(temp) {
		if(!strncmp(key, temp->key, n)) {
			return temp;
		}
		temp = temp->next;
	}

	return NULL;
}

char *
map_get(struct map *map, const char *key)
{
	int index = hash_key(key, map->cap);
	struct node *elem = find_node(map->arr[index], key);

	if(!elem) {
		return NULL;
	}
	return elem->value;
}

void
map_push(struct map *map, const char *key, const char *value)
{
	if(!map) {
		return;
	}

	int index = hash_key(key, map->cap);
	struct node *elem = find_node(map->arr[index], key);

	if(elem) {
		free(elem->value);
		char *new_val = malloc(sizeof(char) * (strlen(value) + 1));
		strcpy(new_val, value);
		elem->value = new_val;
	} else {
		struct node *new_node = malloc(sizeof(struct node));
		new_node->prev = NULL;

		new_node->key = malloc(sizeof(char) * (strlen(key) + 1));
		if(!new_node) {
			printf("Allocation Problem\n");
			exit(1);
		}
		strcpy(new_node->key, key);

		new_node->value = malloc(sizeof(char) * (strlen(value) + 1));
		if(!new_node) {
			printf("Allocation Problem\n");
			exit(1);
		}
		strcpy(new_node->value, value);

		// Inserting new_node at start of doubly linked list at arr[index]
		new_node->next = map->arr[index];
		if(map->arr[index]) {
			map->arr[index]->prev = new_node;
		}
		map->arr[index] = new_node;
		map->size++;
	}
}

void
map_remove(struct map *map, const char *key)
{
	int index = hash_key(key, map->cap);
	struct node *elem = find_node(map->arr[index], key);

	if(!elem) {
		return;
	}

	if(map->arr[index] == elem) {
		map->arr[index] = elem->next;
	} else {
		struct node *prev = elem->prev, *next = elem->next;
		prev->next = next;
		next->prev = prev;
	}

	map->size--;

	free(elem->key);
	free(elem->value);
	free(elem);
}

bool
map_empty(struct map *map)
{
	return map->size == 0;
}

/* Map Free */
void
node_free(struct node* head)
{
	struct node* temp;
	while(head) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

void
free_map(struct map *map)
{
	for(int i = 0; i<map->cap; i++) {
		node_free(map->arr[i]);
		map->arr[i] = NULL;
	}
	free(map);
}

/* Iterator */
struct node*
itr_adv(struct map_itr *itr)
{
	// Empty Map or Iterator
	if(!itr || map_empty(itr->map)) {
		return NULL;
	}

	// Uninitialized Iterator
	if(itr->index == itr->map->cap) {
		// Finding first non-empty node in arr
		u_int8_t i;
		for(i = 0; !itr->map->arr[i]; i++);
		itr->last = itr->map->arr[i];
		itr->index = i;
		return itr->last;
	}

	// Empty last element in arr
	if(itr->map->cap == itr->index + 1 && !itr->map->arr[itr->index]) {
		return NULL;
	}

	// Last element of map reached, considering last element of arr is not empty
	if(itr->map->cap == itr->index + 1 && (itr->last && !itr->last->next)) {
		return NULL;
	}

	// Last element of linked list reached
	if(itr->last && !itr->last->next) {
		itr->index++;
		itr->last = NULL;
		return itr_adv(itr);
	}

	// Linked List Traversal
	if(itr->last) {
		itr->last = itr->last->next;
		return itr->last;
	}

	// New index in arr & it is empty & it is not last index
	if(!itr->last && !itr->map->arr[itr->index]) {
		itr->index++;
		return itr_adv(itr);
	}

	// New non-empty index in arr
	if(!itr->last) {
		itr->last = itr->map->arr[itr->index];
		return itr->last;
	}
}

// TODO: itr_prev, and a better logic for itr_adv

struct map_itr*
new_map_itr(struct map *map)
{
	if(!map) {
		return NULL;
	}

	struct map_itr *itr = malloc(sizeof(struct map_itr));
	itr->index = map->cap; // Setting it as cap to signify no assignment of itr
	itr->map = map;
	itr->last = NULL;

	return itr;
}

/* Print */
void
print_node(struct node *head)
{
	struct node* temp = head;
	while(temp) {
		printf("(%s:%s)->", temp->key, temp->value);
		temp = temp->next;
	}
	printf("N\n");
}

void
print_map(struct map *map)
{
	for(int i = 0; i<map->cap; i++) {
		print_node(map->arr[i]);
	}
}