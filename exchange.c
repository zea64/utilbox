#define _GNU_SOURCE
#include "common.h"
#include <fcntl.h>

static const char USAGE[] = "exchange <PATH> <PATH>\n";

int exchange(int argc, char** argv) {
	ASSERT(argc == 2, USAGE);
	ASSERT(
		renameat2(AT_FDCWD, argv[0], AT_FDCWD, argv[1], RENAME_EXCHANGE) != -1,
		"renameat2: %s\n",
		strerror(errno)
	);
	return EXIT_SUCCESS;
}
