import sys

hdr = ''.join(chr(c) for c in range(ord('A'), ord('A') + 26))
total = 0
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
    hsmudge = 0
    half = h // 2
    for m in range(1, h):
        n = 0
        rng = m if m <= half else h - m
        print(f'row {m}: ', end='')
        for j in range(rng):
            n += sum(c1 != c2 for c1, c2 in zip(pat[m + j], pat[m - (j + 1)]))
            if n > 1:
                print(f'rows {m + j} {m - (j + 1)} mismatch: ', end='')
                break
        print('n', n)
        if n == 0:
            assert hmirror == 0
            hmirror = m
            print('hmirror at', hmirror, 100 * hmirror)
        elif n == 1:
            assert hsmudge == 0
            hsmudge = m
            print('hsmudge at', hsmudge, 100 * hsmudge)

    vmirror = 0
    vsmudge = 0
    half = w // 2
    for m in range(1, w):
        n = 0
        rng = m if m <= half else w - m
        print(f'col {hdr[m]}: ', end='')
        for j in range(rng):
            for k, row in enumerate(pat):
                if row[m + j] != row[m - (j + 1)]:
                    n += 1
                    if n > 1:
                        print(f'cols {hdr[m + j]} {hdr[m - (j + 1)]} mismatch at row {k}: ', end='')
                        break
            if n > 1:
                break
        print('n', n)
        if n == 0:
            assert hmirror == 0
            assert vmirror == 0
            vmirror = m
            print('vmirror at', vmirror)
        elif n == 1:
            assert hsmudge == 0
            assert vsmudge == 0
            vsmudge = m
            print('vsmudge at', vsmudge)

    assert sum(x > 0 for x in (hmirror, hsmudge, vmirror, vsmudge)) == 2
    if hsmudge > 0:
        total += 100 * hsmudge
    if vsmudge > 0:
        total += vsmudge
    print()

print(total)
