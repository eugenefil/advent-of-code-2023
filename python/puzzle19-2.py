import sys

VARS = ('x', 'm', 'a', 's')
wfs = {}
for line in sys.stdin:
    line = line.rstrip()
    if line == '':
        break
    name, rules = line.split('{')
    rules = rules.rstrip('}').split(',')
    wf = []
    for rule in rules:
        targ = rule.split(':')
        if len(targ) == 1:
            wf.append(targ)
        else:
            cond, targ = targ
            assert cond[1] in ('<', '>')
            wf.append((targ, cond[1], VARS.index(cond[0]), int(cond[2:])))
    wfs[name] = wf
print(len(wfs), 'workflows')
# print(wfs)

A = []
parts = [('in', 0, ((1, 4000), (1, 4000), (1, 4000), (1, 4000)))]
while parts:
    w, i, dv = parts.pop()
    if w == 'R':
        continue
    elif w == 'A':
        A.append(dv)
        continue
    rule = wfs[w][i]
    if len(rule) > 1:
        targ, op, var, num = rule
        v = dv[var]
        if num < v[0] or (num == v[0] and op == '<'):
            vv = ((), v)
        elif num > v[1] or (num == v[1] and op == '>'):
            vv = (v, ())
        else:
            vv = ((v[0], num - int(op == '<')), (num + int(op == '>'), v[1]))

        if op == '<':
            tt = ((targ, 0), (w, i + 1))
        else:
            tt = ((w, i + 1), (targ, 0))
        for v, t in zip(vv, tt):
            if v == ():
                continue
            parts.append((*t, tuple(v if j == var else d for j, d in enumerate(dv))))
    else:
        parts.append((rule[0], 0, dv))
print(sum(n[0]*n[1]*n[2]*n[3] for n in [[v[1] - v[0] + 1 for v in dv] for dv in A]))
