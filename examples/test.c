#include <libhmap/hmap.h>
#include <stdio.h>

int main() {
	hmap_t *my_map = hmap_new();

	hmap_push(my_map, "hello", (void*) "world");
	printf("%s: %s\n", "hello", (char *) hmap_get(my_map, "hello"));

	int a = 10;
	hmap_push(my_map, "foo", (void*) &a);
	printf("%s: %d\n", "foo", *(int *) hmap_get(my_map, "foo"));

	hmap_itr_t *itr = hmap_itr_new(my_map);
	hmap_node_t *elem;

	while((elem = hmap_itr_adv(itr)) != NULL) {
		printf("%p\n", (char *) elem->value);
	}

	hmap_free(my_map);

	return 0;
}