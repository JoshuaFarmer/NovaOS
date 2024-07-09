#pragma once
#include "typedef.h"
#include "interrupts.h"
#include "GUI/text.h"
#include "GUI/element.h"
#include "GUI/graphics.h"
#include "GUI/video.h"
#include "GUI/window.h"

void start_gui(void) {
	init_gdt();
	init_idt();
	init_heap();
	init_graphics();

	window_t *window1 = init_window(96, 144, L"test window 1");
	element_t* text1 = create_text_element(window1, L"Hello, World!", 0, 0);

	if(add_window(window1)) draw_text(membuff, L"BRUH", 0, 0, 0);

	window_mngr(&windows);
	remove_text_element(window1, text1);
}