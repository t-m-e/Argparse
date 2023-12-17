/*
 * Argparse test.
 * Contains:
 * * Argparse_exists test
 * * Argparse_get test
 * * Argparse_getArg test
 */

#include "../include/argparse.h"

#include <stdio.h>

int main(int argc, char** argv) {
    struct Argparse argparse = Argparse_new(
        "-a -b: -c::",
        argv, 
        argc
    );

    if (Argparse_exists(&argparse, "-a")) {
        printf("[+] '-a' flag exists\n");
    } else {
        printf("[-] '-a' not found\n");
    }

    if (Argparse_exists(&argparse, "-b")) {
        printf("[+] '-b' flag exists\n");
        printf("[*] '-b' flag argument: %s\n", Argparse_get(&argparse, "-b"));
    } else {
        printf("[-] '-b' not found\n");
    }

    if (Argparse_exists(&argparse, "-c")) {
        printf("[+] '-c' flag exists\n");
        printf("[*] argument[0]: %s\n", Argparse_getArg(&argparse, "-c", 0));
        printf("[*] argument[1]: %s\n", Argparse_getArg(&argparse, "-c", 1));
    }

    /* free contents of the map */
    Argparse_free(&argparse);
    return 0;
}
