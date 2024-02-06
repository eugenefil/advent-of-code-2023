#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct mapping {
	unsigned long dst, src, rng;
};

struct map {
	struct mapping mappings[50];
	size_t len;
	char name[32];
};

int parse_seeds(unsigned long *seeds, size_t len)
{
	int num_seeds = 0;
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
		while (*s == ' ')
			++s;
		if (isdigit(*s)) {
			unsigned long seed;
			char *endptr;

			if (num_seeds >= len) {
				fprintf(stderr, "seeds array is not enough\n");
				abort();
			}

			errno = 0;
			seed = strtoul(s, &endptr, 10);
			if (errno != 0)
				goto inv_line;
			s = endptr;
			seeds[num_seeds++] = seed;
		} else if (*s == '\n' || *s == '\0')
			break;
		else
			goto inv_line;
	}
	return num_seeds;

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
	unsigned long seeds[32];
	int num_seeds;
	unsigned long locations[32];
	unsigned long min_loc;
	struct map maps[10];
	int num_maps;

	num_seeds = parse_seeds(seeds, ARRAY_SIZE(seeds));
	if (num_seeds == 0) {
		fprintf(stderr, "0 seeds found\n");
		abort();
	}
#if 0
	printf("seeds:\n");
	for (int i = 0; i < num_seeds; ++i)
		printf("%lu\n", seeds[i]);
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

	for (int i = 0; i < num_seeds; ++i) {
		unsigned long seed = seeds[i];
		for (int j = 0; j < num_maps; ++j) {
			struct map *map = &maps[j];
			for (int k = 0; k < map->len; ++k) {
				struct mapping *m = &map->mappings[k];
				if (m->src <= seed && seed < m->src + m->rng) {
					seed = m->dst + (seed - m->src);
					break;
				}
			}
		}
		locations[i] = seed;
	}

	min_loc = locations[0];
	for (int i = 1; i < num_seeds; ++i) {
		if (locations[i] < min_loc)
			min_loc = locations[i];
	}
	printf("%lu\n", min_loc);
	return 0;
}
