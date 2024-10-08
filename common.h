#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(cond, ...) \
	if (!(cond)) { \
		fprintf(stderr, __VA_ARGS__); \
		exit(EXIT_FAILURE); \
	}
