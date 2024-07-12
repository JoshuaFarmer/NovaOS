#pragma once
#include "GUI/GUIdef.h"
#include "Kernel/malloc.h"
#include "GUI/graphics.h"

element_t* create_text_element(window_t* win, wchar_t* text, uint16_t x, uint16_t y) {
	element_t* elem = malloc(sizeof(element_t));
	elem->x = x;
	elem->y = y;
	elem->type = elemtext;
	elem->exists = true;

	elem->text = malloc(wstrlen(text) * sizeof(wchar_t));
	wstrcpy(elem->text, text);

	elem->data.raw = NULL;
	elem->data.length = 0;

	// find the first empty element
	int i;
	for (i = 0; win->elements[i] != NULL && i < MAX_ELEMENT_COUNT+1; ++i);
	if(i==MAX_ELEMENT_COUNT)return NULL;

	elem->idx = i;
	win->elements[i] = elem;
	return elem;
}

void remove_text_element(window_t* win, element_t* elem) {
	if (elem->data.raw != NULL && elem->data.length != 0) {
		free((byte_t*)elem->data.raw);
	}

	elem->exists = false;

	win->elements[elem->idx] = NULL;
	free((byte_t*)elem->text);
	free((byte_t*)elem);
}

void draw_text_element(byte_t* membuff, window_t* win, element_t* elem) {
	int x = win->x + elem->x + WIN_MARGIN;
	int y = win->y + CONTROL_HEIGHT + elem->y;
	draw_textw(membuff, win, elem->text, x, y, 0, FONT_WIDTH*2, true);
}

element_t* create_bitmap_element(window_t* win, byte_t* data, uint16_t w, uint16_t h, uint16_t x, uint16_t y) {
	element_t* elem = malloc(sizeof(element_t));
	elem->x = x;
	elem->y = y;
	elem->type = elembitmap;
	elem->exists = true;

	elem->text = NULL;

	elem->data.raw = malloc(sizeof(image_t));
	elem->data.length = sizeof(image_t);

	((image_t*)elem->data.raw)->data = malloc(w*h);
	((image_t*)elem->data.raw)->w = w;
	((image_t*)elem->data.raw)->h = h;

	memcpy(((image_t*)elem->data.raw)->data, data, w*h);

	// find the first empty element
	int i;
	for (i = 0; win->elements[i] != NULL && i < MAX_ELEMENT_COUNT+1; ++i);
	if(i==MAX_ELEMENT_COUNT)return NULL;

	elem->idx = i;
	win->elements[i] = elem;
	return elem;
}

void remove_bitmap_element(window_t* win, element_t* elem) {
	elem->exists = false;

	if (elem->data.raw != NULL) {
		image_t* img = (image_t*)elem->data.raw;
		if (img->data != NULL && elem->data.length > 0) {
			free((byte_t*)img->data);
		}
		free((byte_t*)elem->data.raw);
	}

	win->elements[elem->idx] = NULL;
	free((byte_t*)elem);
}

void draw_bitmap_element(byte_t* membuff, window_t* win, element_t* elem) {
    uint32_t x0 = win->x + elem->x + WIN_MARGIN;
    uint32_t y0 = win->y + CONTROL_HEIGHT + elem->y;
    uint32_t w0 = ((image_t*)elem->data.raw)->w;
    uint32_t h0 = ((image_t*)elem->data.raw)->h;

    image_t* image = (image_t*)elem->data.raw;
    
    // Ensure image and data are valid
    if (image == NULL || image->data == NULL) {
        return;
    }

    for (uint32_t y = 0; y < h0; ++y) {
        for (uint32_t x = 0; x < w0; ++x) {
            // Calculate the index in the linear data array
            uint32_t index = y * w0 + x;
            
            // Calculate coordinates in the membuff
            uint32_t membuff_x = x0 + x;
            uint32_t membuff_y = y0 + y;
            
            // Check bounds
            if (membuff_x >= win->x && membuff_x < win->x + win->w &&
                membuff_y >= win->y && membuff_y < win->y + win->h) {
                // Assuming putp(membuff, x, y, pixel_value) places a pixel at (x, y) in membuff
                putp(membuff, membuff_x, membuff_y, image->data[index]);
            }
        }
    }
}

element_t* create_button_element(window_t* win, wchar_t* text, void (*onClick)(), uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	element_t* elem = malloc(sizeof(element_t));
	elem->data.raw = malloc(sizeof(button_t));

	button_t* but = (button_t*)elem->data.raw;

	elem->x = x;
	elem->y = y;
	elem->type = elembutton;
	elem->exists = true;

	but->w = w;
	but->h = h;

	elem->text = malloc(wstrlen(text) * sizeof(wchar_t));
	wstrcpy(elem->text, text);

	but->onClick = onClick;
	elem->data.length = 1;

	// find the first empty element
	int i;
	for (i = 0; win->elements[i] != NULL && i < MAX_ELEMENT_COUNT+1; ++i);
	if(i==MAX_ELEMENT_COUNT)return NULL;

	elem->idx = i;
	win->elements[i] = elem;
	return elem;
}

void remove_button_element(window_t* win, element_t* elem) {
	elem->exists = false;

	win->elements[elem->idx] = NULL;

	if (elem->data.raw && elem->data.length == 1) {
		free(elem->data.raw);
	}
	
	free((byte_t*)elem->text);
	free((byte_t*)elem);
}

void draw_button_element(byte_t* membuff, window_t* win, element_t* elem) {
	button_t* but = (button_t*)elem->data.raw;
	
	// draw outline
	rect_t rect = {
		.x = win->x + elem->x + WIN_MARGIN,
		.y = win->y + elem->y + CONTROL_HEIGHT,
		.w = but->w,
		.h = but->h,
	};

	drawRectOutline(&rect, 240);

	// draw text
	int x = win->x + elem->x + (but->w / 8) + WIN_MARGIN;
	int y = win->y + elem->y + (but->h / 4) + CONTROL_HEIGHT;
	draw_textw(membuff, win, elem->text, x, y, 0, FONT_WIDTH*2, true);
}

element_t* fetch_element(window_t* win, size_t idx) {
	for (size_t i = 0; i < MAX_ELEMENT_COUNT; ++i) {
		if (win->elements[i]->idx == idx) return win->elements[i];
	}
	return NULL;
}