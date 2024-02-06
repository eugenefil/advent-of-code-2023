#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main()
{
	unsigned long sum = 0;
	char *line = NULL;
	size_t n = 0;

	for (;;) {
		unsigned long id;
		unsigned long rmax, gmax, bmax;
		int ret;
		char *s, *endptr;

		if ((ret = getline(&line, &n, stdin)) <= 0) {
			if (feof(stdin))
				break;
			perror("read stdin");
			return 1;
		}
		if (line[ret - 1] == '\n')
			line[--ret] = '\0';
		s = line;

		if (strncmp(s, "Game ", 5))
			goto inv_line;
		s += 5;

		if (!isdigit(*s))
			goto inv_line;
		errno = 0;
		id = strtoul(s, &endptr, 10);
		if (id == 0 || errno != 0)
			goto inv_line;
		s = endptr;

		if (*s != ':')
			goto inv_line;
		++s;

		rmax = gmax = bmax = 1;
		for (;;) {
			unsigned long num;

			while (*s && (*s == ' ' || *s == ',' || *s == ';'))
				++s;

			if (!isdigit(*s))
				goto inv_line;
			errno = 0;
			num = strtoul(s, &endptr, 10);
			if (num == 0 || errno != 0)
				goto inv_line;
			s = endptr;

			if (*s != ' ')
				goto inv_line;
			++s;

			if (strncmp(s, "red", 3) == 0) {
				s += 3;
				if (num > rmax)
					rmax = num;
			} else if (strncmp(s, "green", 5) == 0) {
				s += 5;
				if (num > gmax)
					gmax = num;
			} else if (strncmp(s, "blue", 4) == 0) {
				s += 4;
				if (num > bmax)
					bmax = num;
			} else
				goto inv_line;

			if (*s == '\0')
				break;
		}

		sum += rmax * gmax * bmax;
	}
	free(line);
	printf("%lu\n", sum);
	return 0;

inv_line:
	fprintf(stderr, "invalid line %s\n", line);
	free(line);
	return 1;
}
