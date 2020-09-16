#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"


static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

void terminal_clear() {

	uint16_t space = vga_entry(' ', terminal_color);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = space;
		}
	}
	terminal_row = 0;
	terminal_column = 0;
}

void terminal_initialize()
{

	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	uint16_t space = vga_entry(' ', terminal_color);

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = space;
		}
	}

	terminal_clear();
}

void terminal_setcolor(uint8_t color)
{

	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{

	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll_up(size_t n) {

	if (n <= 0)
		return;
	if (n > VGA_HEIGHT) {
		terminal_clear();
		terminal_row = VGA_HEIGHT - 1;
		return;
	}

	for (size_t line = 0; line < VGA_HEIGHT - n - 1; ++line) {

		uint16_t* source = &terminal_buffer[(n + line) * VGA_WIDTH];
		uint16_t* dest = &terminal_buffer[line * VGA_WIDTH];

		for (size_t col = 0; col < VGA_WIDTH; ++col) {
			dest[col] = source[col];
		}
	}

	uint16_t space = vga_entry(' ', terminal_color);
	for (size_t line = VGA_HEIGHT - n; line < VGA_HEIGHT; ++line) {
		uint16_t* dest = &terminal_buffer[line * VGA_WIDTH];
		for (size_t col = 0; col < VGA_WIDTH; ++col) {
			dest[col] = space;
		}
	}
}

void terminal_putchar(char c)
{
	if (c == '\n') {
		terminal_column = 0;
		++terminal_row;
	}
	else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			++terminal_row;
		}
	}

	if (terminal_row >= VGA_HEIGHT) {
		terminal_scroll_up(terminal_row - VGA_HEIGHT + 1);
		terminal_row = VGA_HEIGHT - 1;
	}
}

void terminal_write(const char* data, size_t size)
{

	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data)
{

	terminal_write(data, strlen(data));
}

