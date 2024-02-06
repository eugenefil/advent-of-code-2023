import sys
import math

path = sys.stdin.readline().rstrip()
sys.stdin.readline()
nodes = {}
cur = []
for line in sys.stdin:
    node, lr = line.rstrip().split(' = ')
    l, r = lr.strip('()').split(', ')
    nodes[node] = {'L':l, 'R':r}
    if node.endswith('A'):
        cur.append(node)
    # print(node, l, r)

pathlen = len(path)
totals = []
for i, n in enumerate(cur):
    step = 0
    print(n, end=' ')
    while not n.endswith('Z'):
        n = nodes[n][path[step % pathlen]]
        step += 1
    totals.append(step)
    print(n, step)

total_muls = []
for n in totals:
    print(n, end=': ')
    muls = []
    while n > 1:
        root = int(math.sqrt(n))
        k = 2
        while k <= root:
            if n % k == 0:
                muls.append(k)
                n //= k
                break
            k += 1
        if k > root:
            muls.append(n)
            break
    total_muls.append(muls)
    print(muls)

# lcm(a, b, c) = lcm(lcm(a, b), c)
lcm_muls = total_muls[0]
for muls in total_muls[1:]:
    uniq_muls = []
    comm_muls = []
    for m in muls:
        if m in lcm_muls:
            comm_muls.append(m)
            lcm_muls.remove(m) # there may be many identical factors
        else:
            uniq_muls.append(m)
    lcm_muls.extend(comm_muls)
    lcm_muls.extend(uniq_muls)
print(lcm_muls)

lcm = 1
for mul in lcm_muls:
    lcm *= mul
print(lcm)
