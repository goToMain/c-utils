#include <stdlib.h>

/*
 * cut_string:
 *   splices the src string with delim and places the result in dest
 *   string and return the pointer to the next index of src.
 *
 * Usage example:
 *   char dest[32];
 *   char *src = "hello,world,test,prog";
 *   char *p = cut_string(src, dest, 32, ',', 1);
 *   dest == "world"
 *   p == ",test,prog"
 *
 * WARNING! Null returned if we go past the end
 */
char *cut_string(const char *src, char *dest, int dest_maxlen, char delim, int offset)
{
#define SAFESET_target(i,val) do { if (i < dest_maxlen) dest[i]=val; } while (0)
    int i=0, j=0;

    SAFESET_target(0,0);
    while (1) {
        // treat all consecutive delimiters chars as one unit.
        while (src[i] == delim)
            i++;
        if (offset == 0)
            break;
        while (src[i] && (src[i] != delim))
            i++;
        offset--;

        // If we hit eol before offset was zero return NULL
        if (src[i] == 0)
            return NULL;
    }

    // Copy string into dest buffer.
    while (src[i] && (src[i] != delim)) {
        SAFESET_target(j,src[i]);
        i++; j++;
    }
    SAFESET_target(j,0);

    return (char *)src;
}
