import sys

grid = []
expand_y = []
for y, line in enumerate(sys.stdin):
    line = line.rstrip()
    grid.append(list(line))
    if line.strip('.') == '':
        expand_y.append(y)
print('expand y', expand_y)

w, h = len(grid[0]), len(grid)
print(f'w {w} h {h}')

expand_x = []
for x in range(w):
    n = 0
    for y in range(h):
        if grid[y][x] != '.':
            n += 1
            break
    if n == 0:
        expand_x.append(x)
print('expand x', expand_x)

galaxies = []
for y, row in enumerate(grid):
    for x, c in enumerate(row):
        if c == '#':
            galaxies.append((x, y))
print("galaxies", len(galaxies))

n = 0
sum = 0
for i, g1 in enumerate(galaxies[:-1]):
    for g2 in galaxies[i + 1:]:
        n += 1

        nx = 0
        x1, x2 = sorted([g1[0], g2[0]])
        for x in expand_x:
            if x > x1 and x < x2:
                nx += 1
        ny = 0
        y1, y2 = sorted([g1[1], g2[1]])
        for y in expand_y:
            if y > y1 and y < y2:
                ny += 1
        sum += abs(g2[0] - g1[0]) + abs(g2[1] - g1[1]) + (nx + ny) * 999999
print(n, sum)
