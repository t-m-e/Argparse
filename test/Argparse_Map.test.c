/*
 * Argparse_Map test.
 * Contains:
 * * Argparse_Map_hashFunc
 * * Argparse_Map_new
 * * Argparse_Map_append
 * * Argparse_Map_exists
 * * Argparse_Map_get
 * * Argparse_Map_getArg
 * * Argparse_Map_free
 */

#include "../include/argparse.h"

#include <stdio.h>
#include <stddef.h>

static int argparse_test_passed_count = 0;
static int argparse_test_total_count = 0;

#define TEST(x) \
    printf("\n[!] Test %s:\n", #x)

#define TEST_ASSERT(x) \
    argparse_test_total_count++; \
    if (!(x)) {\
        printf("[-] %s ...failed!\n", #x); \
    } else { \
        argparse_test_passed_count++; \
        printf("[+] %s ...passed!\n", #x); \
    }

int main(int argc, char** argv) {
    /* Argparse_Map_hashFunc test */
    TEST(Argparse_Map_hashFunc);
    TEST_ASSERT(Argparse_Map_hashFunc("-a") == ((('-') | ('a' << 8)) % ARGPARSE_BUCKET_COUNT));

    /* Argparse_Map_new test */
    TEST(Argparse_Map_new);
    struct Argparse_Map map = Argparse_Map_new();
    for (int i = 0; i < ARGPARSE_BUCKET_COUNT; i++) {
        TEST_ASSERT(map.buckets[i] == NULL);
    }
    TEST_ASSERT(map.size == ARGPARSE_BUCKET_COUNT);
    TEST_ASSERT(map.count == 0);

    /* Argparse_Map_append test */
    TEST(Argparse_Map_Append);
    TEST_ASSERT(Argparse_Map_append(&map, "-a", NULL) == 1);
    TEST_ASSERT(Argparse_Map_append(&map, "-b", "arg_for_b") == 1);
    TEST_ASSERT(Argparse_Map_append(&map, "-c", "arg1_for_c") == 1);
    TEST_ASSERT(Argparse_Map_append(&map, "-c", "arg2_for_c") == 1);

    /* Argparse_Map_exists test */
    TEST(Argparse_Map_exists);
    TEST_ASSERT(Argparse_Map_exists(&map, "-a") == 1);
    TEST_ASSERT(Argparse_Map_exists(&map, "-d") == 0);

    /* Argparse_Map_get test */
    TEST(Argparse_Map_get);
    TEST_ASSERT(Argparse_Map_get(&map, "-b") != NULL);
    printf("[*] ... %s\n", Argparse_Map_get(&map, "-b"));
    TEST_ASSERT(Argparse_Map_get(&map, "-c") != NULL);
    printf("[*] ... %s\n", Argparse_Map_get(&map, "-c"));

    /* Argparse_Map_getArg test */
    TEST(Argparse_Map_getArg);
    TEST_ASSERT(Argparse_Map_getArg(&map, "-c", 0) != NULL);
    printf("[*] ... %s\n", Argparse_Map_getArg(&map, "-c", 0));
    TEST_ASSERT(Argparse_Map_getArg(&map, "-c", 1) != NULL);
    printf("[*] ... %s\n", Argparse_Map_getArg(&map, "-c", 1));
    TEST_ASSERT(Argparse_Map_getArg(&map, "-b", 1) == NULL);
    printf("[*] ... %s\n", Argparse_Map_getArg(&map, "-b", 1));

    /* Argparse_Map_free test */
    TEST(Argparse_Map_free);
    Argparse_Map_free(&map);
    TEST_ASSERT(map.size == 0);
    TEST_ASSERT(map.count == 0);
    for (int i = 0; i < ARGPARSE_BUCKET_COUNT; i++) {
        TEST_ASSERT(map.buckets[i] == NULL);
        printf("[*] ... %p\n", (void*)map.buckets[i]);
    }

    printf("\n\n");
    printf("TESTS PASSED: %d/%d\n", argparse_test_passed_count, argparse_test_total_count);

    return 0;
}
