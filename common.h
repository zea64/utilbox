#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#define ASSERT(cond, status, ...)                                                                                                                              \
	if (!(cond)) {                                                                                                                                             \
		fprintf(stderr, __VA_ARGS__);                                                                                                                          \
		exit((status));                                                                                                                                        \
	}

struct FnTable {
	const char *name;
	int (*fn)(int, char **);
};

int do_cmd(size_t cmds_len, const struct FnTable cmds[static cmds_len], int argc, char **argv);
[[gnu::cold]] void print_cmds(size_t cmds_len, const struct FnTable cmds[static cmds_len], const char name[static 1]);
int do_cmd_full(size_t cmds_len, const struct FnTable cmds[static cmds_len], int argc, char **argv, const char name[static 1]);
