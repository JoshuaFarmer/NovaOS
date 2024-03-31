dat:
	; system information
	SYS_help_text	db "A List of commands for COSA.", 10, 13
			db "restart     |       restart the computer", 10, 13
			db "splash      |       display the splash screen", 10, 13 
			db "help        |       show this list", 10, 13
			db "get         |       read the contents of disk 1", 10, 13
			db "run         |       run code from 8000h", 10, 13
			db "mon         |       very bad monitor program", 10, 13
			db "set-user    |       change the prompt user", 10, 13, 0
	SYS_desc     db "Computer Operating System A.", 10, 13, "Kernel Version 0.3", 10, 13, "(C) Joshua Farmer 2023", 10, 13, 0

	SYS_colour	 db 0x1F
	SYS_prompt   db "> ", 0    
	SYS_user     db "sys", 20h, 0
	times 24	 db 0

	; system commands
	INST_reboot db "reboot", 0
	INST_desc   db "splash", 0
	INST_help   db "help", 0
	INST_user   db "set-user", 0
	INST_cls    db "cls", 0
	INST_ls     db "ls", 0

	INST_disk    db "get", 0
	INST_monitor db "mon", 0
	INST_run     db "run", 0 ; run code from 0x8000
	
	INST_WFSE    db "WFSE", 0
	
	; gp data
	DAT_run_error	  db "unable to start program :(", 10, 13, 0
	
	DAT_exit_code dw 0
	DAT_exit_code_msg db "program ended with code : ", 0

	DAT_user_prompt	  db "enter your name: ", 0
	DAT_new_line      db 13, 10, 0
dat_t: