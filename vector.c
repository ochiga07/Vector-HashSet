#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation){
    v->elem_size = elemSize;
    v->log_length = 0;
    if(initialAllocation != 0){
        v->allocated_length = initialAllocation;
        v->initial_allocation = initialAllocation;
    }else{
        //Default of 5
        v->allocated_length = 5;
        v->initial_allocation = 5;
    }
    v->vector_free_function = freeFn;
    v->base = malloc(v->allocated_length * v->elem_size);
}

void VectorDispose(vector *v){
    if(v->vector_free_function != NULL){
        void* curr = v->base;
        for(int i = 0;i < v->log_length;i++){
            v->vector_free_function(curr);
            curr = (char*) curr + v->elem_size;
        }
    }
    free(v->base);
}

int VectorLength(const vector *v){
    return v->log_length;
}

void *VectorNth(const vector *v, int position){
    assert(position >= 0 && position < v->log_length);
    void* loc = (char*)(v->base) + v->elem_size * position;
    return loc; 
}

void VectorReplace(vector *v, const void *elemAddr, int position){
    assert(position >= 0 && position < v->log_length);

    void* dest = (char*) v->base + position * v->elem_size;
    if(v->vector_free_function != NULL) v->vector_free_function(dest);

    memcpy(dest, elemAddr, v->elem_size);
}

void VectorInsert(vector *v, const void *elemAddr, int position){
    assert(position >= 0 && position <= v->log_length);
    if(v->log_length == v->allocated_length){
        v->allocated_length += v->initial_allocation;
        v->base = realloc(v->base, v->allocated_length * v->elem_size);
        //assert
    }

    void* dest = (char*) (v->base) + v->log_length * v->elem_size;
    void* src = (char*) dest - v->elem_size;

    int curr_pos = v->log_length - 1;

   
    while(curr_pos >= position){
        memcpy(dest, src, v->elem_size);
        dest = src;
        src = (char*) src - v->elem_size;
        curr_pos--;
    }
    
    
    memcpy(dest, elemAddr, v->elem_size);
    v->log_length++;
}

void VectorAppend(vector *v, const void *elemAddr){
    if(v->log_length == v->allocated_length){
        v->allocated_length += v->initial_allocation;
        v->base = realloc(v->base, v->allocated_length * v->elem_size);
        //assert
    }
    void* end_pos = (char*) (v->base) + v->log_length * v->elem_size;
    end_pos = memcpy(end_pos, elemAddr, v->elem_size);
    v->log_length++;
}

void VectorDelete(vector *v, int position){
    assert(position >= 0 && position <= v->log_length);

    void* dest = (char*) v->base + position * v->elem_size;
    if(v->vector_free_function != NULL) v->vector_free_function(dest);
    void* src = (char*) dest + v->elem_size;

    int indx = position;
    while(indx != v->log_length){
        memcpy(dest, src, v->elem_size);
        dest = src;
        src = (char*) src + v->elem_size;
        indx++;
    } 
    v->log_length--;
}

void VectorSort(vector *v, VectorCompareFunction compare){
    assert(compare != NULL);
    qsort(v->base, v->log_length, v->elem_size, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData){
    assert(mapFn != NULL);
    char* base = (char*) v->base;
    for(int i = 0;i < v->log_length;i++){
        mapFn((void*)(base), auxData);
        base += v->elem_size;
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ 
    assert(startIndex >= 0 && startIndex <= v->log_length);
    
    char* curr = (char*) v->base + startIndex * v->elem_size;
    if(isSorted){
        //Binary Search
        int* loc = bsearch(key, (void*) v->base, v->log_length - startIndex,
            v->elem_size,searchFn);
        if(loc != NULL){
            return ((char*) loc - (char*) v->base) / v->elem_size;
        }   
    }else{
        //Linear Search
        for(int i = startIndex;i < v->log_length;i++){
            if(searchFn((void*) curr, key) == 0) {
                return i;
            }
            curr += v->elem_size;
        }
    }

    return kNotFound; 
} 
