#include "common.h"
#include <errno.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

static const char USAGE[] = "USAGE: core_sched <TID>|<CMD>\n";

int core_sched(int argc, char** argv) {
	ASSERT(argc >= 1, USAGE);

	char* endptr;
	int tid = strtol(argv[0], &endptr, 10);

	if (*endptr == '\0') { // TID case
		ASSERT(argc == 1, USAGE);
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
	} else { // CMD case
		ASSERT(
			prctl(
				PR_SCHED_CORE,
				PR_SCHED_CORE_CREATE,
				0,
				PR_SCHED_CORE_SCOPE_THREAD_GROUP,
				0
			) != -1,
			"prctl: %s\n",
			strerror(errno)
		);
		execvp(argv[0], argv);
		ASSERT(0, "execvp: %s\n", strerror(errno));
	}

	return EXIT_SUCCESS;
}
