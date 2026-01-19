#include "hashset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static int HashInt(const void *elem, int numBuckets)
{
    return abs(*(int *)elem) % numBuckets;
}

static int CompareInt(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

static void TestBasicInt()
{
    printf("\n=== Test 1: Basic int operations ===\n");
    hashset h;
    HashSetNew(&h, sizeof(int), 10, HashInt, CompareInt, NULL);
    
    assert(HashSetCount(&h) == 0);
    printf("Empty hashset count is 0\n");
    
    int val = 42;
    HashSetEnter(&h, &val);
    assert(HashSetCount(&h) == 1);
    
    int *found = (int *)HashSetLookup(&h, &val);
    assert(found != NULL && *found == 42);
    printf("Insert and lookup works\n");
    
    int missing = 99;
    found = (int *)HashSetLookup(&h, &missing);
    assert(found == NULL);
    printf("Lookup returns NULL for missing element\n");
    
    HashSetDispose(&h);
}

static void TestOverwrite()
{
    printf("\n=== Test 2: Overwriting elements ===\n");
    hashset h;
    HashSetNew(&h, sizeof(int), 5, HashInt, CompareInt, NULL);
    
    int val = 10;
    HashSetEnter(&h, &val);
    int count1 = HashSetCount(&h);
    
    val = 10;
    HashSetEnter(&h, &val);
    int count2 = HashSetCount(&h);
    
    printf("Count after first insert: %d, after second: %d\n", count1, count2);
    
    int *found = (int *)HashSetLookup(&h, &val);
    assert(found != NULL && *found == 10);
    printf("Element still accessible after re-insert\n");
    
    HashSetDispose(&h);
}

static int HashCollision(const void *elem, int numBuckets)
{
    return 0;
}

static void TestCollisions()
{
    printf("\n=== Test 3: Hash collisions ===\n");
    hashset h;
    HashSetNew(&h, sizeof(int), 10, HashCollision, CompareInt, NULL);
    
    for (int i = 0; i < 5; i++) {
        HashSetEnter(&h, &i);
    }
    
    for (int i = 0; i < 5; i++) {
        int *found = (int *)HashSetLookup(&h, &i);
        assert(found != NULL && *found == i);
    }
    printf("All colliding elements are findable\n");
    
    HashSetDispose(&h);
}

static int HashString(const void *elem, int numBuckets)
{
    char *s = *(char **)elem;
    int hash = 0;
    while (*s) {
        hash += *s;
        s++;
    }
    return hash % numBuckets;
}

static int CompareString(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

static void FreeString(void *elem)
{
    free(*(char **)elem);
}

static void TestStrings()
{
    printf("\n=== Test 4: String operations ===\n");
    hashset h;
    HashSetNew(&h, sizeof(char *), 10, HashString, CompareString, FreeString);
    
    char *words[] = {"hello", "world", "test", "hashset"};
    for (int i = 0; i < 4; i++) {
        char *copy = strdup(words[i]);
        HashSetEnter(&h, &copy);
    }
    
    for (int i = 0; i < 4; i++) {
        char **found = (char **)HashSetLookup(&h, &words[i]);
        assert(found != NULL);
        assert(strcmp(*found, words[i]) == 0);
    }
    printf("String insert and lookup works\n");
    
    char *missing = "nothere";
    char **found = (char **)HashSetLookup(&h, &missing);
    assert(found == NULL);
    printf("Missing string returns NULL\n");
    
    HashSetDispose(&h);
}

static void PrintInt(void *elem, void *aux)
{
    printf("%d ", *(int *)elem);
}

static void TestMap()
{
    printf("\n=== Test 5: Map function ===\n");
    hashset h;
    HashSetNew(&h, sizeof(int), 5, HashInt, CompareInt, NULL);
    
    for (int i = 1; i <= 10; i++) {
        HashSetEnter(&h, &i);
    }
    
    printf("Elements in hashset: ");
    HashSetMap(&h, PrintInt, NULL);
    printf("\nMap function works\n");
    
    HashSetDispose(&h);
}

static void TestLarge()
{
    printf("\n=== Test 6: Large dataset ===\n");
    hashset h;
    HashSetNew(&h, sizeof(int), 100, HashInt, CompareInt, NULL);
    
    int n = 1000;
    for (int i = 0; i < n; i++) {
        HashSetEnter(&h, &i);
    }
    
    printf("Inserted %d elements\n", n);
    
    for (int i = 0; i < n; i++) {
        int *found = (int *)HashSetLookup(&h, &i);
        assert(found != NULL && *found == i);
    }
    printf("All %d elements are findable\n", n);
    
    HashSetDispose(&h);
}

struct person {
    char name[50];
    int age;
};

static int HashPerson(const void *elem, int numBuckets)
{
    struct person *p = (struct person *)elem;
    int hash = 0;
    for (int i = 0; p->name[i]; i++) {
        hash += p->name[i];
    }
    return hash % numBuckets;
}

static int ComparePerson(const void *a, const void *b)
{
    struct person *p1 = (struct person *)a;
    struct person *p2 = (struct person *)b;
    return strcmp(p1->name, p2->name);
}

static void TestStruct()
{
    printf("\n=== Test 7: Custom struct ===\n");
    hashset h;
    HashSetNew(&h, sizeof(struct person), 10, HashPerson, ComparePerson, NULL);
    
    struct person p1 = {"Alice", 25};
    struct person p2 = {"Bob", 30};
    struct person p3 = {"Charlie", 35};
    
    HashSetEnter(&h, &p1);
    HashSetEnter(&h, &p2);
    HashSetEnter(&h, &p3);
    
    struct person lookup = {"Bob", 0};
    struct person *found = (struct person *)HashSetLookup(&h, &lookup);
    assert(found != NULL);
    assert(strcmp(found->name, "Bob") == 0);
    assert(found->age == 30);
    printf("Struct insert and lookup works\n");
    
    HashSetDispose(&h);
}

static void TestEdgeCases()
{
    printf("\n=== Test 8: Edge cases ===\n");
    hashset h;
    
    HashSetNew(&h, sizeof(int), 1, HashInt, CompareInt, NULL);
    for (int i = 0; i < 10; i++) {
        HashSetEnter(&h, &i);
    }
    
    for (int i = 0; i < 10; i++) {
        int *found = (int *)HashSetLookup(&h, &i);
        assert(found != NULL);
    }
    printf("Works with single bucket\n");
    HashSetDispose(&h);
    
    HashSetNew(&h, sizeof(int), 1000, HashInt, CompareInt, NULL);
    for (int i = 0; i < 50; i++) {
        HashSetEnter(&h, &i);
    }
    assert(HashSetCount(&h) > 0);
    printf("Works with many buckets\n");
    HashSetDispose(&h);
}

int main()
{
    printf("Starting HashSet Tests...\n");
    
    TestBasicInt();
    TestOverwrite();
    TestCollisions();
    TestStrings();
    TestMap();
    TestLarge();
    TestStruct();
    TestEdgeCases();
    
    printf("\nAll tests passed!\n");
    return 0;
}