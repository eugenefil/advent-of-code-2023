import sys

grid = []
for line in sys.stdin:
    line = line.rstrip()
    grid.append([int(x) for x in line])
h, w = len(grid), len(grid[0])
print('h', h, 'w', w)

q = {((0, 0), (0, 0), 0): (0, None)}
done = {}
while q:
    key = min(q, key=lambda x: q[x][0])
    if key[0] == (w - 1, h - 1):
        break
    dist, _ = done[key] = q.pop(key)

    (x, y), (x0, y0), steps = key
    dx, dy = x - x0, y - y0
    for dx1, dy1 in ((-1, 0), (0, -1), (1, 0), (0, 1)):
        x1, y1 = x + dx1, y + dy1
        if x1 not in range(0, w) or y1 not in range(0, h):
            continue
        if (x1, y1) == (x0, y0):
            continue
        if (dx1, dy1) == (dx, dy):
            if steps == 3:
                continue
            steps1 = steps + 1
        else:
            steps1 = 1
        key1 = ((x1, y1), (x, y), steps1)
        if key1 in done:
            continue

        dist1 = dist + grid[y1][x1]
        v1 = q.get(key1, None)
        if v1 == None or dist1 < v1[0]:
            q[key1] = (dist1, (x, y))

    # print('done', done)
    # print('q', q)
    # print()
print(q[key])
