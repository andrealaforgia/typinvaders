#include "strdup.h"

#include <stdlib.h>
#include <string.h>

char *strdup(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    size_t len = strlen(s) + 1;  // +1 for the null terminator
    char *copy = malloc(len);

    if (copy == NULL) {
        return NULL;  // Return NULL if memory allocation fails
    }

    memcpy(copy, s, len);
    return copy;
}
