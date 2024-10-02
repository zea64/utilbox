#define _GNU_SOURCE
#include "common.h"
#include <fcntl.h>

static const char USAGE[] = "exchange <PATH> <PATH>";

int exchange(int argc, char** argv) {
	ASSERT(argc == 2, USAGE);
	return simple_sys(
		renameat2(AT_FDCWD, argv[0], AT_FDCWD, argv[1], RENAME_EXCHANGE),
		"renameat2"
	);
}
