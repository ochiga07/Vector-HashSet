// hashset.h
// Hash table implementation

#ifndef _hashset_
#define _hashset_
#include "vector.h"

// Function pointer types
typedef int (*HashSetHashFunction)(const void *elemAddr, int numBuckets);
typedef int (*HashSetCompareFunction)(const void *elem1, const void *elem2);
typedef void (*HashSetMapFunction)(void *elem, void *auxData);
typedef void (*HashSetFreeFunction)(void *elem);

// Hashset struct
typedef struct {
  int elem_size;
  int log_length;
  int num_buckets;
  HashSetHashFunction hash_fn;
  HashSetCompareFunction compare_fn;
  HashSetFreeFunction free_fn;
  vector* buckets;
} hashset;

// Initialize hashset
void HashSetNew(hashset *h, int elemSize, int numBuckets, 
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn);

// Free hashset
void HashSetDispose(hashset *h);

// Get number of elements
int HashSetCount(const hashset *h);

// Add element to hashset
void HashSetEnter(hashset *h, const void *elemAddr);

// Find element in hashset (returns pointer or NULL)
void *HashSetLookup(const hashset *h, const void *elemAddr);

// Apply function to each element
void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData);
     
#endif