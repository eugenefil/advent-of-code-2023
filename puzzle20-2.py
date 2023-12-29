import sys
import math

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

def sim_to_reset(con, start):
    n = 0
    while True:
        n += 1
        # print(n)
        q = [start]
        while q:
            p = q.pop(0)
            prev, cur, val = p
            # print(p)
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
                res = 1
                for v in ins.values():
                    res *= v
                res = 1 - res
                for o in outs:
                    q.append((cur, o, res))

                if cur == con and res == 0:
                    print(con, 'resets at', n)
                    return n
            else:
                for o in outs:
                    q.append((cur, o, val))
            # print(q)

resets = [
    sim_to_reset('jv', ('broadcaster', 'kt', 0)),
    sim_to_reset('qs', ('broadcaster', 'rg', 0)),
    sim_to_reset('jm', ('broadcaster', 'xv', 0)),
    sim_to_reset('pr', ('broadcaster', 'pd', 0)),
]
fact = []
for r in resets:
    f = []
    while r > 1:
        prime = True
        for d in range(2, int(math.sqrt(r)) + 1):
            if r % d == 0:
                f.append(d)
                r //= d
                prime = False
                break
        if prime:
            f.append(r)
            break
    fact.append(f)
print(fact)

if all(len(f) == 1 for f in fact):
    fact = [f[0] for f in fact]
    print(fact[0] * fact[1] * fact[2] * fact[3])
