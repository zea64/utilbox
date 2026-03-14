#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#define ASSERT(cond, status, ...) \
	if (!(cond)) { \
		fprintf(stderr, __VA_ARGS__); \
		exit((status)); \
	}
