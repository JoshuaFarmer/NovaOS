void text_editor() {
	clsscr();
	txtx = 0;
	txty = 0;
	update_cursor(txtx,txty);

	uint16_t c = 0;
	bool running = true;

	while (running) {
		c = getch();
		if (c == 0xF001) {
			c = getch();
			if (c == 'c') return;
		} else {
			if ((c >= ' ' && c <= '~') || c == KEY_TAB || c == '\n' || c == '\b') {
				putc(c);
			}
		}
	}
}

void system(const uint16_t* sys) {
	char cmd[10][128];

	for (int h = 0; h < 10; h++) {
		memset(cmd[h], 0, 128);
	}

	int c = 0;
	int i = 0;
	int j = 0;

	while (sys[i] != '\0') {
		while (sys[i] == ' ')
			i+=1;

		if (sys[i] == '\0')
			break;

		while (sys[i] != ' ' && sys[i] != '\0') {
			cmd[c][j++] = sys[i++];
		}
		cmd[c][j] = '\0';
		++c;
		j = 0;
	}

	if (strcmp(cmd[0], "user") == 0) {
		if (c > 1) {
			for (int k = 0; k < c; ++k) {
				if (strcmp(cmd[k], "-s") == 0 && k+1 != c) {
					memcpy(system_user, cmd[k+1], 128);
					break;
				}
			}
		}
	}

	else if (strcmp(cmd[0], "clear") == 0) {
		clsscr();
		txtx = 0;
		txty = 0;
	}

	else if (strcmp(cmd[0], "ls") == 0) {
		// later.
	}

	else if (strcmp(cmd[0], "exit") == 0) {
		running = false;
	}

	else if (strcmp(cmd[0], "edit") == 0) {
		text_editor();
	}

	else if (strcmp(cmd[0], "txtmode") == 0) {
		text_editor();
		if (strcmp(cmd[1], "") == 0) {return;}
		else if (strcmp(cmd[1], "-80x25") == 0) {
			set_text_mode(0);
			VGA_WIDTH = 80;
			VGA_HEIGHT = 25;
		} else if (strcmp(cmd[1], "-90x60") == 0) {
			set_text_mode(1);
			VGA_WIDTH = 90;
			VGA_HEIGHT = 60;
		}
	}

	else if (strcmp(cmd[0], "") == 0) {}
	else {
		printf("COMMAND: %s\n", cmd[0]);
		printf("ARG(S):\n");
		for (int k = 1; k < c; ++k) {;
			puts(cmd[k]); putc('\n');
		}
	}
}

void shell(const char* system_user, const char* current_dir, uint16_t* kbdbuf) {
	while (running) {
		printf("%T10.%s%T7.@%T9.%s%T7.", system_user, current_dir);
		gets((uint16_t*)kbdbuf, 128);

		system((const uint16_t*)kbdbuf);
	}
}