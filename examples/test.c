#include <libhmap/libhmap.h>
#include <stdio.h>

int main() {
	struct map *my_map = new_map();

	map_push(my_map, "hello", "world");
	printf("%s: %s\n", "hello", map_get(my_map, "hello"));

	struct map_itr *itr = new_map_itr(my_map);
	struct node *elem;

	while((elem = itr_adv(itr)) != NULL) {
		printf("%s\n", elem->value);
	}

	free_map(my_map);

	return 0;
}