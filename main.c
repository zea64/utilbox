#include "common.h"
#include <stdlib.h>

struct FnTable {
	const char* name;
	int (*fn)(int, char**);
};

int exchange(int, char**);
int my_pread(int, char**);
int prep_fd(int, char**);

const struct FnTable cmds[] = {
	{"exchange", &exchange},
	{"pread", &my_pread},
	{"prep_fd", &prep_fd},
};

int main(int argc, char** argv) {
	ASSERT(argc >= 2, "No subcommand specified\n");

	for (size_t i = 0; i < (sizeof(cmds) / sizeof(*cmds)); i++) {
		if (strcmp(argv[1], cmds[i].name) == 0) {
			return cmds[i].fn(argc - 2, argv + 2);
		}
	}

	fputs("Invalid subcommand\n", stderr);
	return EXIT_FAILURE;
}
