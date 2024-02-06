import sys

mods = {}
for line in sys.stdin:
    mod, outs = line.rstrip().split(' -> ')
    outs = outs.split(', ')
    if mod == 'broadcaster':
        t, name = '', mod
    else:
        t, name = mod[0], mod[1:]
        assert t in ('%', '&')
    mods[name] = [t, outs, 0, {}]

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

n = 0
low = 0
high = 0
while True:
    n += 1
    print(n)
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
    if n == 1000:
        break
print(low * high)
