#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static char *digits[] = {
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

static char *strrev(const char *haystack, const char *needle)
{
	size_t haystack_len = strlen(haystack);
	size_t needle_len = strlen(needle);
	const char *s;

	if (haystack_len < needle_len)
		return NULL;

	s = haystack + haystack_len - needle_len;	
	for (; s >= haystack; --s)
		if (strncmp(s, needle, needle_len) == 0)
			return (char *)s;
	return NULL;
}

static int get_calib_value(const char *line, size_t len)
{
	int val, n;
	const char *digit;
	const char *c = line;

	while (*c && !(*c >= 0x30 && *c <= 0x39))
		++c;
	if (*c == '\0')
		return -1;

	n = 0;
	digit = line + len;
	if (c >= line + 3) {
		for (int i = 0; i < ARRAY_SIZE(digits); ++i) {
			const char *d = strstr(line, digits[i]);
			if (d && d < digit) {
				digit = d;
				n = i + 1;
			}
		}
	}

	if (digit > c)
		n = *c - 0x30;
	val = n * 10;

	c = line + len - 1;
	while (!(*c >= 0x30 && *c <= 0x39))
		--c;

	n = 0;
	digit = line - 1;
	if (c < line + len - 3) {
		for (int i = 0; i < ARRAY_SIZE(digits); ++i) {
			const char *d = strrev(line, digits[i]);
			if (d && d > digit) {
				digit = d;
				n = i + 1;
			}
		}
	}

	if (digit < c)
		n = *c - 0x30;

	return val + n;
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
