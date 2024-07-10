// excluded until FS is working

#pragma once
#include "typedef.h"

typedef enum {
	ROOT, // root
	MOD,  // moderator, has some root commands,
	      // but not all (in linux this would be
			// kinda like "SUDO" but with some
			// functions still restricted, e.g. rm -rf /)
	USER,
} perms_t;

typedef struct {
	wchar_t name[32];
	perms_t perms;
};