#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++) {
		if (putchar(bytes[i]) == EOF) {
			return false;
		}
	}
	return true;
}

// TODO: You can only pad out to 9 characters.
int printf(const char* restrict format, ...) {
	const int ITOA_SIZE = 16;
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%') {
				amount++;
			}
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount)) {
				return -1;
			}
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char)va_arg(parameters, int); // char promotes to int
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if ((*format == 'd') ||
				   ((*(format + 1) == 'd') && isdigit(*format)) ||
				   ((*(format + 2) == 'd') && isdigit(*(format + 1)))) {
			char padding = ' ';
			char width = 0;

			if (*(format + 2) == 'd') {
				padding = *format;
				format++;
				width = (*format) - '0';
				format++;
			} else if (*(format + 1) == 'd') {
				width = (*format) - '0';
				format++;
			}

			format++; // skip the 'd'
			int n = (int)va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			char text[ITOA_SIZE];
			itoa(n, text, 10);
			int len = strlen(text);

			if (width - len > 0) {
				int remainder = width - len;
				for (int n = 0; n < remainder; n++) {
					putchar(padding);
				}
			}

			if (!print(text, len)) {
				return -1;
			}

			if (len > width) {
				written += len;
			} else {
				written += width;
			}
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if ((*format == 'x') ||
				   ((*(format + 1) == 'x') && isdigit(*format)) ||
				   ((*(format + 2) == 'x') && isdigit(*(format + 1)))) {
			char padding = ' ';
			char width = 0;

			if (*(format + 2) == 'x') {
				padding = *format;
				format++;
				width = (*format) - '0';
				format++;
			} else if (*(format + 1) == 'x') {
				width = (*format) - '0';
				format++;
			}

			format++; // skip the 'd'
			int n = (int)va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			char text[ITOA_SIZE];
			itoa(n, text, 16);
			strlwr(text);
			int len = strlen(text);

			if (width - len > 0) {
				int remainder = width - len;
				for (int n = 0; n < remainder; n++) {
					putchar(padding);
				}
			}

			if (!print(text, len)) {
				return -1;
			}

			if (len > width) {
				written += len;
			} else {
				written += width;
			}
		} else if ((*format == 'X') ||
				   ((*(format + 1) == 'X') && isdigit(*format)) ||
				   ((*(format + 2) == 'X') && isdigit(*(format + 1)))) {
			char padding = ' ';
			char width = 0;

			if (*(format + 2) == 'X') {
				padding = *format;
				format++;
				width = (*format) - '0';
				format++;
			} else if (*(format + 1) == 'X') {
				width = (*format) - '0';
				format++;
			}

			format++; // skip the 'd'
			int n = (int)va_arg(parameters, int);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			char text[ITOA_SIZE];
			itoa(n, text, 16);
			int len = strlen(text);

			if (width - len > 0) {
				int remainder = width - len;
				for (int n = 0; n < remainder; n++) {
					putchar(padding);
				}
			}

			if (!print(text, len)) {
				return -1;
			}

			if (len > width) {
				written += len;
			} else {
				written += width;
			}
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len)) {
				return -1;
			}
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
