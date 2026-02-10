#include "String.h"
#include <assert.h>
#include <stdio.h>

int printString(String *s) { return printf("%.*s\n", (int)s->len, s->arr); }

int main(void) {
    String s = {0};
    joinCString(&s, "Hello World!");
    printString(&s);
}
