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
            wf.append((targ, cond[1], VARS.index(cond[0]), int(cond[2:])))
    wfs[name] = wf
print(len(wfs), 'workflows')
# print(wfs)

parts = []
for line in sys.stdin:
    parts.append([int(v.split('=')[1]) for v in line.rstrip().strip('{}').split(',')])
print(len(parts), 'parts')
# print(parts)

A = []
for part in parts:
    w = wfs['in']
    i = 0
    while i < len(w):
        rule = w[i]
        if len(rule) > 1:
            targ, op, var, num = rule
            r = False
            if op == '<':
                r = part[var] < num
            elif op == '>':
                r = part[var] > num
            else:
                assert 0
        else:
            targ = rule[0]
            r = True
        if r:
            if targ == 'R':
                break
            elif targ == 'A':
                A.append(part)
                break
            else:
                w = wfs[targ]
                i = 0
        else:
            i += 1
    assert i < len(w)
print(sum(sum(a) for a in A))
