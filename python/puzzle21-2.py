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
N = w // 2 + 4 * w
print('N', N)
assert N >= w // 2 and N >= h // 2
while n < N:
    n += 1
    prev = odd if n % 2 else even
    plots1 = {}
    for x, y in plots:
        for dx, dy in ((-1, 0), (0, -1), (1, 0), (0, 1)):
            x1, y1 = x + dx, y + dy
            if grid[y1 % h][x1 % w] == '#':
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
print(len(fin))

# h1 = w1 = 2 * N + 1
# print('h1', h1, 'w1', w1)
# grid1 = [['.' for x in range(w1)] for y in range(h1)]
# for y in range(h1):
#     for x in range(w1):
#         c = grid[(y + h // 2 - N) % h][(x + w // 2 - N) % w]
#         if c == 'S' and not (x == N and y == N):
#             c = '.'
#         grid1[y][x] = c
# for x, y in fin:
#     grid1[y + N - h // 2][x + N - w // 2] = 'O'
# vspacers = list(range(N - w // 2, 0, -w)) + list(range(N + w // 2 + 1, w1, w))
# vspacers.sort()
# hspacers = list(range(N - h // 2, 0, -h)) + list(range(N + h // 2 + 1, h1, h))
# hspacers.sort()
# for sp in reversed(hspacers):
#     grid1.insert(sp, [' '] * w1)
# for row in grid1:
#     for sp in reversed(vspacers):
#         row.insert(sp, ' ')
# with open('out21', 'w') as f:
#     for row in grid1:
#         print(''.join(row), file=f)

center = sum(fin.get((x, y), 0) for y in range(h) for x in range(w))
print('center', center)
left = sum(fin.get((x, y), 0) for y in range(-h, 2 * h) for x in range(-4 * w, -2 * w))
print('left', left)
right = sum(fin.get((x, y), 0) for y in range(-h, 2 * h) for x in range(3 * w, 5 * w))
print('right', right)
top = sum(fin.get((x, y), 0) for y in range(-4 * h, -2 * h) for x in range(-2 * w, 3 * w))
print('top', top)
bottom = sum(fin.get((x, y), 0) for y in range(3 * h, 5 * h) for x in range(-2 * w, 3 * w))
print('bottom', bottom)

print()
luo_edge = sum(fin.get((x, y), 0) for y in range(-2 * h, -h) for x in range(-3 * w, -2 * w))
print('luo_edge', luo_edge)
lui_edge = sum(fin.get((x, y), 0) for y in range(-2 * h, -h) for x in range(-2 * w, -w))
print('lui_edge', lui_edge)

ldo_edge = sum(fin.get((x, y), 0) for y in range(2 * h, 3 * h) for x in range(-3 * w, -2 * w))
print('ldo_edge', ldo_edge)
ldi_edge = sum(fin.get((x, y), 0) for y in range(2 * h, 3 * h) for x in range(-2 * w, -w))
print('ldi_edge', ldi_edge)

ruo_edge = sum(fin.get((x, y), 0) for y in range(-2 * h, -h) for x in range(3 * w, 4 * w))
print('ruo_edge', ruo_edge)
rui_edge = sum(fin.get((x, y), 0) for y in range(-2 * h, -h) for x in range(2 * w, 3 * w))
print('rui_edge', rui_edge)

rdo_edge = sum(fin.get((x, y), 0) for y in range(2 * h, 3 * h) for x in range(3 * w, 4 * w))
print('rdo_edge', rdo_edge)
rdi_edge = sum(fin.get((x, y), 0) for y in range(2 * h, 3 * h) for x in range(2 * w, 3 * w))
print('rdi_edge', rdi_edge)

tile1 = sum(fin.get((x, y), 0) for y in range(h) for x in range(w, 2 * w))
print('tile1', tile1)
tile2 = sum(fin.get((x, y), 0) for y in range(h) for x in range(2 * w, 3 * w))
print('tile2', tile2)

def tot_plots(steps):
    print()
    steps1 = steps - w // 2
    tiles = steps1 // w
    print('tiles', tiles)
    assert steps1 % w == 0
    # for odd num of tiles tile1 and tile2 change places
    assert tiles % 2 == 0
    cross = 2 * (tiles - 2) * (tile1 + tile2)
    print('cross', cross)

    triangle = 0
    for i in range(1, tiles - 1):
        triangle += (i // 2 + i % 2) * tile1 + i // 2 * tile2
    print('triangle', triangle)
    triangles = 4 * triangle
    print('triangles', triangles)

    edges = (tiles - 3) * (ruo_edge + rui_edge + rdo_edge + rdi_edge +
        luo_edge + lui_edge + ldo_edge + ldi_edge)
    total = center + cross + triangles + edges + left + right + bottom + top
    print(steps, 'steps:', total)

tot_plots(N)
tot_plots(26501365)
