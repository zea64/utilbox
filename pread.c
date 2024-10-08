#define _GNU_SOURCE
#include "common.h"
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

static const char USAGE[] = "USAGE: pread <PATH> <OFFSET> <SIZE>\n";

int my_pread(int argc, char** argv) {
	ASSERT(argc == 3, USAGE);

	errno = 0;

	char* endptr;
	unsigned long long offset = strtoull(argv[1], &endptr, 10);
	ASSERT(*endptr == '\0' && errno == 0, "Invalid offset\n");

	unsigned long long size = strtoll(argv[2], &endptr, 10);
	ASSERT(*endptr == '\0' && errno == 0, "Invalid size\n");

	int fd = open(argv[0], O_RDONLY);
	ASSERT(fd != -1, "open: %s\n", strerror(errno));

	char buf[8192];
	size_t cur_pointer = offset;
	const size_t end_pointer = size + offset;
	while (cur_pointer < end_pointer) {
		size_t remaining = end_pointer - cur_pointer;
		size_t read_chunk = remaining > sizeof(buf) ? sizeof(buf) : remaining;

		ssize_t size = pread(fd, buf, read_chunk, cur_pointer);
		ASSERT(size != -1, "pread: %s\n", strerror(errno));
		ASSERT(size != 0, "pread: unexpected EOF\n");

		cur_pointer += size;

		size_t total_written = 0;
		do {
			ssize_t write_size =
				write(1, buf + total_written, size - total_written);
			ASSERT(write_size != -1, "write: %s\n", strerror(errno));
			total_written += write_size;
		} while (total_written != read_chunk);
	}

	return EXIT_SUCCESS;
}
