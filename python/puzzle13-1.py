import sys

hdr = ''.join(chr(c) for c in range(ord('A'), ord('A') + 26))
sum = 0
eof = False
while not eof:
    pat = []
    eof = True
    header = False
    for i, line in enumerate(sys.stdin):
        line = line.rstrip()
        if line == '':
            eof = False
            break
        pat.append(line)
        if not header:
            header = True
            print('   ' + hdr[:len(line)])
        print(f'{i:2}', line)

    h = len(pat)
    w = len(pat[0])
    print('h', h, 'w', w)
    assert h > 1 and w > 1

    hmirror = 0
    half = h // 2
    for m in range(1, h):
        hmirror = m
        n = m if m <= half else h - m
        for j in range(n):
            if pat[m + j] != pat[m - (j + 1)]:
                print(f'rows {m + j} and {m - (j + 1)} do not match')
                hmirror = 0
                break
        if hmirror > 0:
            break
    if hmirror > 0:
        print('horiz at', hmirror, 100 * hmirror)
        sum += 100 * hmirror
        continue

    vmirror = 0
    half = w // 2
    for m in range(1, w):
        vmirror = m
        n = m if m <= half else w - m
        for j in range(n):
            for k, row in enumerate(pat):
                if row[m + j] != row[m - (j + 1)]:
                    print(f'cols {hdr[m + j]} and {hdr[m - (j + 1)]} do not match at row {k}')
                    vmirror = 0
                    break
            if vmirror == 0:
                break
        if vmirror > 0:
            break
    if vmirror > 0:
        assert hmirror == 0
        print('vert at', vmirror, vmirror)
        sum += vmirror
    print()

print(sum)
