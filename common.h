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

inline int simple_sys(int ret, const char* perr) {
	if (ret == -1) {
		perror(perr);
		ret = -errno;
	}

	return ret;
}
