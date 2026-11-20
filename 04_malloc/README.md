# 42 Malloc

Custom C implementation of the malloc library functions. It creates the shared library `libft_malloc.so` which can be used to replace malloc in any system commands.

It exposes the following methods:

```c
//mandatory part
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

// Debug calls
void show_alloc_mem(); // Print informations about allocated zones

//bonus part
void show_alloc_mem_ex(); // Print a hex dump of the heaps
```

## Subject

The goal of this project is to implement a custom dynamic memory allocation mechanism in C that replicates the behavior of standard `libc` functions without modifying or recompiling existing system binaries.

### Core Requirements & Constraints

- **Shared Library & Symlink:** The Makefile must compile a dynamic library named `libft_malloc_$HOSTTYPE.so` (defaulting `$HOSTTYPE` to `` `uname -m`_`uname -s` `` if undefined or empty) and create a symbolic link `libft_malloc.so` pointing to it.
- **Global Variables:** Maximum of **1 global variable** allowed to manage memory allocations (plus **1 additional global** if implementing thread-safety).
- **Allowed Functions:** `mmap(2)`, `munmap(2)`, `getpagesize()` (macOS) / `sysconf(_SC_PAGESIZE)` (Linux), `getrlimit(2)`, `libft` functions, and `libpthread` (for bonus). Using the system `malloc` is strictly forbidden.
- **Memory Alignment:** Returned pointers must be properly aligned (16-byte alignment on 64-bit architectures).
- **Error Handling:** Functions must handle errors safely and never trigger undefined behavior or segmentation faults (`malloc` and `realloc` return `NULL` on failure; `free(NULL)` performs no operation).

### Zone Architecture

To minimize `mmap()` and `munmap()` system calls, memory is divided into three pre-allocated strategies:

- **TINY** (`1` to `n` bytes): Stored in pre-allocated zones of size `N`.
	- **1** to **1024** bytes
- **SMALL** (`n + 1` to `m` bytes): Stored in pre-allocated zones of size `M`.
	- **1025** to **16.384** bytes
- **LARGE** (`m + 1` bytes and above): Allocated individually outside standard zones via dedicated `mmap()` calls.
	- **block** > **16384** bytes

> **Sizing Rules:** Zone sizes `N` and `M` must be multiples of the system page size (`sysconf(_SC_PAGESIZE)` / `getpagesize()`), and each pre-allocated zone must hold **at least 64 allocations** (including internal metadata headers).

### Visualization (`show_alloc_mem`)

Must print the state of currently allocated memory blocks sorted by increasing memory address, followed by the total allocated bytes:

```text
TINY : 0xA0000
0xA0020 - 0xA004A : 42 bytes
0xA006A - 0xA00BE : 84 bytes
SMALL : 0xAD000
0xAD020 - 0xADEAD : 3725 bytes
LARGE : 0xB0000
0xB0020 - 0xBBEEF : 48847 bytes
Total : 52698 bytes
```

## Commands

### Build

```bash
make
make clean # Clean temporary built files
make fclean # Clean all built files
make re
```

### Format

```bash
make norm # check norma implementation
make format # apply norma format to all files
```


### Testing

```bash
# valgrind
make valgrind
make clean_valgrind

# strace
make test_strace
make clean_strace

# 
make test
```

## Bonus

- Show hex dump of allocated zones with `show_alloc_mem_ex()`


### Structure

todo

### Constants

For better performance, we preallocate heaps for small malloc calls.

```c
todo
```