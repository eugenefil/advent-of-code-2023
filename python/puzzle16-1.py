import sys

grid = []
for line in sys.stdin:
    line = line.rstrip()
    grid.append(line)
h, w = len(grid), len(grid[0])
print('h', h, 'w', w)

reflect = {
    '/': {(1, 0): (0, -1), (-1, 0): (0, 1), (0, 1): (-1, 0), (0, -1): (1, 0)},
    '\\': {(1, 0): (0, 1), (-1, 0): (0, -1), (0, 1): (1, 0), (0, -1): (-1, 0)}
}
meta = [[[0, {}] for x in range(w)] for y in range(h)]
beams = [[-1, 0, 1, 0]]
while beams:
    for i in range(len(beams) - 1, -1, -1):
        beam = beams[i]
        x, y, dx, dy = beam
        x += dx
        y += dy
        if x not in range(0, w) or y not in range(0, h):
            del beams[i]
            continue
        beam[0], beam[1] = x, y
        meta[y][x][0] = 1 # energized

        dx2, dy2 = 0, 0
        c = grid[y][x]
        if c in ('/', '\\'):
            dx, dy = reflect[c][(dx, dy)]
        elif c == '|' and (dx, dy) in ((1, 0), (-1, 0)):
            dx, dy = 0, 1
            dx2, dy2 = 0, -1
        elif c == '-' and (dx, dy) in ((0, 1), (0, -1)):
            dx, dy = 1, 0
            dx2, dy2 = -1, 0

        if (dx, dy) in meta[y][x][1]:
            del beams[i]
        else:
            beam[2], beam[3] = dx, dy
            meta[y][x][1][(dx, dy)] = 1

        if dx2 + dy2 != 0 and (dx2, dy2) not in meta[y][x][1]:
            beams.append([x, y, dx2, dy2])
            meta[y][x][1][(dx2, dy2)] = 1

# for row in meta:
#     print(''.join('#' if x[0] else '.' for x in row))
total = sum(c[0] for r in meta for c in r)
print(total)
