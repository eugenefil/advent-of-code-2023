#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int readline(int fd, char *buf, size_t len)
{
	static char *nl;
	static size_t nread = 0;
	int ret;

	if (nl != NULL) {
		nread -= nl + 1 - buf;
		memmove(buf, nl + 1, nread + 1);
	}

	while (len - 1 - nread > 0) {
		ret = read(fd, buf + nread, len - 1 - nread);
		if (ret < 0) {
			perror("read");
			return -errno;
		}
		if (ret == 0) {
			if (nread == 0)
				return 0;
			break;
		}
		nread += ret;
		buf[nread] = '\0';
	}

	nl = strchr(buf, '\n');
	if (nl == NULL) {
		if (nread == len - 1)
			fprintf(stderr, "buffer is not big enough for line: %s\n", buf);
		else
			fprintf(stderr, "missing newline on line: %s\n", buf);
		return -1;
	}

	*nl = '\0';
	if (*buf == '\0') {
		fprintf(stderr, "empty line found\n");
		return -1;
	}
	return nl - buf;
}

int get_calib_value(char *line, size_t len)
{
	int val;
	char *c = line;

	while (*c && !(*c >= 0x30 && *c <= 0x39))
		++c;
	if (*c == '\0')
		return -1;
	val = (*c - 0x30) * 10;

	c = line + len - 1;
	while (!(*c >= 0x30 && *c <= 0x39))
		--c;
	return val + (*c - 0x30);
}

int main(int argc, char *argv[])
{
	char buf[64];
	int len;
	unsigned long sum = 0;
	int fd;

	if (argc == 2 && strcmp(argv[1], "-") == 0)
		fd = 0;
	else
		fd = open("input1", O_RDONLY);
	if (fd < 0) {
		perror("open input1");
		return 1;
	}

	for (;;) {
		int val;
		int ret = readline(fd, buf, sizeof(buf));
		if (ret < 0) {
			fprintf(stderr, "error reading input\n");
			return 1;
		}
		if (ret == 0)
			break;

		val = get_calib_value(buf, ret);
		if (val < 0) {
			fprintf(stderr, "no calibration value found in line %s\n", buf);
			return 1;
		}
		printf("%s %d\n", buf, val);
		sum += val;
	}

	close(fd);
	printf("%lu\n", sum);
	return 0;
}
