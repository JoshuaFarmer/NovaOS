// excluded until FS is working

#pragma once
#include "typedef.h"

typedef enum {
	ROOT, // root
	MOD, // moderator, has some root commands, but not all
	USER,
} perms_t;

typedef struct {
	wchar_t name[32];
	perms_t perms;
};