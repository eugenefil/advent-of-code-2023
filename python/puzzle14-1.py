import sys

plat = []
for line in sys.stdin:
    plat.append(list(line.rstrip()))
h, w = len(plat), len(plat[0])
print('h', h, 'w', w)

for x in range(w):
    for y in range(1, h):
        if plat[y][x] != 'O':
            continue
        for yo in range(y, 0, -1):
            if plat[yo - 1][x] != '.':
                break
            plat[yo - 1][x] = 'O'
            plat[yo][x] = '.'
# for r in plat:
#     print(''.join(r))

load = 0
for y, row in enumerate(plat):
    for c in row:
        if c == 'O':
            load += h - y
print(load)
