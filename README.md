# Argparse

## Purpose

Argparse is used to parse arguments! Easy enough right? 
Argparse is a header-only implementation made to be easily integrated into any project. Both declaration and implementation exist inside of the `include/argparse.h` file. 

## Examples

```c
/* $ ./argparse -a -b argb -c argc1 argc2 */

#include "include/argparse.h"

int main(
    int argc, 
    char** argv
) {
    /* create argparse rules */
    /* ':' means an argument to a flag should be collected */
    /* spaces delimit identifiers, meaning you can have flags longer than 1 char long */
    /* note: flags are literal, so if you want '-' before it you have to make sure you put it in the rule */
    /* create argparse argument map */ 
    struct Argparse argparse = Argparse_new("-a -b: -c::", argv, argc);

    /* use your args */
    if (Argparse_exists(&argparse, "-a")) {
        // do something
    }

    char* option  = Argparse_get(&argparse, "-b");
    char* option1 = Argparse_getArg(&argparse, "-c", 0);
    char* option2 = Argparse_getArg(&argparse, "-c", 1);
}
```
