import sys
from heapq import heappush, heappop

grid = []
for line in sys.stdin:
    line = line.rstrip()
    grid.append([int(x) for x in line])
h, w = len(grid), len(grid[0])
print('h', h, 'w', w)

def addq(prio, key):
    e = [prio, key]
    d[key] = e
    heappush(q, e)

def modq(prio, e):
    key = e[1]
    e[1] = () # mark unused
    addq(prio, key)

def popq():
    while q:
        prio, key = heappop(q)
        if key != ():
            del d[key]
            return prio, key
    return None

q = []
d = {}
addq(0, (0, 0, 0, 0, 0))
done = {}
while True:
    e = popq()
    assert(e)
    dist, key = e
    done[key] = 1
    x, y, x0, y0, steps = key
    if (x, y) == (w - 1, h - 1):
        break
    
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
        key1 = (x1, y1, x, y, steps1)
        if key1 in done:
            continue

        dist1 = dist + grid[y1][x1]
        e1 = d.get(key1, None)
        if e1 == None:
            addq(dist1, key1)
        elif dist1 < e1[0]:
            modq(dist1, e1)

    # print('done', done)
    # print('q', q)
    # print()
print(dist)
