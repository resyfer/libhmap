#include <stdio.h>
#include <assert.h>
#include <libhmap/hmap.h>

void crud_test(void)
{
	printf("\n!!--CRUD Test--!!\n");

	printf("HMap initialization...");
	hmap_t h;
	hmap_new(&h);
	assert(h.arr != NULL);
	printf("OK ✅\n");

	printf("HMap Push...");
	int a = 10;
	hmap_push(&h, "a", (void*) &a);
	assert(h.size == 1);
	assert(!hmap_empty(&h));
	printf("OK ✅\n");

	printf("HMap Get...");
	assert(*((int*) hmap_get(&h, "a")) == a);
	printf("OK ✅\n");

	printf("HMap Remove...");
	hmap_remove(&h, "a");
	assert(h.size == 0);
	assert(hmap_get(&h, "a") == NULL);
	printf("OK ✅\n");

	printf("HMap Empty...");
	assert(hmap_empty(&h));
	printf("OK ✅\n");

	printf("HMap Free...");
	hmap_free(&h);
	printf("OK ✅\n");
}

void itr_test(void)
{
	printf("\n!!--Iterator Test--!!\n");

	printf("HMap initialization...");
	hmap_t h;
	hmap_new_cap(&h, 5);
	assert(h.arr != NULL);
	printf("OK ✅\n");

	int a = 10, b = 15;
	hmap_push(&h, "a", &a);
	hmap_push(&h, "b", &b);

	printf("HMap Iterator Initialization...");
	hmap_itr_t itr;
	hmap_itr_new(&itr, &h);
	printf("OK ✅\n");

	printf("Iterator Advance...");
	int c = *((int*) hmap_itr_adv(&itr)->value);
	int d = *((int*) hmap_itr_adv(&itr)->value);

	assert(hmap_itr_adv(&itr) == NULL);
	printf("OK ✅, Itr -> %d %d\n", c, d);

	printf("Map Print:\n");
	hmap_print(&h);
}

int main() {

	crud_test();
	itr_test();

	return 0;
}