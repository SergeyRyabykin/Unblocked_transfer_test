#include "miscellaneous.h"

#define MB 1000000

size_t xstrlen(char const * const str)
{
    size_t len = 0;

    while('\0' != *(str + len) && len < MB) {
        len++;
    }

  return len;
}