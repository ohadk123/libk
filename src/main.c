#include "StringBuilder.h"
#include <stdio.h>

int printString(String *s) { return printf("%.*s\n", (int)s->len, s->data); }

int main(void) {
    StringBuilder sb = {0};
    ErrCode err;
    if ((err = SB_joinEntireFile("./src/main.c", &sb)) != NO_ERROR) {
        printf("error! %d\n", err);
        return -1;
    }
    String s = SB_moveToString(&sb);
    printString(&s);
    return 0;
}
