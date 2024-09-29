#include "miscellaneous.h"

size_t xstrlen(char const * const str)
{
    size_t len = 0;

    while('\0' != *(str + len)) {
        len++;
    }

  return len;
}