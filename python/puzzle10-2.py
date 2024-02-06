import sys
import re

def dump_grid(fname):
    with open(fname, 'w') as f:
        for row in grid:
                print(''.join(row), file=f)


grid = []
for y, row in enumerate(sys.stdin):
    grid.append(list(row.rstrip()))
    if 'S' in row:
        S = row.index('S'), y
w = len(grid[0])
h = len(grid)
print(f'w {w} h {h}')
print(f'S {S}')

DIRECTIONS = [(-1, 0), (0, -1), (1, 0), (0, 1)]

# detect endings of S
S_ends = []
for dx, dy in DIRECTIONS:
    x, y = S[0] + dx, S[1] + dy
    if x < 0 or x >= w or y < 0 or y >= h:
        continue
    c = grid[y][x]
    if ((dx < 0 and c in ('-', 'L', 'F')) or
            (dx > 0 and c in ('-', 'J', '7')) or
            (dy < 0 and c in ('|', 'F', '7')) or
            (dy > 0 and c in ('|', 'J', 'L'))):
        S_ends.append((dx, dy))
print("S ends on", S_ends)
assert(len(S_ends) == 2)

# replace S with its char on the grid
if S_ends == [(-1, 0), (0, -1)]:
    S_c = 'J'
elif S_ends == [(0, -1), (1, 0)]:
    S_c = 'L'
elif S_ends == [(1, 0), (0, 1)]:
    S_c = 'F'
elif S_ends == [(-1, 0), (0, 1)]:
    S_c = '7'
elif S_ends == [(-1, 0), (1, 0)]:
    S_c = '-'
elif S_ends == [(0, -1), (0, 1)]:
    S_c = '|'
else:
    assert(0)
print('S is', S_c)
grid[S[1]][S[0]] = S_c

# find loop vertices
x, y = S_ends[0]
loop = [S, (S[0] + x, S[1] + y)]
while loop[0] != loop[-1]:
    x1, y1 = loop[-1]
    c1 = grid[y1][x1]
    n = 0
    for dx, dy in DIRECTIONS:
        x2, y2 = x1 + dx, y1 + dy
        if x2 < 0 or x2 >= w or y2 < 0 or y2 >= h:
            continue
        if (x2, y2) == loop[-2]: # don't go back
            continue
        c2 = grid[y2][x2]
        if ((dx < 0 and c2 in ('-', 'L', 'F') and c1 in ('-', 'J', '7')) or
                (dx > 0 and c1 in ('-', 'L', 'F') and c2 in ('-', 'J', '7')) or
                (dy < 0 and c2 in ('|', '7', 'F') and
                            c1 in ('|', 'L', 'J')) or
                (dy > 0 and c1 in ('|', '7', 'F') and
                            c2 in ('|', 'L', 'J'))):
            loop.append((x2, y2))
            n += 1
            break
    if n == 0:
        print(f"pipe breaks at ({x1}, {y1})")
    assert(n == 1)
loop.pop()
print("farthest path", len(loop) / 2)

# replace junk pipes with ground
loop = dict(zip(loop, (True,) * len(loop))) # searching list is slow, use dict
for y in range(h):
    for x in range(w):
        if (x, y) not in loop:
            grid[y][x] = '.'
dump_grid('input10-clean')

# insert spaces for vertical corridors between pipes into the grid
VERTPAIRS = [(l, r) for l in ['|', 'J', '7'] for r in ['|', 'L', 'F']]
print('vert pairs', VERTPAIRS)
x = 0
while x < w - 1:
    n = 0
    for y in range(h):
        c1, c2 = grid[y][x], grid[y][x + 1]
        if (c1, c2) in VERTPAIRS: # we have corridor
            n += 1
            break
    if n > 0:
        for y in range(h):
            c1 = grid[y][x]
            c2 = ' '
            if c1 in ('-', 'L', 'F'): # don't break connections
                c2 = '-'
            grid[y].insert(x + 1, c2)
        x += 1
        w += 1
    x += 1

# insert spaces for horizontal corridors between pipes into the grid
HORIZPAIRS = [(t, b) for t in ['-', 'L', 'J'] for b in ['-', '7', 'F']]
print('horiz pairs', HORIZPAIRS)
y = 0
while y < h - 1:
    n = 0
    for x in range(w):
        c1, c2 = grid[y][x], grid[y + 1][x]
        if (c1, c2) in HORIZPAIRS: # we have corridor
            n += 1
            break
    if n > 0:
        row = [' '] * w
        grid.insert(y + 1, row)
        for x in range(w):
            c = grid[y][x]
            if c in ('|', '7', 'F'): # don't break connections
                row[x] = '|'
        y += 1
        h += 1
    y += 1

print(f'w {w} h {h}')
dump_grid('input10-corridors')

# collect horiz strips of ground/space
strips = {}
for y, row in enumerate(grid):
    strips[y] = [m.span() for m in re.finditer(r'[. ]+', ''.join(row))]
print(sum(len(v) for v in strips.values()), 'strips')

# get first batch of outside strips from those adjacent to grid boundaries
outside = {0: strips.pop(0), h - 1: strips.pop(h - 1)}
for y, rowstrips in strips.items():
    outstrips = []
    if len(rowstrips) > 0 and rowstrips[0][0] == 0:
        outstrips.append(rowstrips.pop(0))
    if len(rowstrips) > 0 and rowstrips[-1][1] == w:
        outstrips.append(rowstrips.pop())
    outside[y] = outstrips

changed = True
while changed:
    changed = False

    # scan remaining strips top to bottom and filter those adjacent to outside strips
    y = 1
    while y < h - 1:
        indices = []
        for i, (x1, x2) in enumerate(strips[y]):
            for xo1, xo2 in outside[y - 1]:
                if xo2 - 1 < x1 or xo1 > x2 - 1:
                    continue
                indices.append(i)
                break
        for i in reversed(indices):
            changed = True
            outside[y].append(strips[y].pop(i))
        y += 1

    # scan remaining strips bottom to top and filter those adjacent to outside strips
    y = h - 2
    while y > 0:
        indices = []
        for i, (x1, x2) in enumerate(strips[y]):
            for xo1, xo2 in outside[y + 1]:
                if xo2 - 1 < x1 or xo1 > x2 - 1:
                    continue
                indices.append(i)
                break
        for i in reversed(indices):
            changed = True
            outside[y].append(strips[y].pop(i))
        y -= 1

for y, rowstrips in outside.items():
    for x1, x2 in rowstrips:
        grid[y][x1:x2] = ['O'] * (x2 - x1)
dump_grid('input10-outside')

area = 0
for y, rowstrips in strips.items():
    for x1, x2 in rowstrips:
        area += grid[y][x1:x2].count('.')
print(area)
