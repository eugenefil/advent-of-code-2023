import sys
from bisect import insort
from heapq import merge

cmds = []
pdir = ''
for line in sys.stdin:
    color = line.rstrip().split()[-1].strip('()#')
    cnt = int(color[0:5], 16)
    dir = ('R', 'D', 'L', 'U')[int(color[-1])]
    assert dir != pdir
    pdir = dir
    cmds.append((dir, cnt))
print(len(cmds), 'cmds')
# print(cmds)

x0 = y0 = xmin = ymin = xmax = ymax = 0
lines = []
for dir, cnt in cmds:
    if dir == 'D':
        x1, y1 = x0, y0 + cnt
        lines.append([x0, y0, x1, y1])
    elif dir == 'U':
        x1, y1 = x0, y0 - cnt
        lines.append([x1, y1, x0, y0])
    elif dir == 'R':
        x1, y1 = x0 + cnt, y0
        lines.append([x0, y0, x1, y1])
    elif dir == 'L':
        x1, y1 = x0 - cnt, y0
        lines.append([x1, y1, x0, y0])
    else:
        assert 0
    x0, y0 = x1, y1
    if x1 < xmin:
        xmin = x1
    if x1 > xmax:
        xmax = x1
    if y1 < ymin:
        ymin = y1
    if y1 > ymax:
        ymax = y1
print(f'range ({xmin}, {ymin}) - ({xmax}, {ymax})')
h = ymax - ymin + 1
w = xmax - xmin + 1
print('h', h, 'w', w)

for l in lines:
    l[0] -= xmin
    l[1] -= ymin
    l[2] -= xmin
    l[3] -= ymin
print(len(lines), 'lines')

def get_x(line):
    return line[0]

by = list(set(l[1] for l in lines)) + [h - 1] # last range is (h - 1), (h - 1)
by.sort()
vlines = []
hlines = []
for by0, by1 in zip(by[:-1], by[1:]):
    vl = []
    hl = []
    for l in lines:
        x0, y0, x1, y1 = l
        if x0 != x1:
            if y0 == by0:
                hl.append(l)
            continue
        if not (y1 <= by0 or y0 >= by1):
            vl.append(l)
    vl.sort(key=get_x)
    vlines.append(vl)
    hl.sort(key=get_x)
    hlines.append(hl)
# for i, (vl, hl) in enumerate(zip(vlines, hlines)):
#     print(f'{i}: y {by[i]}-{by[i + 1]} vlines {vl} hlines {hl}')

rects = []
for i, (vl, hl) in enumerate(zip(vlines, hlines)):
    r = []
    x = 0
    y0, y1 = by[i], by[i + 1]
    lines = merge(vl, hl, key=get_x)
    for x0, _, x1, _ in lines:
        if x < x0:
            r.append((x, y0, x0 - 1, y0))
        if x1 >= x: # this is for horiz and vert lines with same x0
            x = x1 + 1
    if x < w:
        r.append((x, y0, w - 1, y0))
    rects.append(r)

    if y1 - y0 > 1:
        r = []
        x = 0
        for x0, _, x1, _ in vl:
            if x < x0:
                r.append((x, y0 + 1, x0 - 1, y1 - 1))
            x = x1 + 1
        if x < w:
            r.append((x, y0 + 1, w - 1, y1 - 1))
        rects.append(r)
# for i, r in enumerate(rects):
#     print(f'{i}: rects {r}')

outrects = [[] for _ in range(len(rects))]
outrects[0] = rects[0] # all top and bottom rects are outer rects
outrects[-1] = rects[-1]
rects[0] = rects[-1] = []
for i, irects in enumerate(rects):
    o = outrects[i]
    if len(irects) > 0 and irects[0][0] == 0:
        o.append(irects.pop(0))
    if len(irects) > 0 and irects[-1][2] == w - 1:
        o.append(irects.pop())
# for i, r in enumerate(outrects):
#     print(f'{i}: outrects {r}')

changed = True
while changed:
    changed = False

    for i in range(1, len(rects) - 1):
        r = rects[i]
        for j in reversed(range(len(r))):
            x0, _, x1, _ = r[j]
            for ox0, _, ox1, _ in outrects[i - 1]:
                if ox1 < x0:
                    continue
                if ox0 > x1:
                    break
                insort(outrects[i], r.pop(j))
                changed = True
                break

    for i in range(len(rects) - 2, 0, -1):
        r = rects[i]
        for j in reversed(range(len(r))):
            x0, _, x1, _ = r[j]
            for ox0, _, ox1, _ in outrects[i + 1]:
                if ox1 < x0:
                    continue
                if ox0 > x1:
                    break
                insort(outrects[i], r.pop(j))
                changed = True
                break
# for i, r in enumerate(outrects):
#     print(f'{i}: outrects {r}')

outarea = sum(
    (x1 - x0 + 1) * (y1 - y0 + 1)
    for r in outrects
    for x0, y0, x1, y1 in r)
print(h * w - outarea)
