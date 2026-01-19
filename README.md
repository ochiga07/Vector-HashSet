# Vector and HashSet Implementation

Generic data structures in C that can store any type of data.

## What's Included

**Vector** - Dynamic array that grows automatically
- Store any data type
- Insert, append, delete operations
- Search and sort functionality
- Map function to apply operations to all elements

**HashSet** - Hash table for fast lookups
- Built on top of vector
- Fast insert and lookup (O(1) average)
- Uses separate chaining for collision handling
- Supports custom hash and compare functions

## Files

- `vector.h` / `vector.c` - Vector implementation
- `hashset.h` / `hashset.c` - HashSet implementation  
- `vectortest.c` - Tests for vector
- `hashsettest.c` - Tests for hashset

## How to Test

Compile and run vector tests:
```bash
gcc vector.c vectortest.c -o vectortest
./vectortest
```

Compile and run hashset tests:
```bash
gcc vector.c hashset.c hashsettest.c -o hashsettest
./hashsettest
```

## Quick Example

```c
// Vector of ints
vector v;
VectorNew(&v, sizeof(int), NULL, 4);
int x = 42;
VectorAppend(&v, &x);
VectorDispose(&v);

// HashSet of strings
hashset h;
HashSetNew(&h, sizeof(char*), 10, HashString, CompareString, FreeString);
char *word = "hello";
HashSetEnter(&h, &word);
char **found = HashSetLookup(&h, &word);
HashSetDispose(&h);
```