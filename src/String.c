#include "String.h"
#include <string.h>

Bool compareStrings(String *a, String *b) {
    if (a->len != b->len) return FALSE;
    return strncmp((cstr) a->data, (cstr) b->data, a->len) == 0;
}

Bool compareCString(String *a, cstr b) {
    usize blen = strlen(b);
    if (a->len != blen) return FALSE;
    return strncmp((cstr) a->data, b, a->len) == 0;
}
