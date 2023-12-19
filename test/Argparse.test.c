/*
 * Argparse test.
 * Contains:
 * * Argparse_new test
 * * Argparse_exists test
 * * Argparse_get test
 * * Argparse_getArg test
 * * Argparse_free test
 */

#include "../include/argparse.h"

#include <stdio.h>

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

int main() {
    int argc = 8;
    char* argv[] = {
        "-a", "-b", "1", "-c", "2", "3", "something", "else"
    };

    /* Argparse_new test */
    TEST(Argparse_new);
    printf("[*] ... ");
    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    struct Argparse argparse = Argparse_new(
        "-a -b: -c::",
        argv, 
        argc
    );
    TEST_ASSERT(argparse.map.size == ARGPARSE_BUCKET_COUNT);
    TEST_ASSERT(argparse.map.count != 0);
    printf("[*] ... %ld\n", argparse.map.count);

    /* Argparse_exists test */
    TEST(Argparse_exists);
    TEST_ASSERT(Argparse_exists(&argparse, "-a"));
    TEST_ASSERT(Argparse_exists(&argparse, "-b"));
    TEST_ASSERT(Argparse_exists(&argparse, "-c"));

    /* Argparse_get test */
    TEST(Argparse_get);
    TEST_ASSERT(Argparse_get(&argparse, "-b") != NULL);
    printf("[*] ... %s\n", Argparse_get(&argparse, "-b"));
    TEST_ASSERT(Argparse_get(&argparse, "-c") != NULL);
    printf("[*] ... %s\n", Argparse_get(&argparse, "-c"));
    TEST_ASSERT(Argparse_get(&argparse, "-d") == NULL);

    /* Argparse_getArg test */
    TEST(Argparse_getArg);
    TEST_ASSERT(Argparse_getArg(&argparse, "-b", 0) != NULL);
    printf("[*] ... %s\n", Argparse_getArg(&argparse, "-b", 0));
    TEST_ASSERT(Argparse_getArg(&argparse, "-b", 1) == NULL);
    TEST_ASSERT(Argparse_getArg(&argparse, "-c", 0) != NULL);
    printf("[*] ... %s\n", Argparse_getArg(&argparse, "-c", 0));
    TEST_ASSERT(Argparse_getArg(&argparse, "-c", 1) != NULL);
    printf("[*] ... %s\n", Argparse_getArg(&argparse, "-c", 1));

    /* Argparse_getParam test */
    TEST(Argparse_getParam);
    TEST_ASSERT(Argparse_getParam(&argparse, 0) != NULL);
    printf("[*] ... %s\n", Argparse_getParam(&argparse, 0));
    TEST_ASSERT(Argparse_getParam(&argparse, 1) != NULL);
    printf("[*] ... %s\n", Argparse_getParam(&argparse, 1));
    TEST_ASSERT(Argparse_getParam(&argparse, 2) == NULL);

    /* Argparse_free test */
    TEST(Argparse_free);
    Argparse_free(&argparse);
    TEST_ASSERT(argparse.map.size == 0);
    TEST_ASSERT(argparse.map.count == 0);
    for (int i = 0; i < ARGPARSE_BUCKET_COUNT; i++) {
        TEST_ASSERT(argparse.map.buckets[i] == NULL);
    }

    printf("\n\n");
    printf("TESTS PASSED: %d/%d\n", argparse_test_passed_count, argparse_test_total_count);

    return 0;
}
