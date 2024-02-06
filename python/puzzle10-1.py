import sys

lines = []
for line in sys.stdin:
    lines.append(line.rstrip())
w = len(lines[0])
h = len(lines)
print(f'w {w} h {h}')
grid = ''.join(lines)

def pos2xy(pos):
    return (pos % w, pos // w)

def pipe(x, y):
    return grid[y * w + x]

xs, ys = pos2xy(grid.index('S'))
print(xs, ys)

ends = []
for dx, dy in [(-1, 0), (0, -1), (1, 0), (0, 1)]:
    x, y = (xs + dx, ys + dy)
    if x < 0 or x >= w or y < 0 or y >= h:
        continue
    if ((dx < 0 and pipe(x, y) in ('-', 'L', 'F')) or
            (dx > 0 and pipe(x, y) in ('-', 'J', '7')) or
            (dy < 0 and pipe(x, y) in ('|', 'F', '7')) or
            (dy > 0 and pipe(x, y) in ('|', 'J', 'L'))):
        ends.append([(x, y), (xs, ys)])
print(ends)
assert(len(ends) == 2)

steps = 1
while ends[0][0] != ends[1][0]:
    for i in range(2):
        (x1, y1), (x0, y0) = ends[i]
        p1 = pipe(x1, y1)
        n = 0
        for dx, dy in [(-1, 0), (0, -1), (1, 0), (0, 1)]:
            x2, y2 = (x1 + dx, y1 + dy)
            if x2 < 0 or x2 >= w or y2 < 0 or y2 >= h:
                continue
            if (x2, y2) == (x0, y0): # don't go back
                continue
            p2 = pipe(x2, y2)
            if ((dx < 0 and p2 in ('-', 'L', 'F') and p1 in ('-', 'J', '7')) or
                    (dx > 0 and p1 in ('-', 'L', 'F') and p2 in ('-', 'J', '7')) or
                    (dy < 0 and p2 in ('|', '7', 'F') and
                                p1 in ('|', 'L', 'J')) or
                    (dy > 0 and p1 in ('|', '7', 'F') and
                                p2 in ('|', 'L', 'J'))):
                ends[i] = [(x2, y2), (x1, y1)]
                n += 1
                break
        if n == 0:
            print(f"pipe breaks at ({x1}, {y1})")
        assert(n == 1)
    steps += 1
print(steps)
