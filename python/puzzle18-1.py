import sys
from bisect import insort

cmds = []
for line in sys.stdin:
    dir, cnt, color = line.rstrip().split()
    cmds.append((dir, int(cnt), color.strip('()#')))
print('# cmds', len(cmds))

x, y = 0, 0
points = [[x, y]]
for dir, cnt, _ in cmds:
    if dir == 'U':
        for y in range(y - 1, y - cnt - 1, -1):
            points.append([x, y])
    elif dir == 'D':
        for y in range(y + 1, y + cnt + 1):
            points.append([x, y])
    elif dir == 'L':
        for x in range(x - 1, x - cnt - 1, -1):
            points.append([x, y])
    elif dir == 'R':
        for x in range(x + 1, x + cnt + 1):
            points.append([x, y])
    else:
        assert 0
xmin = min(points, key=lambda p: p[0])[0]
xmax = max(points, key=lambda p: p[0])[0]
ymin = min(points, key=lambda p: p[1])[1]
ymax = max(points, key=lambda p: p[1])[1]
print(f'range ({xmin}, {ymin}) - ({xmax}, {ymax})')
h = ymax - ymin + 1
w = xmax - xmin + 1
print('h', h, 'w', w)

grid = [['.'] * w for _ in range(h)]
for p in points:
    p[0] -= xmin
    p[1] -= ymin
    grid[p[1]][p[0]] = '#'
with open('input18-grid', 'w') as f:
    for row in grid:
        print(''.join(row), file=f)

# collect strips of '.'
strips = {}
for y, row in enumerate(grid):
    length = len(row)
    x = 0
    strips[y] = []
    while x < length:
        while x < length and grid[y][x] != '.':
            x += 1
        if x >= length:
            break
        x0 = x
        while x < length and grid[y][x] == '.':
            x += 1
        strips[y].append((x0, x - 1))
print(len(strips), 'strips')

# all top and bottom strips are outer strips
outstrips = {0: strips.pop(0), h - 1: strips.pop(h - 1)}

# all left/right edge-touching strips are outer too
for y, ystrips in strips.items():
    o = []
    if len(ystrips) > 0 and ystrips[0][0] == 0:
        o.append(ystrips.pop(0))
    if len(ystrips) > 0 and ystrips[-1][1] == w - 1:
        o.append(ystrips.pop())
    outstrips[y] = o
# for y, ystrips in outstrips.items():
#     for x0, x1 in ystrips:
#         for x in range(x0, x1 + 1):
#             grid[y][x] = 'O'
# with open('input18-outside', 'w') as f:
#     for row in grid:
#         print(''.join(row), file=f)

changed = True
while changed:
    changed = False

    for y, ystrips in strips.items():
        for i in reversed(range(len(ystrips))):
            x0, x1 = ystrips[i]
            for ox0, ox1 in outstrips[y - 1]:
                if ox1 < x0:
                    continue
                if ox0 > x1:
                    break
                insort(outstrips[y], ystrips.pop(i))
                changed = True
                break

    for y, ystrips in reversed(strips.items()):
        for i in reversed(range(len(ystrips))):
            x0, x1 = ystrips[i]
            for ox0, ox1 in outstrips[y + 1]:
                if ox1 < x0:
                    continue
                if ox0 > x1:
                    break
                insort(outstrips[y], ystrips.pop(i))
                changed = True
                break

for y, ystrips in outstrips.items():
    for x0, x1 in ystrips:
        for x in range(x0, x1 + 1):
            grid[y][x] = 'O'
with open('input18-outside', 'w') as f:
    for row in grid:
        print(''.join(row), file=f)

print(h * w - sum(s[1] - s[0] + 1 for ystrips in outstrips.values() for s in ystrips))
