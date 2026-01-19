// vector.h
// Generic vector that can store any type

#ifndef _vector_
#define _vector_

#include "bool.h"

// Function pointer types
typedef int (*VectorCompareFunction)(const void *elem1, const void *elem2);
typedef void (*VectorMapFunction)(void *elem, void *auxData);
typedef void (*VectorFreeFunction)(void *elem);

// Vector struct
typedef struct {
  int log_length;
  int allocated_length;
  int initial_allocation;
  int elem_size;
  void (*vector_free_function)(void *elem);
  void* base;
} vector;

// Initialize a new vector
void VectorNew(vector *v, int elemSize, VectorFreeFunction freefn, int initialAllocation);

// Free the vector
void VectorDispose(vector *v);

// Get number of elements
int VectorLength(const vector *v);

// Get pointer to element at position
void *VectorNth(const vector *v, int position);

// Insert element at position
void VectorInsert(vector *v, const void *elemAddr, int position);

// Add element to end
void VectorAppend(vector *v, const void *elemAddr);

// Replace element at position
void VectorReplace(vector *v, const void *elemAddr, int position);

// Delete element at position
void VectorDelete(vector *v, int position);

// Search for element (returns position or -1)
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchfn, int startIndex, bool isSorted);

// Sort the vector
void VectorSort(vector *v, VectorCompareFunction comparefn);

// Apply function to each element
void VectorMap(vector *v, VectorMapFunction mapfn, void *auxData);

#endif