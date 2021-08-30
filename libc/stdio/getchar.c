#include <stdint.h>
#include <stdio.h>

#if defined(__is_libk)
#include <kernel/keyboard.h>
#endif

int getchar(void) {
    uint8_t ch = keyboard_getchar();
    if (ch == '\n') {
        printf("\r\n");
    } else if (ch == '\b') {
        // Backspace is handled in tty.
        printf("%c", ch);
    } else {
        printf("%c", ch);
    }
    return ch;
}
