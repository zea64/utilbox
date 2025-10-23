#include "common.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/prctl.h>

static const char USAGE[] = "USAGE: core_sched <TID>\n";

int core_sched(int argc, char** argv) {
	ASSERT(argc == 1, USAGE);

	char* endptr;
	int tid = strtol(argv[0], &endptr, 10);
	ASSERT(*endptr == '\0', "Invalid tid\n");

	ASSERT(
		prctl(
			PR_SCHED_CORE,
			PR_SCHED_CORE_CREATE,
			tid,
			PR_SCHED_CORE_SCOPE_THREAD,
			0
		) != -1,
		"prctl: %s\n",
		strerror(errno)
	);

	return EXIT_SUCCESS;
}
