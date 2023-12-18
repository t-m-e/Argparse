/* 
 * Implementation for Argparse.
 * Contains:
 * * Argparse_Bucket structure
 * * Argparse_Map structure
 * * Argparse_RuleList structure
 * * Argparse strcture
 */

#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define ARGPAR_MAP  static
#define ARGPAR_IMPL static

/* ===== Hashmap Implementation ===== */

/* Update if more arguments needed */
#define ARGPARSE_BUCKET_COUNT 20 

/*
 * Buckets for Map. 
 * Contains:
 * * key => key value to create hash entry
 * * value => string being stored
 * * nonce => number added to original hash value to get current entry
 * * next => pointer to the next argument for the current key
 */
struct Argparse_Bucket {
    char* key;
    char* value;
    size_t nonce;
    struct Argparse_Bucket* next;
}; 

/*
 * Map to contain buckets.
 * Contains:
 * * buckets => array of ARGPARSE_BUCKET_COUNT buckets
 * * size => total number of buckets
 * * count => current number of buckets allocated
 */
struct Argparse_Map {
    struct Argparse_Bucket* buckets[ARGPARSE_BUCKET_COUNT];
    size_t size;
    size_t count;
};

/* static definitions */

/*
 * Hash function.
 * Generates hashes from keys.
 */
ARGPAR_MAP size_t Argparse_Map_hashFunc(
    char* key
) {
    size_t hash = 0;
    size_t shift = 0;

    for (char* c = key; *c != '\0'; c++) {
        if (shift == sizeof(size_t) * 8) shift = 0;
        hash |= (size_t)(*c) << shift;
        shift += sizeof(char) * 8;
    }

    return hash % ARGPARSE_BUCKET_COUNT;
}

/*
 * Initialize a new Map.
 * Defaults values
 */
ARGPAR_MAP struct Argparse_Map Argparse_Map_new() {
    return (struct Argparse_Map) {
        .buckets = { 0 },
        .size = ARGPARSE_BUCKET_COUNT,
        .count = 0
    };
}

/*
 * Append key, value pair to Map.
 * If key doesn't exist, create a new bucket allocation.
 * If key does exist, append new bucket to existing bucket allocation.
 */
ARGPAR_MAP int Argparse_Map_append(
    struct Argparse_Map* map,
    char* key,
    char* value
) {
    if (map->count == map->size) return 0; 

    size_t hash = Argparse_Map_hashFunc(key);
    size_t nonce = 0;
    while (map->buckets[hash] != NULL && 
           nonce != ARGPARSE_BUCKET_COUNT
    ) {
        if (map->buckets[hash] != NULL &&
            map->buckets[hash]->nonce == nonce &&
            strcmp(map->buckets[hash]->key, key) == 0
        ) {
            break;
        }

        nonce++;
        hash = (hash + 1) % map->size;
    }

    if (nonce == ARGPARSE_BUCKET_COUNT) {
        return 0;
    }

    if (map->buckets[hash] != NULL) {
        struct Argparse_Bucket* ptr = map->buckets[hash];
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }

        ptr->next = malloc(sizeof(struct Argparse_Bucket));
        if (ptr->next == NULL) {
            return 0;
        }

        ptr->next->key = calloc(1, strlen(key));
        if (ptr->next->key == NULL) {
            return 0;
        }

        ptr->next->value = calloc(1, strlen(value));
        if (ptr->next->value == NULL) {
            return 0;
        }

        memcpy(ptr->next->key, key, strlen(key));
        memcpy(ptr->next->value, value, strlen(value));
        ptr->next->nonce = nonce;
        ptr->next->next = NULL;

        return 1;
    }

    map->buckets[hash] = malloc(sizeof(struct Argparse_Bucket));
    if (map->buckets[hash] == NULL) {
        return 0;
    }

    map->buckets[hash]->key = calloc(1, strlen(key) + 1);
    if (map->buckets[hash]->key == NULL) {
        return 0;
    }
    memcpy(map->buckets[hash]->key, key, strlen(key));

    if (value != NULL) {
        map->buckets[hash]->value = calloc(1, strlen(value) + 1);
        if (map->buckets[hash]->value == NULL) {
            return 0;
        }
        memcpy(map->buckets[hash]->value, value, strlen(value));
    }

    map->buckets[hash]->nonce = nonce;
    map->buckets[hash]->next = NULL;
    map->count += 1;

    return 1;
}

/* 
 * Check is a key exists in Map.
 * If 0, then key doesn't exist.
 * If 1, then key exists.
 */
ARGPAR_MAP int Argparse_Map_exists(
    struct Argparse_Map* map,
    char* key
) {
    if (map->count == 0) return 0;

    size_t hash = Argparse_Map_hashFunc(key);
    size_t nonce = 0;
    while (map->buckets[hash] != NULL && 
           strcmp(map->buckets[hash]->key, key) != 0 && 
           nonce != ARGPARSE_BUCKET_COUNT
    ) {
        nonce++;
        hash = (hash + 1) % map->size;
    }

    if (nonce == ARGPARSE_BUCKET_COUNT) {
        return 0;
    }

    if (map->buckets[hash] == NULL) {
        return 0;
    }

    return 1;
}

/*
 * Get value from Map.
 * If bucket is allocated, return the value in the bucket.
 * If bucket isn't allocated, return NULL.
 */
ARGPAR_MAP char* Argparse_Map_get(
    struct Argparse_Map* map,
    char* key
) {
    if (map->count == 0) return NULL;

    size_t hash = Argparse_Map_hashFunc(key);
    size_t nonce = 0;
    while (map->buckets[hash] != NULL && 
           strcmp(map->buckets[hash]->key, key) != 0 && 
           nonce != ARGPARSE_BUCKET_COUNT
    ) {
        nonce++;
        hash = (hash + 1) % map->size;
    }

    if (nonce == ARGPARSE_BUCKET_COUNT) {
        return NULL;
    }

    if (map->buckets[hash] == NULL) {
        return NULL;
    }

    return map->buckets[hash]->value;
}

/*
 * Get specified value from Map.
 * If bucket is allocated, return the value in the bucket of the specified offset.
 * If bucket isn't allocated, return NULL.
 */
ARGPAR_MAP char* Argparse_Map_getArg(
    struct Argparse_Map* map,
    char* key,
    size_t argIndex
) {
    if (map->count == 0) return NULL;

    size_t hash = Argparse_Map_hashFunc(key);
    size_t nonce = 0;
    while (map->buckets[hash] != NULL &&
           strcmp(map->buckets[hash]->key, key) != 0 &&
           nonce != ARGPARSE_BUCKET_COUNT
    ) {
        nonce++;
        hash = (hash + 1) % map->size;
    }

    if (nonce == ARGPARSE_BUCKET_COUNT) {
        return NULL;
    }

    struct Argparse_Bucket* bucket = map->buckets[hash];
    for (size_t i = 0; i < argIndex; i++) {
        if (bucket == NULL) {
            return NULL;
        }

        bucket = bucket->next;
    }

    if (bucket == NULL) {
        return NULL;
    }

    return bucket->value;
}

#include <stdio.h>

/*
 * Free buckets from Map.
 */
ARGPAR_MAP void Argparse_Map_free(
    struct Argparse_Map* map
) {
    for (size_t i = 0; i < map->size; i++) {
        if (map->buckets[i] != NULL) {
            struct Argparse_Bucket* ptr = map->buckets[i];
            while (ptr != NULL) {
                struct Argparse_Bucket* tmp = ptr;
                ptr = ptr->next;
                free(tmp);
            }
            map->buckets[i] = NULL;
        }
    }

    map->size = 0;
    map->count = 0;
}

/* ===== Hashmap Implementation End ===== */

/* structure definitions */

/*
 * Argparse structure.
 * Contains:
 * * map => Map of key, value pairs
 */
struct Argparse {
    struct Argparse_Map map;
};

/* 
 * RuleList Structure.
 * Contains:
 * * flag => key value of the Rule
 * * argCount => number of arguments associated with the flag
 * * next => pointer to next Rule in the list
 */
struct Argparse_RuleList {
    char* flag;
    size_t argCount;
    struct Argparse_RuleList* next;
};

/* static definitions */

/*
 * Initialize new Argparse structure.
 * Parse the rule into a RuleList.
 * Use RuleList to parse args into Map.
 * If error, return { 0 }.
 * If success, return Argparse structure with key, value pairs input.
 */
ARGPAR_IMPL struct Argparse Argparse_new(
    char* rule,
    char** args,
    int argCount
) {
    struct Argparse argparse = {
        .map = Argparse_Map_new()
    };
    size_t ruleIndex = 0;
    size_t argIndex = 1;

    /* create rule list */
    struct Argparse_RuleList* rules = NULL;
    struct Argparse_RuleList* currentRulePtr = rules; 
    
    while (rule[ruleIndex]) {
        /* whitespace */
        if (rule[ruleIndex] == ' ' ||
            rule[ruleIndex] == '\t' ||
            rule[ruleIndex] == '\n'
        ) {
            ruleIndex++;
        }

        /* argument */
        else if (rule[ruleIndex] == ':') {
            if (currentRulePtr == NULL) {
                return (struct Argparse) { 0 };
            }

            currentRulePtr->argCount += 1;
            ruleIndex++;
        }

        /* flag */
        else {
            char flag[100] = { 0 };
            size_t i = 0;
            while (rule[ruleIndex] != ' ' &&
                rule[ruleIndex] != '\t' &&
                rule[ruleIndex] != '\n' && 
                rule[ruleIndex] != ':'
            ) {
                flag[i++] = rule[ruleIndex++];
            }

            if (rules == NULL) {
                rules = malloc(sizeof(struct Argparse_RuleList));
                if (rules == NULL) {
                    return (struct Argparse) { 0 };
                }

                rules->flag = malloc(i + 1);
                if (rules->flag == NULL) {
                    return (struct Argparse) { 0 };
                }
                rules->flag[i] = 0;
                memcpy(rules->flag, flag, i);
                rules->argCount = 0;
                rules->next = NULL;

                currentRulePtr = rules;
            } else {
                struct Argparse_RuleList* ptr = rules;
                while (ptr->next != NULL) {
                    ptr = ptr->next;
                }

                ptr->next = malloc(i + 1);
                if (ptr->next == NULL) {
                    return (struct Argparse) { 0 };
                }

                ptr->next->flag = malloc(i + 1);
                if (ptr->next->flag == NULL) {
                    return (struct Argparse) { 0 };
                }
                memcpy(ptr->next->flag, flag, i);
                ptr->next->argCount = 0;
                ptr->next->next = NULL;

                currentRulePtr = ptr->next; 
            }    
        }
    }

    /* parse args using list entries */
    while (argIndex < argCount) {
        struct Argparse_RuleList* ptr = rules;
        int found = 0;
        while (ptr != NULL && argIndex < argCount) {
            found = 0;
            if (strcmp(args[argIndex], ptr->flag) == 0) {
                /* match made */ 
                found = 1;
                argIndex++;

                if (ptr->argCount > 0) {
                    if (argCount < argIndex + ptr->argCount) {
                        return (struct Argparse) { 0 };
                    }

                    for (size_t i = 0; i < ptr->argCount; i++) {
                        Argparse_Map_append(
                            &(argparse.map),
                            ptr->flag,
                            args[argIndex++]
                        );
                    }
                } else {
                    Argparse_Map_append(
                        &(argparse.map),
                        ptr->flag,
                        NULL
                    );
                }
            }

            ptr = ptr->next;
        } 

        if (found == 0) {
            return (struct Argparse) { 0 };
        }
    }
     

    return argparse;
}

/*
 * Check if a key exists in Map.
 * If key exists in Map, return 1.
 * If key doesn't exist in Map, return 0.
 */
ARGPAR_IMPL int Argparse_exists(
    struct Argparse* argparse,
    char* key
) {
    return Argparse_Map_exists(&argparse->map, key);
}

/* 
 * Get value from key in Map.
 * If key doesn't exist in Map, return NULL.
 * If key exists in Map, return value.
 */
ARGPAR_IMPL char* Argparse_get(
    struct Argparse* argparse,
    char* key
) {
    return Argparse_Map_get(&argparse->map, key);
}

/*
 * Get value at index offset in Map.
 * If key doesn't exist in Map, return NULL.
 * If key exists in Map, return value at index offset (if exists).
 */
ARGPAR_IMPL char* Argparse_getArg(
    struct Argparse* argparse,
    char* key,
    size_t index
) {
    return Argparse_Map_getArg(&argparse->map, key, index);
}

/* 
 * Free Map in Argparse.
 */
ARGPAR_IMPL void Argparse_free(
    struct Argparse* argparse
) {
    Argparse_Map_free(&argparse->map);
}

#endif
