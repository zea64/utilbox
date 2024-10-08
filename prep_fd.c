#include "common.h"

#include <fcntl.h>
#include <unistd.h>

static const char USAGE[] = "Usage: prep_fd [FD=PATH]... -- <EXE> [ARGS]...\n";

int prep_fd(int argc, char** argv) {
	int i = 0;
	for (; i < argc; i++) {
		const char* cur_arg = argv[i];

		// Move on to execve arguments
		if (strcmp(cur_arg, "--") == 0) {
			i++;
			break;
		}

		char* endptr;
		int requested_fd = strtol(cur_arg, &endptr, 10);
		ASSERT(*endptr == '=', USAGE);

		int fd = open(endptr + 1, O_RDWR);
		ASSERT(fd != -1, "open %s: %s\n", endptr, strerror(errno));

		ASSERT(
			dup2(fd, requested_fd) != -1,
			"dup2 %d: %s\n",
			requested_fd,
			strerror(errno)
		);
	}

	ASSERT(i < argc, USAGE);

	execvp(argv[i], &argv[i]);

	// This is only reachable if exec fails
	ASSERT(0, "exec: %s\n", strerror(errno));
}
