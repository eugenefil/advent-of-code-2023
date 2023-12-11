#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct card {
	int wins;
	int extra;
};

int parsecards(struct card *cards, size_t len)
{
	char *line = NULL;
	size_t n = 0;
	int num_cards = 0;

	for(;;) {
		int ret;
		char *s;
		unsigned long win_nums[10];
		size_t win_cnt = 0;
		unsigned long our_nums[25];
		size_t our_cnt = 0;
		struct card *card;

		ret = getline(&line, &n, stdin);
		if (ret <= 0) {
			free(line);
			if (feof(stdin))
				return num_cards;
			perror("getline stdin");
			abort();
		}
		if (line[ret - 1] == '\n')
			line[--ret] = '\0';
		
		if (num_cards == len) {
			fprintf(stderr, "cards array is not enough\n");
			abort();
		}
		card = &cards[num_cards++];
		memset(card, 0, sizeof(*card));
		
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
					card->wins++;
					break;
				}
			}
		}
	}
	assert(0);

inv_line:
	fprintf(stderr, "invalid line %s\n", line);
	free(line);
	abort();
}

int main()
{
	struct card cards[256];
	int num_cards;
	int num_gen_cards = 0;

	num_cards = parsecards(cards, ARRAY_SIZE(cards));
	for (int i = 0; i < num_cards; ++i) {
		if (cards[i].wins > 0) {
			int last = MIN(i + cards[i].wins, num_cards - 1);
			for (int j = i + 1; j <= last; ++j)
				cards[j].extra++;
		}
	}

	for (int i = 0; i < num_cards; ++i) {
		num_gen_cards += 1 + cards[i].extra;
		if (cards[i].wins > 0 && cards[i].extra > 0) {
			int last = MIN(i + cards[i].wins, num_cards - 1);
			for (int j = i + 1; j <= last; ++j)
				cards[j].extra += cards[i].extra;
		}
	}
	printf("%d\n", num_gen_cards);
	return 0;
}
