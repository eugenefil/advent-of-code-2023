import sys

grid = []
for line in sys.stdin:
    line = line.rstrip()
    grid.append(list(line))
    if line.strip('.') == '':
        grid.append(list(line))

w, h = len(grid[0]), len(grid)
print(f'w {w} h {h}')

for x in reversed(range(w)):
    n = 0
    for y in range(h):
        if grid[y][x] != '.':
            n += 1
            break
    if n == 0:
        for y in range(h):
            grid[y].insert(x + 1, '.')
        w += 1
print(f'w {w} h {h}')

# for row in grid:
#     print(''.join(row))

galaxies = []
for y, row in enumerate(grid):
    for x, c in enumerate(row):
        if c == '#':
            galaxies.append((x, y))

n = 0
sum = 0
for i, g1 in enumerate(galaxies[:-1]):
    for g2 in galaxies[i + 1:]:
        n += 1
        sum += abs(g2[0] - g1[0]) + abs(g2[1] - g1[1])
print(n, sum)
