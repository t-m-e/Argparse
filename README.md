# Argparse

## Purpose

Argparse is used to parse arguments! Easy enough right? 
Argparse is a header-only implementation made to be easily integrated into any project. Both declaration and implementation exist inside of the `include/argparse.h` file. 

## Rule Strings

Rule strings are strings that define names of arguments and how many arguments they consume. Rule strings take the following format: `:` consumes an argument, whitespace delimits different argument names, and anything else is parsed as an argument string.
Example:
- `-a -b: -c::`
- `alpha beta: gamma`

Good thing to note, the Argparse parsing takes argument names literally, so if your argument is input as `-a` then the parser will interpret it as it is input.

## Usage

Use with argc/argv:
```c
/* $ ./argparse -a -b argb -c argc1 argc2 */

#include "include/argparse.h"

int main(
    int argc, 
    char** argv
) {
    struct Argparse argparse = Argparse_new("-a -b: -c::", ++argv, --argc);

    /* use your args */
    if (Argparse_exists(&argparse, "-a")) {
        // do something
    }

    char* option  = Argparse_get(&argparse, "-b");
    char* option1 = Argparse_getArg(&argparse, "-c", 0);
    char* option2 = Argparse_getArg(&argparse, "-c", 1);
}
```

Use with self defined array:
```c
/* $ ./argparse */

#include "include/argparse.h"
#include <stdio.h>

int main() {
    char* argv[] = {
        "-a", "-b", "some", "other", "thing"
    };
    int argc = 5;

    struct Argparse argparse = Argparse_new("-a -b", argv, argc);

    if (Argparse_exists(&argparse, "-a")) { /* ... */ }
    if (Argparse_exists(&argparse, "-b")) { /* ... */ }
    
    /* macro defined in argparse.h
     * item is a user defined name for the parameter value (a char*) inside of the loop scope
     * argparse is the struct Argparse variable defined above
     */
    FOREACH_ARGPARSE_PARAM(item, argparse) {
        printf("%s", item);
    }
}
```
