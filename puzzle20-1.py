import sys

mods = {}
ff = []
for line in sys.stdin:
    mod, outs = line.rstrip().split(' -> ')
    outs = outs.split(', ')
    if mod == 'broadcaster':
        t, name = '', mod
    else:
        t, name = mod[0], mod[1:]
        assert t in ('%', '&')
    mods[name] = [t, outs, 0, {}]
    if t == '%':
        ff.append(name)

sink = None
for name, mod in mods.items():
    for out in mod[1]:
        if out not in mods:
            assert sink == None
            sink = out
        else:
            t, _, _, ins = mods[out]
            if t == '&':
                ins[name] = 0
if sink:
    mods[sink] = ['', [], 0, {}]
# print(mods)

cycle = []
n = 0
while True:
    low = 0
    high = 0
    q = [('button', 'broadcaster', 0)]
    while q:
        p = q.pop(0)
        prev, cur, val = p
        if val == 0:
            low += 1
        else:
            high += 1
        # print(p, 'low', low, 'high', high)
        mod = mods[cur]
        t, outs, state, ins = mod
        if t == '%':
            if val == 0:
                state = 1 - state
                mod[2] = state
                for o in outs:
                    q.append((cur, o, state))
        elif t == '&':
            ins[prev] = val
            con = 1
            for v in ins.values():
                con *= v
            con = 1 - con
            for o in outs:
                q.append((cur, o, con))
        else:
            for o in outs:
                q.append((cur, o, val))
        # print(q)
    cycle.append((low, high))
    n += 1
    print(n)
    if n == 1000:
        break
    if sum(mods[f][2] for f in ff) == 0:
        break
# print('cycle', cycle)
print('cycle len', len(cycle))
clow = sum(l[0] for l in cycle)
chigh = sum(l[1] for l in cycle)
print('cycle low', clow, 'high', chigh)

full = 1000 // len(cycle)
low = clow * full
high = chigh * full
for i in range(0, 1000 % len(cycle)):
    low += cycle[i][0]
    high += cycle[i][1]
print(low * high)
