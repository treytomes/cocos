#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

#include <kernel/keyboard.h>
#include <kernel/irq.h>
#include <kernel/system.h>
#include <kernel/timer.h>
#include <kernel/tty.h>

uint8_t keyboard_layout_us[2][128] = {
    {
        KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '-', '=', KEY_BACKSPACE, KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u',
        'i', 'o', 'p', '[', ']', KEY_ENTER, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j',
        'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
        ',', '.', '/', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
        KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }, {
        KEY_NULL, KEY_ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
        '_', '+', KEY_BACKSPACE, KEY_TAB, 'Q', 'W',   'E', 'R', 'T', 'Y', 'U',
        'I', 'O', 'P',   '{', '}', KEY_ENTER, 0, 'A', 'S', 'D', 'F', 'G', 'H',
        'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
        'M', '<', '>', '?', 0, 0, 0, ' ', 0, KEY_F1, KEY_F2, KEY_F3, KEY_F4,
        KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, 0, 0, KEY_HOME, KEY_UP,
        KEY_PAGE_UP, '-', KEY_LEFT, '5', KEY_RIGHT, '+', KEY_END, KEY_DOWN,
        KEY_PAGE_DOWN, KEY_INSERT, KEY_DELETE, 0, 0, 0, KEY_F11, KEY_F12
    }
};

struct Keyboard keyboard;

// bad hack! for a better RNG
static bool seeded = false;

uint8_t cursor_color = 0;

void draw_cursor() {
    terminal_putentryat(219, cursor_color, terminal_get_cursor_column(), terminal_get_cursor_row());
    cursor_color++;
    if (cursor_color > 15) {
        cursor_color = 0;
    }
}

void erase_cursor() {
    uint8_t color = terminal_get_cursor_color();
    terminal_putentryat(' ', color, terminal_get_cursor_column(), terminal_get_cursor_row());
}

uint8_t keyboard_getchar() {
    while (keyboard.next_char == 0) {
        draw_cursor();
    }
    erase_cursor();
    uint8_t ch = keyboard.next_char;
    keyboard.next_char = 0;
    return ch;
}

uint8_t keyboard_read() {
    uint16_t scancode = (uint16_t) inportb(0x60);
    uint8_t rawScancode = KEY_SCANCODE(scancode);
    bool isPressed = KEY_IS_PRESS(scancode);

    if (!seeded) {
        seed(((uint32_t)scancode) * 17 + timer_get());
        seeded = true;
    }

    if (rawScancode == KEY_LALT || rawScancode == KEY_RALT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_ALT), isPressed);
    } else if (rawScancode == KEY_LCTRL || rawScancode == KEY_RCTRL) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CTRL), isPressed);
    } else if (rawScancode == KEY_LSHIFT || rawScancode == KEY_RSHIFT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SHIFT), isPressed);
    } else if (rawScancode == KEY_CAPS_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CAPS_LOCK), isPressed);
    } else if (rawScancode == KEY_NUM_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_NUM_LOCK), isPressed);
    } else if (rawScancode == KEY_SCROLL_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SCROLL_LOCK), isPressed);
    }

    if (isPressed) {
        if (rawScancode == KEY_CAPS_LOCK) {
            keyboard.capsLock = !keyboard.capsLock;
        } else if (rawScancode == KEY_SCROLL_LOCK) {
            keyboard.scrollLock = !keyboard.scrollLock;
        } else if (rawScancode == KEY_NUM_LOCK) {
            keyboard.numLock = !keyboard.numLock;
        }
    }

    uint8_t next_scancode = rawScancode;
    uint8_t next_char = KEY_CHAR(scancode);

    keyboard.keys[next_scancode] = isPressed;
    keyboard.chars[next_char] = isPressed;

    if (isPressed && (next_char != 0)) {
        keyboard.next_char = next_char;
    }

    return next_scancode;
}

static void keyboard_handler(struct Registers *regs) {
    keyboard_read();
}

void keyboard_init() {
    keyboard.capsLock = false;
    keyboard.scrollLock = false;
    keyboard.numLock = false;
    irq_install(1, keyboard_handler);
}
