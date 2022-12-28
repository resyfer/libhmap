# libhmap

A C library for a hash map. Pretty easy to use. For instructions on usage, please go [here](#usage).
For installation, please go [here](#installation). For documentation, please go [here](#documentation)

## Usage

### TL;DR

The shown below is a tl;dr version. Proper instructions are [here](#step-1)

```c
#include <libhmap/libhmap.h>
#include <stdio.h>

int main() {
  struct map *my_map = new_map();

  map_push(my_map, "hello", "world");
  printf("%s : %s\n", "hello", map_get(my_map, "hello"));

  free_map(my_map);
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
#include <libhmap/libhmap.h>
```

**NOTE**: Link the library when compiling

```bash
$ gcc a.c -o a.o -lhmap
```

### Step 2

Initialize a hash map:

```c
struct map *my_map = new_map();
```

This will create a hash map with default size of internal array (which is set at 25 currently).

To create a smaller internal array, say, of size 10:

```c
struct map *my_map = new_map_cap(10);
```

**NOTE**: This does not decrease the number of elements the hash map can hold, which honestly depends on the memory you have. It just sets the internal array to smaller size.

### Step 3

To push:

```c
map_push(my_map, "hello", "world");
```

**NOTE**: If the same key is more than once,
the latest push for it will overide the value.

To get the value of a key:

```c
char *val = map_get(my_map, "hello");
```

For keys and values you may use string literals,
strings as characters arrays, and even dynamically allocated string
for both `map_push` and `map_get`.

**NOTE**: Any strings passed are copied into the hash map,
so please free up any dynamically allocated strings you may have used eg:

```c
char *var = malloc(5 * sizeof(char));
char *k = map_get(my_map, var);
free(var);
```

### Step 4

Free up the hash map's allocated memory after use:

```c
free_map(my_map);
```

**NOTE**: Any strings stored from `map_get` are dangling pointers after `free_map`.

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
struct map* new_map();
struct map* new_map_cap(u_int8_t cap);
```

### Create-Read-Update-Delete

```c
char* map_get(struct map *map, const char *key); // Read
void map_push(struct map *map, const char *key, const char *value); // Create, Update
void map_remove(struct map *map, const char *key); // Delete
```

### Iterator

Iterates lazily over all the elements currently present in
the hash map (this does not include `NULL` elements in
underlying data structure)

#### Create

```c
struct map_itr* new_map_itr(struct map *map);
```

#### Advance by 1

```c
struct node* itr_adv(struct map_itr *itr);
```

#### Example

```c
struct map_itr *itr = new_map_itr(my_map);
struct node *elem;

while((elem = itr_adv(itr)) != NULL) {
  printf("%s: %s\n", elem->key, elem->value);
}

```

### Free

**Please free your hash maps after use to prevent memory leaks.**

```c
void free_map(struct map *map);
```

### Misc

```c
bool map_empty(struct map *map); // True if map is empty (ie. no elements)
void print_map(struct map *map); // Outputs the whole internal data structure in a visual form
```
