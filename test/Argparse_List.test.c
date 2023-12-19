/*
 * Argparse_List test.
 * Contains:
 * * Argparse_List_append test
 * * FOREACH_ARGPARSE_PARAM test
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
    struct Argparse_List* list = NULL;

    /* Argparse_List_append test */
    TEST(Argparse_List_append);
    TEST_ASSERT((Argparse_List_append(&list, "something")) == 1);
    TEST_ASSERT((Argparse_List_append(&list, "else")) == 1);
    TEST_ASSERT((Argparse_List_append(&list, "entirely")) == 1);
    
    /* FOREACH_ARGPARSE_PARAM test */
    TEST(FOREACH_ARGPARSE_PARAM);
    struct Argparse argparse = (struct Argparse) {
        .map = Argparse_Map_new(),
        .list = list
    };

    FOREACH_ARGPARSE_PARAM(item, argparse) {
        TEST_ASSERT(item != NULL);
        printf("[*] ... %s\n", item);
    }

    printf("\n\n");
    printf("TESTS PASSED: %d/%d\n", argparse_test_passed_count, argparse_test_total_count);
    return 0;
}
