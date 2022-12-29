#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <include/hmap.h>

hmap_t*
hmap_new_cap(u_int8_t cap)
{
	hmap_t *new_map = malloc(sizeof(hmap_t));
	if(!new_map) {
		printf("Allocation Problem\n");
		exit(1);
	}
	memset(new_map, 0, sizeof(hmap_t));

	hmap_node_t **map_arr = calloc(cap, sizeof(hmap_node_t*));
	if(!map_arr) {
		printf("Allocation Problem\n");
		exit(1);
	}

	new_map->arr = map_arr;
	new_map->cap = cap;
	new_map->size = 0;

	return new_map;
}

hmap_t*
hmap_new(void)
{
	return hmap_new_cap(__HMAP_DEFAULT_CAP);
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
hmap_node_t*
find_node(hmap_node_t* head, const char *key)
{
	int n = strlen(key);

	hmap_node_t *temp = head;
	while(temp) {
		if(!strncmp(key, temp->key, n)) {
			return temp;
		}
		temp = temp->next;
	}

	return NULL;
}

void *
hmap_get(hmap_t *hmap, const char *key)
{
	int index = hash_key(key, hmap->cap);
	hmap_node_t *elem = find_node(hmap->arr[index], key);

	if(!elem) {
		return NULL;
	}
	return elem->value;
}

void
hmap_push(hmap_t *hmap, const char *key, void *value)
{
	if(!hmap) {
		return;
	}

	int index = hash_key(key, hmap->cap);
	hmap_node_t *elem = find_node(hmap->arr[index], key);

	if(elem) {
		free(elem->value);
		elem->value = value;
	} else {
		hmap_node_t *new_node = malloc(sizeof(hmap_node_t));
		if(!new_node) {
			printf("Allocation Problem\n");
			exit(1);
		}
		new_node->prev = NULL;

		new_node->key = (char *) key;
		new_node->value = value;

		// Inserting new_node at start of doubly linked list at arr[index]
		new_node->next = hmap->arr[index];
		if(hmap->arr[index]) {
			hmap->arr[index]->prev = new_node;
		}
		hmap->arr[index] = new_node;
		hmap->size++;
	}
}

void
hmap_remove(hmap_t *hmap, const char *key)
{
	int index = hash_key(key, hmap->cap);
	hmap_node_t *elem = find_node(hmap->arr[index], key);

	if(!elem) {
		return;
	}

	if(hmap->arr[index] == elem) {
		hmap->arr[index] = elem->next;
	} else {
		hmap_node_t *prev = elem->prev, *next = elem->next;
		prev->next = next;
		next->prev = prev;
	}

	hmap->size--;

	free(elem->key);
	free(elem->value);
	free(elem);
}

bool
hmap_empty(hmap_t *hmap)
{
	return hmap->size == 0;
}

/* Map Free */
void
hmap_node_free(hmap_node_t* head)
{
	hmap_node_t* temp;
	while(head) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

void
hmap_free(hmap_t *hmap)
{
	for(int i = 0; i<hmap->cap; i++) {
		hmap_node_free(hmap->arr[i]);
		hmap->arr[i] = NULL;
	}
	free(hmap);
}

/* Iterator */
hmap_node_t*
hmap_itr_adv(hmap_itr_t *itr)
{
	// Empty Map or Iterator
	if(!itr || hmap_empty(itr->map)) {
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
		return hmap_itr_adv(itr);
	}

	// Linked List Traversal
	if(itr->last) {
		itr->last = itr->last->next;
		return itr->last;
	}

	// New index in arr & it is empty & it is not last index
	if(!itr->last && !itr->map->arr[itr->index]) {
		itr->index++;
		return hmap_itr_adv(itr);
	}

	// New non-empty index in arr
	if(!itr->last) {
		itr->last = itr->map->arr[itr->index];
		return itr->last;
	}
}

// TODO: itr_prev, and a better logic for itr_adv

hmap_itr_t*
hmap_itr_new(hmap_t *hmap)
{
	if(!hmap) {
		return NULL;
	}

	hmap_itr_t *itr = malloc(sizeof(hmap_itr_t));
	itr->index = hmap->cap; // Setting it as cap to signify no assignment of itr
	itr->map = hmap;
	itr->last = NULL;

	return itr;
}

/* Print */
void
hmap_print_node(hmap_node_t *head)
{
	hmap_node_t* temp = head;
	while(temp) {
		printf("(%s:%p)->", temp->key, temp->value);
		temp = temp->next;
	}
	printf("N\n");
}

void
hmap_print(hmap_t *hmap)
{
	for(int i = 0; i<hmap->cap; i++) {
		hmap_print_node(hmap->arr[i]);
	}
}