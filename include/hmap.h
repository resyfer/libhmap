#ifndef __LIBHMAP_H
#define __LIBHMAP_H

#include <sys/types.h>
#include <stdbool.h>

/**
 * @mainpage
 * A hash map library for C. It hashes the key in a fair and fast manner
 * to decide which linked list to put the value in, and thus pushing values
 * as well as retrieving values from the hash map takes the average O(1) time
 * complexities.
 */

/**
 * @brief Hash map's elemental block
 *
 * Bsaically a singly linked list
 */
typedef struct hmap_node_t {
	char *key; /*!< Key of the element */
	void *value; /*!< Value of the element */
	struct hmap_node_t* next; /*!< Next Node in linked list */
} hmap_node_t;

/**
 * @brief Hash map data structure
 *
 * Contains an array of linked lists of nodes.
 */
typedef struct {
	hmap_node_t **arr; /*!< Internal array of hash map comtaining linked lists
						of hmap_node_t */
	u_int8_t cap; /*!< Maximum size of internal array of hash map, ie.
					the number of linked lists it has */
	u_int32_t size; /*!< Number of elements in the hash map */
} hmap_t;

/**
 * @brief An iterator to a hash map.
 */
typedef struct {
	hmap_t *map; /*!< Hash map for which it the iterator of */
	u_int8_t index; /*!< The index of the linked list in the internal array
					where it was last at */
	hmap_node_t* last; /*!< The pointer to the last node it was at */
} hmap_itr_t;

/**
 * @brief Initialize a hash map
 *
 * @param hmap Hash map instance to initialize
 */
void hmap_new(hmap_t *hmap);

/**
 * @brief Initialize a hash map with a specific size for internal array.
 *
 * @note This does not change the upper limit for the size of the hash
 * 		 map, which is that the hash map will continue to take in elements
 *       till `min(memory, 2^32 - 1)`
 *
 * @param hmap Hash map instance to initialize
 * @param cap Internal array size
 */
void hmap_new_cap(hmap_t *hmap, u_int8_t cap);

/**
 * @brief Get an element from the hash map using its key.
 *
 * @param hmap Hash map instance
 * @param key Key of the element to get
 * @return void* Element
 */
void* hmap_get(hmap_t *hmap, const char *key);

/**
 * @brief Push an element with its key to the hash map
 *
 * @param hmap Hash map instance
 * @param key Key of the element to add
 * @param value Element
 */
void hmap_push(hmap_t *hmap, const char *key, void *value);

/**
 * @brief Remove an element using its key from the hash map
 *
 * @param hmap Hash map instance
 * @param key Key of the element to remove
 */
void hmap_remove(hmap_t *hmap, const char *key);

/**
 * @brief Check if hash map is empty
 *
 * @param hmap Hash map instance
 * @return true Hash map is empty
 * @return false Hash map is not empty
 */
bool hmap_empty(hmap_t *hmap);

/* Iterator */

/**
 * @brief Initialize an iterator to a hash map
 *
 * @param itr Itrator to initialize
 * @param hmap Hash map instance
 */
void hmap_itr_new(hmap_itr_t *itr, hmap_t *hmap);

/**
 * @brief Advance the iterator to the next non-null
 * 		  element in the hash map.
 *
 * The order of the elements is based on the internal
 * algorithms and may not be in the order that they
 * are pushed to the hash map
 *
 * @param itr
 * @return hmap_node_t*
 */
hmap_node_t* hmap_itr_adv(hmap_itr_t *itr);

/**
 * @brief Free the internal elemental blocks of a
 * 				hash map.
 *
 * @param hmap Hash map instance
 */
void hmap_free(hmap_t *hmap);

/**
 * @brief Print a hash map.
 *
 * @param hmap Hash map instance
 */
void hmap_print(hmap_t *hmap);

#endif