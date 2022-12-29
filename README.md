# libhmap

A C library for a hash map. Pretty easy to use. For instructions on usage, please go [here](#usage).
For installation, please go [here](#installation). For documentation, please go [here](#documentation)

## Usage

### TL;DR

The shown below is a tl;dr version. Proper instructions are [here](#step-1)

```c
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
```

Compile and run:

```bash
$ gcc test.c -o test.o -lhmap
$ ./test.o
```

### Step 1

Import the library:

```c
#include <libhmap/hmap.h>
```

**NOTE**: Link the library when compiling

```bash
$ gcc a.c -o a.o -lhmap
```

### Step 2

Initialize a hash map:

```c
hmap_t *my_map = hmap_new();
```

This will create a hash map with default size of internal array (which is set at 25 currently).

To create a smaller internal array, say, of size 10:

```c
hmap_t *my_map = hmap_new_cap(10);
```

**NOTE**: This does not decrease the number of elements the hash map can hold, which honestly depends on the memory you have. It just sets the internal array to smaller size.

### Step 3

To push:

```c
hmap_push(my_map, "hello", (void*) "world");
//or
hmap_push(my_map, "foo", (void*) &my_number);
```

**NOTE**: If the same key is more than once,
the latest push for it will overide the value.

To get the value of a key:

```c
char *val = hmap_get(my_map, "hello");
//or
int *num = hmap_get(my_map, "foo");
```

**NOTE**: None of the values are copied into the hash map so memory management is left to the user for these, even after `hmap_free`.

### Step 4

Free up the hash map's allocated memory after use:

```c
hmap_free(my_map);
```

## Installation

Make sure to have [make](https://www.gnu.org/software/make/), [gcc](https://www.gnu.org/software/gcc/) and [git](https://git-scm.com/) installed

```bash
git clone https://github.com/resyfer/libhmap.git
cd libhmap
make install
cd ..
rm -rf libhmap
```

Provide the password when prompted.

## Documentation

### Initialize

```c
hmap_t* hmap_new();
hmap_t* hmap_new_cap(u_int8_t cap);
```

### Create-Read-Update-Delete

```c
void* hmap_get(hmap_t *hmap, const char *key); // Read
void hmap_push(hmap_t *hmap, const char *key, void *value); // Create, Update
void hmap_remove(hmap_t *hmap, const char *key); // Delete
```

### Iterator

Iterator iterates lazily over all the elements currently present in
the hash map (this does not include `NULL` elements in
underlying data structure)

#### Create

```c
hmap_itr_t* hmap_itr_new(hmap_t *hmap);
```

#### Advance by 1

```c
hmap_node_t* hmap_itr_adv(hmap_itr_t *itr);
```

#### Example

```c
hmap_itr_t *itr = hmap_itr_new(my_map);
hmap_node_t *elem;

while((elem = itr_adv(itr)) != NULL) {
  printf("%s: %s\n", elem->key, elem->value);
}

```

### Free

Freeing up hash map.

```c
void hmap_free(hmap_t *hmap);
```

### Misc

```c
bool hmap_empty(hmap_t *hmap); // True if map is empty (ie. no elements)
void hmap_print(hmap_t *hmap); // Outputs the whole internal data structure in a visual form
```
