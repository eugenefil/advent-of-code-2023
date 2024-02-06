#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct seed_range {
	unsigned long start, len;
};

struct mapping {
	unsigned long dst, src, rng;
};

struct map {
	struct mapping mappings[50];
	size_t len;
	char name[32];
};

int parse_seed_ranges(struct seed_range *ranges, size_t len)
{
	int num_ranges = 0;
	char *line = NULL;
	size_t n = 0;
	char *s;
	int ret;

	ret = getline(&line, &n, stdin);
	if (ret <= 0) {
		free(line);
		if (feof(stdin))
			return 0;
		perror("getline stdin");
		abort();
	}
	
	s = line;
	if (strncmp(s, "seeds:", 6))
		goto inv_line;
	s += 6;

	for (;;) {
		unsigned long nums[2];
		int i;
		char *endptr;

		for (i = 0; i < 2; ++i) {
			while (*s == ' ')
				++s;
			if (isdigit(*s)) {
				errno = 0;
				nums[i] = strtoul(s, &endptr, 10);
				if (errno != 0)
					goto inv_line;
				s = endptr;
			} else if (*s == '\n' || *s == '\0')
				break;
			else
				goto inv_line;
		}
		if (i == 2) {
			struct seed_range *range;

			if (num_ranges >= len) {
				fprintf(stderr, "seed ranges array is not enough\n");
				abort();
			}
			range = &ranges[num_ranges++];
			range->start = nums[0];
			range->len = nums[1];
		} else if (i == 0)
			break;
		else {
			fprintf(stderr, "range start %lu without len\n", nums[0]);
			abort();
		}
	}
	free(line);
	return num_ranges;

inv_line:
	fprintf(stderr, "invalid pos %lu in line %s\n", s - line, line);
	free(line);
	abort();
}

int parse_maps(struct map *maps, size_t len)
{
	int num_maps = 0;
	char *line = NULL;
	size_t n = 0;
	int ret;

	for (;;) {
		struct map *map;
		char *s;

		do {
			ret = getline(&line, &n, stdin);
			if (ret <= 0) {
				free(line);
				if (feof(stdin))
					return num_maps;
				perror("getline stdin");
				abort();
			}
		} while (*line == '\n');

		if (num_maps >= len) {
			fprintf(stderr, "maps array is not enough\n");
			abort();
		}
		map = &maps[num_maps];
		memset(map, 0, sizeof(*map));

		s = line;
		while (*s && *s != ' ')
			++s;
		if (*s != ' ')
			goto inv_line;
		if (s - line >= sizeof(map->name)) {
			fprintf(stderr, "map name size is not enough\n");
			abort();
		}
		memcpy(map->name, line, s - line);

		for (;;) {
			int num_cnt;
			struct mapping *mapping;
			unsigned long mapping_nums[3];

			ret = getline(&line, &n, stdin);
			if (ret <= 0) {
				if (feof(stdin))
					break;
				perror("getline stdin");
				abort();
			}
			if (*line == '\n')
				break;

			if (map->len >= ARRAY_SIZE(map->mappings)) {
				fprintf(stderr, "map len is not enough\n");
				abort();
			}
			mapping = &map->mappings[map->len];

			s = line;
			num_cnt = 0;
			for (;;) {
				while (*s == ' ')
					++s;
				if (isdigit(*s)) {
					unsigned long num;
					char *endptr;

					if (num_cnt >= ARRAY_SIZE(mapping_nums))
						goto inv_line;

					errno = 0;
					num = strtoul(s, &endptr, 10);
					if (errno != 0)
						goto inv_line;
					s = endptr;
					mapping_nums[num_cnt++] = num;
				} else if (*s == '\n' || *s == '\0')
					break;
				else
					goto inv_line;
			}
			if (num_cnt != 3)
				goto inv_line;
			mapping->dst = mapping_nums[0];
			mapping->src = mapping_nums[1];
			mapping->rng = mapping_nums[2];

			map->len++;
		}
		if (map->len == 0) {
			fprintf(stderr, "0 mappings in map %s\n", map->name);
			abort();
		}

		++num_maps;
	}
	assert(0);

inv_line:
	fprintf(stderr, "invalid line %s\n", line);
	free(line);
	abort();
}

int main()
{
	struct seed_range ranges[256];
	int num_ranges;
	unsigned long min_loc;
	struct map maps[10];
	int num_maps;

	num_ranges = parse_seed_ranges(ranges, ARRAY_SIZE(ranges));
	if (num_ranges == 0) {
		fprintf(stderr, "0 ranges found\n");
		abort();
	}
#if 0
	printf("%d ranges:\n", num_ranges);
	for (int i = 0; i < num_ranges; ++i)
		printf("%lu %lu\n", ranges[i].start, ranges[i].len);
#endif
	num_maps = parse_maps(maps, ARRAY_SIZE(maps));
	if (num_maps == 0) {
		fprintf(stderr, "0 maps found\n");
		abort();
	}
#if 0
	for (int i = 0; i < num_maps; ++i) {
		struct map *map = &maps[i];
		printf("\n%s:\n", map->name);
		for (int j = 0; j < map->len; ++j) {
			struct mapping *m = &map->mappings[j];
			printf("%lu-%lu -> %lu-%lu\n",
				m->src, m->src + m->rng - 1,
				m->dst, m->dst + m->rng - 1);
		}
	}
#endif

	for (int i = 0; i < num_maps; ++i) {
		struct map *map = &maps[i];
		for (int j = 0; j < num_ranges; ++j) {
			struct seed_range *range = &ranges[j];
			for (int k = 0; k < map->len; ++k) {
				struct mapping *m = &map->mappings[k];
				if (range->start + range->len - 1 < m->src ||
				    range->start > m->src + m->rng - 1)
					continue;
				if (range->start < m->src) {
					if (num_ranges >= ARRAY_SIZE(ranges)) {
						fprintf(stderr, "ranges array is not enough\n");
						abort();
					}
					ranges[num_ranges] = (struct seed_range){
						.start = range->start,
						.len = m->src - range->start,
					};
					range->start = m->src;
					range->len -= ranges[num_ranges].len;
					++num_ranges;
				}
				if (range->start + range->len > m->src + m->rng) {
					if (num_ranges >= ARRAY_SIZE(ranges)) {
						fprintf(stderr, "ranges array is not enough\n");
						abort();
					}
					ranges[num_ranges] = (struct seed_range){
						.start = m->src + m->rng,
						.len = range->start + range->len -
						       (m->src + m->rng),
					};
					range->len -= ranges[num_ranges].len;
					++num_ranges;
				}
				range->start = range->start - m->src + m->dst;
				break;
			}
		}
	}

	min_loc = ranges[0].start;
	for (int i = 1; i < num_ranges; ++i) {
		if (ranges[i].start < min_loc)
			min_loc = ranges[i].start;
	}

	printf("%lu\n", min_loc);
	return 0;
}
