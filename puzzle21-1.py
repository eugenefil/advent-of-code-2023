import sys

S = None
grid = []
for y, line in enumerate(sys.stdin):
    line = line.rstrip()
    grid.append(list(line))
    x = line.find('S')
    if x > -1:
        S = (x, y)
assert S
print('S', S)
h, w = len(grid), len(grid[0])
print('h', h, 'w', w)

odd = {}
even = {S: 1}
plots = {S: 1}
n = 0
N = int(sys.argv[1]) if len(sys.argv) == 2 else 64
while n < N:
    n += 1
    prev = odd if n % 2 else even
    plots1 = {}
    for x, y in plots:
        for dx, dy in ((-1, 0), (0, -1), (1, 0), (0, 1)):
            x1, y1 = x + dx, y + dy
            if x1 < 0 or x1 >= w or y1 < 0 or y1 >= h:
                continue
            if grid[y1][x1] == '#':
                continue
            if (x1, y1) in prev:
                continue
            if (x1, y1) in plots1:
                continue
            plots1[(x1, y1)] = 1
    assert plots1
    for p in plots1:
        prev[p] = 1
    plots = plots1

fin = odd if n % 2 else even
# for x, y in fin:
#     grid[y][x] = 'O'
# for row in grid:
#     print(''.join(row))
print(len(fin))
