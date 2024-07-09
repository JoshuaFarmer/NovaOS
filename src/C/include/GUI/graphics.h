#pragma once
#include "typedef.h"
#include "Kernel/malloc.h"

void fillRect(rect_t* rect, uint8_t color) {
	uint8_t* vga = current_buffer;
	for (uint32_t y = rect->y; y < (rect->y + rect->h); y++) {
		for (uint32_t x = rect->x; x < (rect->x + rect->w); x++) {
			vga[y * GVGA_WIDTH + x] = color;
		}
	}
}

void drawLine(int x0, int y0, int x1, int y1, uint8_t color) {
	uint8_t* vga = current_buffer;
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;
	int e2;

	while (1) {
		if (x0 >= 0 && x0 < GVGA_WIDTH && y0 >= 0 && y0 < GVGA_HEIGHT) {
			vga[y0 * GVGA_WIDTH + x0] = color;
		}
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void drawRectWOutline(rect_t* rect, uint8_t fillColor, uint8_t outlineColor) {
	fillRect(rect, fillColor);
	drawLine(rect->x, rect->y, rect->x + rect->w - 1, rect->y, outlineColor);
	drawLine(rect->x, rect->y + rect->h - 1, rect->x + rect->w - 1, rect->y + rect->h - 1, outlineColor);
	drawLine(rect->x, rect->y, rect->x, rect->y + rect->h - 1, outlineColor);
	drawLine(rect->x + rect->w - 1, rect->y, rect->x + rect->w - 1, rect->y + rect->h - 1, outlineColor);
}