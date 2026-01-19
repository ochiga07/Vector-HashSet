#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to print chars
static void PrintChar(void *elem, void *fp)
{
  printf("%c", *(char *)elem);
}

// Compare function for chars
static int CompareChar(const void *a, const void *b)
{
  return (*(char *)a - *(char *)b);
}

// Test basic append
static void TestAppend(vector *v)
{
  char ch;
  
  // Add alphabet
  for (ch = 'A'; ch <= 'Z'; ch++)
    VectorAppend(v, &ch);
  
  printf("Alphabet: ");
  VectorMap(v, PrintChar, stdout);
  
  // Add digits
  for (int i = 0; i < 10; i++) {
    ch = '0' + i;
    VectorAppend(v, &ch);
  }
  printf("\nWith digits: ");
  VectorMap(v, PrintChar, stdout);
}

// Test search
static void TestSearch(vector *v, char ch)
{
  int found = VectorSearch(v, &ch, CompareChar, 0, true);
  int foundLinear = VectorSearch(v, &ch, CompareChar, 0, false);
  
  printf("\nSearching for '%c' - Binary: %s, Linear: %s", 
         ch, found != -1 ? "found" : "not found",
         foundLinear != -1 ? "found" : "not found");
}

// Test sort
static void TestSort(vector *v)
{
  VectorSort(v, CompareChar);
  printf("\nSorted: ");
  VectorMap(v, PrintChar, stdout);
  
  TestSearch(v, 'J');
  TestSearch(v, '$');
}

// Test VectorNth
static void TestNth(vector *v)
{
  for (int i = 0; i < VectorLength(v); i += 2) {
    char *elem = (char *)VectorNth(v, i);
    *elem = tolower(*elem);
  }
  
  printf("\nLowercase every other: ");
  VectorMap(v, PrintChar, stdout);
}

// Test insert and delete
static void TestInsertDelete(vector *v)
{
  char dash = '-';
  
  // Insert dashes
  for (int i = 3; i < VectorLength(v); i += 4)
    VectorInsert(v, &dash, i);
  
  printf("\nWith dashes: ");
  VectorMap(v, PrintChar, stdout);
  
  // Delete dashes
  for (int i = 3; i < VectorLength(v); i += 3)
    VectorDelete(v, i);
  
  printf("\nDashes removed: ");
  VectorMap(v, PrintChar, stdout);
}

// Test replace
static void TestReplace(vector *v)
{
  int pos = 0;
  char find = 's', replace = '*';
  
  while (pos < VectorLength(v)) {
    pos = VectorSearch(v, &find, CompareChar, pos, false);
    if (pos == -1) break;
    VectorReplace(v, &replace, pos);
  }
  
  printf("\nReplaced '%c' with '%c': ", find, replace);
  VectorMap(v, PrintChar, stdout);
}

// Basic test with chars
void SimpleTest()
{
  printf("--- Basic char test ---\n");
  vector alphabet;
  VectorNew(&alphabet, sizeof(char), NULL, 4);
  
  TestAppend(&alphabet);
  TestSort(&alphabet);
  TestNth(&alphabet);
  TestInsertDelete(&alphabet);
  TestReplace(&alphabet);
  
  VectorDispose(&alphabet);
}

// Compare longs
static int CompareLong(const void *a, const void *b)
{
  return (*(long *)a - *(long *)b);
}

// Bigger test with numbers
void BiggerTest()
{
  printf("\n\n--- Testing with lots of numbers ---\n");
  vector numbers;
  VectorNew(&numbers, sizeof(long), NULL, 4);
  
  // Add a bunch of numbers
  printf("Adding numbers...\n");
  for (long i = 0; i < 10000; i++) {
    long val = (i * 12345) % 10000;
    VectorAppend(&numbers, &val);
  }
  
  printf("Sorting...\n");
  VectorSort(&numbers, CompareLong);
  
  printf("Deleting...\n");
  while (VectorLength(&numbers) > 0)
    VectorDelete(&numbers, 0);
  
  printf("Done!\n");
  VectorDispose(&numbers);
}

// Free strings
static void FreeString(void *elem)
{
  free(*(char **)elem);
}

// Print strings
static void PrintString(void *elem, void *fp)
{
  printf("\t%s\n", *(char **)elem);
}

// Test with strings (dynamic memory)
void StringTest()
{
  printf("\n\n--- Testing with strings ---\n");
  vector words;
  VectorNew(&words, sizeof(char *), FreeString, 5);
  
  const char *wordList[] = {"hello", "world", "test", "vector"};
  
  for (int i = 0; i < 4; i++) {
    char *word = malloc(strlen(wordList[i]) + 1);
    strcpy(word, wordList[i]);
    VectorAppend(&words, &word);
  }
  
  printf("Words in vector:\n");
  VectorMap(&words, PrintString, stdout);
  
  VectorDispose(&words);
}

int main() 
{
  SimpleTest();
  BiggerTest();
  StringTest();
  return 0;
}