#include "common.h"
#include <stdlib.h>

struct FnTable {
	const char* name;
	int (*fn)(int, char**);
};

int core_sched(int, char**);
int exchange(int, char**);
int my_pread(int, char**);
int prep_fd(int, char**);

static const struct FnTable cmds[] = {
	{"core_sched", &core_sched},
	{"exchange", &exchange},
	{"pread", &my_pread},
	{"prep_fd", &prep_fd},
};

// Returns -1 if command is not found.
static int do_cmd(int argc, char** argv) {
	for (size_t i = 0; i < (sizeof(cmds) / sizeof(*cmds)); i++)
		if (strcmp(argv[0], cmds[i].name) == 0)
			return cmds[i].fn(argc - 1, argv + 1);

	return -1;
}

int main(int argc, char** argv) {
	if (argc == 0)
		goto err;

	int ret = do_cmd(argc, argv);
	if (ret != -1)
		return ret;

	// Check for `$ utilbox [subcommand]` now.
	// Requires argc >= 1 because we're going to do argv + 1.
	if (argc == 1)
		goto err;

	ret = do_cmd(argc - 1, argv + 1);
	if (ret != -1)
		return ret;

err:
	[[clang::unlikely]] {
		fputs("Invalid subcommand\nAvailable subcommands:\n", stderr);
		for (size_t i = 0; i < (sizeof(cmds) / sizeof(*cmds)); i++) {
			fprintf(stderr, "\t%s\n", cmds[i].name);
		}
		return EX_USAGE;
	}
}
