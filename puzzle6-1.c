#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

int parse_table_line(unsigned long *nums, size_t len, const char *prefix)
{
	int num_cnt = 0;
	char *line = NULL;
	size_t n = 0;
	int ret;
	char *s;

	ret = getline(&line, &n, stdin);
	if (ret <= 0) {
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
			unsigned long num;
			char *endptr;

			if (num_cnt >= len) {
				fprintf(stderr, "nums array is not enough\n");
				goto err;
			}

			errno = 0;
			num = strtoul(s, &endptr, 10);
			if (errno != 0) {
				perror("strtoul");
				goto err;
			}
			s = endptr;
			nums[num_cnt++] = num;
		} else if (*s == '\n' || *s == '\0')
			break;
		else {
			fprintf(stderr, "invalid line %s", line);
			goto err;
		}
	}
	free(line);
	return num_cnt;

err:
	free(line);
	return -1;
}

int main()
{
	unsigned long prod = 1;
	unsigned long times[4];
	int num_times;
	unsigned long distances[4];
	int num_dist;

	num_times = parse_table_line(times, ARRAY_SIZE(times), "Time:");
	if (num_times <= 0)
		return 1;
#if 0
	printf("times:\n");
	for (int i = 0; i < num_times; ++i)
		printf("%lu\n", times[i]);
#endif

	num_dist = parse_table_line(distances, ARRAY_SIZE(distances), "Distance:");
	if (num_dist <= 0)
		return 1;
#if 0
	printf("distances:\n");
	for (int i = 0; i < num_dist; ++i)
		printf("%lu\n", distances[i]);
#endif

	if (num_times != num_dist) {
		fprintf(stderr, "#times is %d, #distance is %d\n", num_times, num_dist);
		return 1;
	}

	for (int i = 0; i < num_times; ++i) {
		int wins = 0;
		unsigned long time = times[i];

		for (int t = 1; t < time; ++t) {
			int dist = t * (time - t);
			if (dist > distances[i])
				++wins;
		}
		fprintf(stderr, "time %lu dist %lu: %d wins\n", time, distances[i], wins);
		prod *= wins;
	}
	
	printf("%lu\n", prod);
	return 0;
}
