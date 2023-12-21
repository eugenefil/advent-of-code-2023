import sys

plat = []
for line in sys.stdin:
    line = line.rstrip()
    plat.append(list(line))
    print(line)
h, w = len(plat), len(plat[0])
print('h', h, 'w', w)

def cycle():
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
    # print()

    for y in range(h):
        for x in range(1, w):
            if plat[y][x] != 'O':
                continue
            for xo in range(x, 0, -1):
                if plat[y][xo - 1] != '.':
                    break
                plat[y][xo - 1] = 'O'
                plat[y][xo] = '.'
    # for r in plat:
    #     print(''.join(r))
    # print()

    for x in range(w):
        for y in range(h - 2, -1, -1):
            if plat[y][x] != 'O':
                continue
            for yo in range(y, h - 1):
                if plat[yo + 1][x] != '.':
                    break
                plat[yo + 1][x] = 'O'
                plat[yo][x] = '.'
    # for r in plat:
    #     print(''.join(r))
    # print()

    for y in range(h):
        for x in range(w - 2, -1, -1):
            if plat[y][x] != 'O':
                continue
            for xo in range(x, w - 1):
                if plat[y][xo + 1] != '.':
                    break
                plat[y][xo + 1] = 'O'
                plat[y][xo] = '.'
    # for r in plat:
    #     print(''.join(r))
    # print()

d = {}
start = 0
start_cycle = 0
stop = 0
for i in range(int(sys.argv[1])):
    cycle()
    key = ''.join(''.join(r) for r in plat)
    if key in d:
        j, load = d[key]
        if start == 0:
            start = i
            start_cycle = j
        elif j == start_cycle:
            stop = i
            break
        print(f'{i}:', 'found at cycle', j, 'load', load)
    else:
        load = 0
        for y, row in enumerate(plat):
            for c in row:
                if c == 'O':
                    load += h - y
        print(f'{i}:', load)
        d[key] = (i, load)

last_cycle = (1000000000 - start) % (stop - start) + start_cycle - 1
print('last cycle', last_cycle)
for c, load in d.values():
    if c == last_cycle:
        print('load', load)
        break
