#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int parse_table_line(unsigned long *num, const char *prefix)
{
	unsigned long x;
	char buf[16] = { 0 };
	size_t len = 0;
	char *line = NULL;
	size_t n = 0;
	char *s;
	char *endptr;

	if (getline(&line, &n, stdin) <= 0) {
		free(line);
		if (feof(stdin))
			return 0;
		perror("getline");
		return -errno;
	}

	s = line;
	if (strncmp(s, prefix, strlen(prefix))) {
		fprintf(stderr, "no prefix '%s' in line '%s'", prefix, line);
		goto err;
	}
	s += strlen(prefix);

	for (;;) {
		while (*s == ' ')
			++s;
		if (isdigit(*s)) {
			size_t cnt = strspn(s, "0123456789");
			if (len + cnt >= sizeof(buf)) {
				fprintf(stderr, "num buf is not enough\n");
				goto err;
			}
			memcpy(buf + len, s, cnt);
			len += cnt;
			s += cnt;
		} else if (*s == '\n' || *s == '\0')
			break;
		else {
			fprintf(stderr, "invalid line %s", line);
			goto err;
		}
	}

	errno = 0;
	x = strtoul(buf, &endptr, 10);
	if (errno != 0 || endptr == buf) {
		fprintf(stderr, "error parsing number in line %s", line);
		goto err;
	}
	*num = x;

	return 1;

err:
	free(line);
	return -1;
}

int main()
{
	int wins = 0;
	unsigned long time;
	unsigned long distance;

	if (parse_table_line(&time, "Time:") < 1)
		return 1;
	printf("time %lu\n", time);

	if (parse_table_line(&distance, "Distance:") < 1)
		return 1;
	printf("distance %lu\n", distance);

	for (unsigned long t = 1; t < time; ++t) {
		unsigned long dist = t * (time - t);
		if (dist > distance)
			++wins;
	}
	
	printf("%d\n", wins);
	return 0;
}
