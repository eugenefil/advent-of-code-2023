#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct number {
	unsigned long num;
	int pos;
	size_t len;
};

struct symbol {
	char c;
	int pos;
};

struct schemline {
	struct number nums[16];
	size_t nums_cnt;
	struct symbol syms[16];
	size_t syms_cnt;
};

int parseline(const char *line, struct schemline *scline)
{
	const char *s = line;

	for (;;) {
		while (*s == '.')
			++s;
		if (*s == '\0')
			break;

		if (isdigit(*s)) {
			struct number *number;
			unsigned long num;
			char *endptr;

			errno = 0;
			num = strtoul(s, &endptr, 10);
			if (errno != 0) {
				fprintf(stderr, "invalid line %s\n", line);
				abort();
			}
			if (scline->nums_cnt + 1 > ARRAY_SIZE(scline->nums)) {
				fprintf(stderr, "nums array len is not enough\n");
				abort();
			}
			number = &scline->nums[scline->nums_cnt++];
			number->num = num;
			number->pos = s - line;
			number->len = endptr - s;
			s = endptr;
		} else {
			struct symbol *symbol;

			if (scline->syms_cnt + 1 > ARRAY_SIZE(scline->syms)) {
				fprintf(stderr, "syms array len is not enough\n");
				abort();
			}
			symbol = &scline->syms[scline->syms_cnt++];
			symbol->c = *s;
			symbol->pos = s - line;
			++s;
		}
	}
	return 0;
}

int get_schemline(struct schemline *scline)
{
	char *line = NULL;
	size_t n = 0;
	int ret;

	memset(scline, 0, sizeof(*scline));
	ret = getline(&line, &n, stdin);
	if (ret <= 0) {
		free(line);
		if (feof(stdin))
			return 0;
		perror("getline stdin");
		return -errno;
	}
	if (line[ret - 1] == '\n')
		line[--ret] = '\0';

	if (parseline(line, scline)) {
		fprintf(stderr, "invalid line %s\n", line);
		free(line);
		return -1;
	}
	free(line);
	return 1;
}

int check_sym(const struct symbol *sym, const struct schemline *scline,
	      unsigned long *gear_nums, size_t len)
{
	int n = 0;

	if (scline == NULL)
		return 0;

	for (int i = 0; i < scline->nums_cnt; ++i) {
		const struct number *num = &scline->nums[i];
		if (num->pos + num->len < sym->pos)
			continue;
		if (num->pos > sym->pos + 1)
			break;
		assert(n < 2);
		if (n < len)
			gear_nums[n] = num->num;
		++n;
	}
	return n;
}

unsigned long sum_gear_ratios(const struct schemline *prev,
			      const struct schemline *cur,
			      const struct schemline *next)
{
	unsigned long sum = 0;
	const struct schemline *sclines[] = {prev, cur, next};

	for (int i = 0; i < cur->syms_cnt; ++i) {
		int n = 0;
		unsigned long gear_nums[2];
		const struct symbol *sym = &cur->syms[i];

		if (sym->c != '*')
			continue;

		for (int j = 0; j < 3; ++j) {
			n += check_sym(sym, sclines[j], gear_nums + n, 2 - n);
			if (n > 2)
				break;
		}
		if (n == 2)
			sum += gear_nums[0] * gear_nums[1];
	}
	return sum;
}

int main()
{
	struct schemline *prev, *cur, *next, *tmp;
	struct schemline sclines[3] = { 0 };
	int ret;
	unsigned long sum = 0;

	prev = &sclines[0];
	cur = &sclines[1];
	next = &sclines[2];

	ret = get_schemline(cur);
	if (ret < 0)
		goto bad_input;

	for (;;) {
		/* for (int i = 0; i < cur->nums_cnt; ++i)
			printf("%lu at %d of len %zu\n", cur->nums[i].num, cur->nums[i].pos, cur->nums[i].len);
		for (int i = 0; i < cur->syms_cnt; ++i)
			printf("%c at %d\n", cur->syms[i].c, cur->syms[i].pos); */

		ret = get_schemline(next);
		if (ret < 0)
			goto bad_input;

		sum += sum_gear_ratios(prev, cur, next);	
		if (ret == 0)
			break;

		tmp = prev;
		prev = cur;
		cur = next;
		next = tmp;
	}
	printf("%lu\n", sum);
	return 0;

bad_input:
	fprintf(stderr, "bad input\n");
	return 1;
}
