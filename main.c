#include "common.h"
#include <stdlib.h>

int core_sched(int, char **);
int my_pread(int, char **);
int prep_fd(int, char **);

static const struct FnTable top_cmds[] = {
	{"core_sched", &core_sched},
	{"pread", &my_pread},
	{"prep_fd", &prep_fd},
};

constexpr size_t top_cmds_len = sizeof(top_cmds) / sizeof(*top_cmds);

// Returns -1 if command is not found.
int do_cmd(size_t cmds_len, const struct FnTable cmds[static cmds_len], int argc, char **argv) {
	for (size_t i = 0; i < cmds_len; i++)
		if (strcmp(argv[0], cmds[i].name) == 0)
			return cmds[i].fn(argc - 1, argv + 1);

	return -1;
}

[[gnu::cold]] void print_cmds(size_t cmds_len, const struct FnTable cmds[static cmds_len], const char name[static 1]) {
	fprintf(stderr, "Invalid subcommand for %s. Available subcommands:\n", name);
	for (size_t i = 0; i < cmds_len; i++) {
		fprintf(stderr, "\t%s\n", cmds[i].name);
	}
}

int do_cmd_full(size_t cmds_len, const struct FnTable cmds[static cmds_len], int argc, char **argv, const char name[static 1]) {
	int ret = do_cmd(cmds_len, cmds, argc, argv);
	if (ret == -1) {
		print_cmds(cmds_len, cmds, name);
		return EX_USAGE;
	}
	return EX_OK;
}

int main(int argc, char **argv) {
	if (argc == 0)
		goto err;

	int ret = do_cmd(top_cmds_len, top_cmds, argc, argv);
	if (ret != -1)
		return ret;

	// Check for `$ utilbox [subcommand]` now.
	// Requires argc >= 1 because we're going to do argv + 1.
	if (argc == 1)
		goto err;

	ret = do_cmd(top_cmds_len, top_cmds, argc - 1, argv + 1);
	if (ret != -1)
		return ret;

err:
	[[clang::unlikely]] {
		print_cmds(top_cmds_len, top_cmds, "utilbox");
		return EX_USAGE;
	}
}
