#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

int main()
{
	unsigned long sum = 0;
	char *line = NULL;
	size_t n = 0;
	int ret;

	for(;;) {
		char *s;
		unsigned long win_nums[10];
		size_t win_cnt = 0;
		unsigned long our_nums[25];
		size_t our_cnt = 0;
		unsigned long points = 0;

		ret = getline(&line, &n, stdin);
		if (ret <= 0) {
			if (feof(stdin))
				break;
			perror("getline stdin");
			return 1;
		}
		if (line[ret - 1] == '\n')
			line[--ret] = '\0';
		
		s = line;
		while (*s && *s != ':')
			++s;
		if (*s == '\0')
			goto inv_line;
		++s;

		for (;;) {
			while (*s == ' ')
				++s;
			if (isdigit(*s)) {
				unsigned long num;
				char *endptr;

				errno = 0;
				num = strtoul(s, &endptr, 10);
				s = endptr;
				if (errno != 0)
					goto inv_line;
				if (win_cnt == ARRAY_SIZE(win_nums)) {
					fprintf(stderr, "win num array is not enough\n");
					return 1;
				}
				win_nums[win_cnt++] = num;
			} else if (*s == '|') {
				++s;
				break;
			} else
				goto inv_line;
		}
		if (win_cnt == 0)
			goto inv_line;

		for (;;) {
			while (*s == ' ')
				++s;
			if (isdigit(*s)) {
				unsigned long num;
				char *endptr;

				errno = 0;
				num = strtoul(s, &endptr, 10);
				s = endptr;
				if (errno != 0)
					goto inv_line;
				if (our_cnt == ARRAY_SIZE(our_nums)) {
					fprintf(stderr, "our num array is not enough\n");
					return 1;
				}
				our_nums[our_cnt++] = num;
			} else if (*s == '\0')
				break;
			else
				goto inv_line;
		}
		if (our_cnt == 0)
			goto inv_line;

		/* for (int i = 0; i < win_cnt; ++i)
			printf("%lu,", win_nums[i]);
		printf("\n");
		for (int i = 0; i < our_cnt; ++i)
			printf("%lu,", our_nums[i]);
		printf("\n"); */

		for (int i = 0; i < our_cnt; ++i) {
			for (int j = 0; j < win_cnt; ++j) {
				if (our_nums[i] == win_nums[j]) {
					points = points == 0 ? 1 : points * 2;
					break;
				}
			}
		}
		sum += points;
	}
	printf("%lu\n", sum);
	return 0;

inv_line:
	fprintf(stderr, "invalid line %s\n", line);
	free(line);
	return 1;
}
