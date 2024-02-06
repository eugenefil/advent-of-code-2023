#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	char *line = NULL;
	size_t n = 0;
	unsigned long sum = 0;

	for (;;) {
		int val;
		ssize_t ret = getline(&line, &n, stdin);
		if (ret < 0) {
			if (feof(stdin))
				break;
			perror("getline");
			free(line);
			return 1;
		}
		if (ret > 0 && line[ret - 1] == '\n')
			line[--ret] = '\0';

		val = get_calib_value(line, ret);
		if (val < 0) {
			fprintf(stderr, "no calibration value found in line %s\n", line);
			return 1;
		}
		printf("%s %d\n", line, val);
		sum += val;
	}

	printf("%lu\n", sum);
	free(line);
	return 0;
}
