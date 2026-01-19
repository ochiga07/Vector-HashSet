#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	assert(elemSize > 0 && numBuckets > 0 && hashfn != NULL && comparefn != NULL);
	h->elem_size = elemSize;
	h->log_length = 0;
	h->num_buckets = numBuckets;
	h->hash_fn = hashfn;
	h->compare_fn = comparefn;
	h->free_fn = freefn;
	h->buckets = (vector*) malloc(h->num_buckets * sizeof(vector));

	//printf("Num Buckets -> %d\n", numBuckets);
	for(int i = 0;i < h->num_buckets;i++){
		VectorNew(&h->buckets[i], elemSize, freefn, 0);
		//printf("Vector %d Adress -> %p\n", i, &h->buckets[i]);
	}
}

void HashSetDispose(hashset *h){
	for(int i = 0;i < h->num_buckets;i++){
		VectorDispose(&h->buckets[i]);
	}
	free(h->buckets);
}

int HashSetCount(const hashset *h){ 
	return h->log_length; 
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData){
	for(int i = 0;i < h->num_buckets;i++){
		VectorMap(&h->buckets[i], mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr){
	assert(elemAddr != NULL);

	
	int bucket_pos = h->hash_fn(elemAddr, h->num_buckets);
	assert(bucket_pos >= 0 && bucket_pos < h->num_buckets);
	int pos = VectorSearch(&h->buckets[bucket_pos],elemAddr, h->compare_fn, 0, false);
	if(pos != -1){
		VectorReplace(&h->buckets[bucket_pos], elemAddr, pos);
	}else{
		VectorAppend(&h->buckets[bucket_pos], elemAddr);
	}

	h->log_length++;
}

void *HashSetLookup(const hashset *h, const void *elemAddr){ 

	assert(elemAddr != NULL);

	int bucket_pos = h->hash_fn(elemAddr, h->num_buckets);
	assert(bucket_pos >= 0 && bucket_pos < h->num_buckets);
	int pos = VectorSearch(&h->buckets[bucket_pos],elemAddr, h->compare_fn, 0, false);

	if(pos != -1){
		return VectorNth(&h->buckets[bucket_pos], pos);
	}

	return NULL; 
}
