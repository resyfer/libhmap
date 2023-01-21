# libhmap

A hash map library for C. Easy to use as well as takes in any data type.For
instructions on basic usage, visit the [examples](/examples/example.c).

For installation, please go [here](#installation). For documentation, please go
[here](https://libhmap-doxygen.netlify.app/).

## Installation

Make sure to have [make](https://www.gnu.org/software/make/),
[gcc](https://www.gnu.org/software/gcc/) and [git](https://git-scm.com/)
installed.

```bash
git clone https://github.com/resyfer/libhmap.git
cd libhmap
make install
cd ..
rm -rf libhmap
```

Provide the password when prompted.

## Development

Update makefile variable `MODE` to be:

```sh
MODE:=dev
```

and then:

```sh
make run # to run the example
# or
make test # to run the tests
```