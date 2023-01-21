#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <include/hmap.h>

/**
 * @brief The default cap for a hash map.
 */
#define __HMAP_DEFAULT_CAP 20

void
hmap_new_cap(hmap_t *hmap, u_int8_t cap)
{
	hmap_node_t **map_arr = calloc(cap, sizeof(hmap_node_t*));
	hmap->arr = map_arr;
	hmap->cap = cap;
	hmap->size = 0;
}

void
hmap_new(hmap_t *hmap)
{
	return hmap_new_cap(hmap, __HMAP_DEFAULT_CAP);
}

int
hash_key(const char *key, u_int8_t cap)
{
	int hash = 0;

	for(int i = 0; key[i] != 0; i++) {
	// 	hash += (key[i] % cap);
		hash += (key[i] * key[i]) + i;

		if(hash <= cap) {
			hash%=cap;
		}
	}

	return hash % cap;
}

/* Map Methods */

hmap_node_t*
find_node(hmap_node_t* head, const char *key,
			bool need_prev, hmap_node_t** prev)
{
	hmap_node_t *temp = head;
	if(need_prev) {
		*prev = NULL;
	}

	while(temp) {
		if(!strcmp(key, temp->key)) {
			return temp;
		}

		if(need_prev) {
			*prev = temp;
		}

		temp = temp->next;
	}

	return NULL;
}

void*
hmap_get(hmap_t *hmap, const char *key)
{
	int index = hash_key(key, hmap->cap);
	hmap_node_t *elem = find_node(hmap->arr[index], key, false, NULL);

	return (elem) ? elem->value : NULL;
}

void
hmap_push(hmap_t *hmap, const char *key, void *value)
{
	int index = hash_key(key, hmap->cap);
	hmap_node_t *elem = find_node(hmap->arr[index], key, false, NULL);

	if(elem) {
		elem->value = value;
	} else {
		hmap_node_t *new_node = malloc(sizeof(hmap_node_t));
		new_node->key = (char *) key;
		new_node->value = value;

		// Inserting new_node at start of doubly linked list at arr[index]
		new_node->next = hmap->arr[index];
		hmap->arr[index] = new_node;
		hmap->size++;
	}
}

void
hmap_remove(hmap_t *hmap, const char *key)
{
	int index = hash_key(key, hmap->cap);

	hmap_node_t *prev;
	hmap_node_t *elem = find_node(hmap->arr[index], key, true, &prev);

	if(!elem) {
		return;
	}

	if(hmap->arr[index] == elem /* || !prev */) {
		hmap->arr[index] = elem->next;
	} else {
		prev->next = prev->next->next;
	}

	hmap->size--;

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
	free(hmap->arr);
}

/* Iterator */

hmap_node_t*
hmap_itr_adv(hmap_itr_t *itr)
{
	// Empty Map
	if(hmap_empty(itr->map)) {
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

void
hmap_itr_new(hmap_itr_t *itr, hmap_t *hmap)
{
	itr->index = hmap->cap;
	// Setting it as cap to signify no assignment of itr

	itr->map = hmap;
	itr->last = NULL;
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
	if(!hmap) {
		printf("HMap uninitialized.\n");
		return;
	}

	for(int i = 0; i<hmap->cap; i++) {
		hmap_print_node(hmap->arr[i]);
	}
}