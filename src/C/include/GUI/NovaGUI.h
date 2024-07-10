#pragma once
#include "typedef.h"
#include "interrupts.h"
#include "GUI/text.h"
#include "GUI/element.h"
#include "GUI/graphics.h"
#include "GUI/video.h"
#include "GUI/window.h"

int example_main(void* ptr) {
	if (ptr == NULL) return -1;

	window_t* window = (window_t*)ptr;
	create_text_element(window, L"TEST #1", 10, 10);
	return 0;
}

void example_update(void* ptr) {
	if (ptr == NULL) return;
	window_t* win = (window_t*)ptr;
	wstrcpy(win->elements[0]->text, L"TEST #2");
	win->elements[0]->x++;
	return;
}

void start_gui(void) {
	init_gdt();
	init_idt();
	init_heap();
	init_graphics();

	window_t *window1 = init_window(96, 144, L"test window 1", example_main, example_update);

	if(add_window(window1)) draw_text(membuff, L"BRUH", 0, 0, 0);

	window_mngr(&windows);
}