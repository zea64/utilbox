#include "common.h"
#include <sys/prctl.h>
#include <unistd.h>

static const char USAGE[] = "USAGE:\n"
							"core_sched create <PID>\n"
							"core_sched get <TID>\n"
							"core_sched sharefrom <SRC TID> <CMD>...\n"
							"core_sched shareto <SRC TID> <DST TID>\n"
							"core_sched spawn <CMD>\n";

static int create(int argc, char **argv) {
	ASSERT(argc == 1, EX_USAGE, USAGE);

	int pid = atoi(argv[0]);
	ASSERT(pid != 0, EX_USAGE, USAGE);

	ASSERT(0 == prctl(PR_SCHED_CORE, PR_SCHED_CORE_CREATE, pid, PR_SCHED_CORE_SCOPE_THREAD, 0), EX_NOPERM, "prctl: %s", strerror(errno));
	return EX_OK;
}

static int get(int argc, char **argv) {
	ASSERT(argc == 1, EX_USAGE, USAGE);

	int pid = atoi(argv[0]);
	ASSERT(pid != 0, EX_USAGE, USAGE);

	unsigned long long cookie = -1;
	ASSERT(0 == prctl(PR_SCHED_CORE, PR_SCHED_CORE_GET, pid, PR_SCHED_CORE_SCOPE_THREAD, &cookie), EX_NOPERM, "prctl: %s", strerror(errno));

	printf("%llu\n", cookie);
	return EX_OK;
}

static int sharefrom(int argc, char **argv) {
	ASSERT(argc >= 2, EX_USAGE, USAGE);

	int pid = atoi(argv[0]);
	ASSERT(pid != 0, EX_USAGE, USAGE);

	ASSERT(0 == prctl(PR_SCHED_CORE, PR_SCHED_CORE_SHARE_FROM, pid, PR_SCHED_CORE_SCOPE_THREAD, 0), EX_NOPERM, "prctl: %s", strerror(errno));

	execvp(argv[1], &argv[1]);
	ASSERT(0, EX_OSERR, "execvp: %s\n", strerror(errno));
}

static int shareto(int argc, char **argv) {
	ASSERT(argc == 2, EX_USAGE, USAGE);

	int src_pid = atoi(argv[0]);
	ASSERT(src_pid != 0, EX_USAGE, USAGE);

	int dst_pid = atoi(argv[1]);
	ASSERT(dst_pid != 0, EX_USAGE, USAGE);

	ASSERT(0 == prctl(PR_SCHED_CORE, PR_SCHED_CORE_SHARE_FROM, src_pid, PR_SCHED_CORE_SCOPE_THREAD, 0), EX_NOPERM, "SHARE_FROM prctl: %s", strerror(errno));
	ASSERT(0 == prctl(PR_SCHED_CORE, PR_SCHED_CORE_SHARE_TO, dst_pid, PR_SCHED_CORE_SCOPE_THREAD, 0), EX_NOPERM, "SHARE_TO prctl: %s", strerror(errno));
	return EX_OK;
}

static int spawn(int argc, char **argv) {
	ASSERT(argc >= 1, EX_USAGE, USAGE);

	ASSERT(0 == prctl(PR_SCHED_CORE, PR_SCHED_CORE_CREATE, 0, PR_SCHED_CORE_SCOPE_THREAD, 0), EX_NOPERM, "prctl: %s", strerror(errno));

	execvp(argv[0], &argv[0]);
	ASSERT(0, EX_OSERR, "execvp: %s\n", strerror(errno));
}

static const struct FnTable cmds[] = {
	{"create", &create}, {"get", &get}, {"sharefrom", &sharefrom}, {"shareto", &shareto}, {"spawn", &spawn},
};

constexpr size_t cmds_len = sizeof(cmds) / sizeof(*cmds);

int core_sched(int argc, char **argv) { return do_cmd_full(cmds_len, cmds, argc, argv, "core_sched"); }
