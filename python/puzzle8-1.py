import sys

path = sys.stdin.readline().rstrip()
sys.stdin.readline()
nodes = {}
for line in sys.stdin:
    node, lr = line.rstrip().split(' = ')
    l, r = lr.strip('()').split(', ')
    nodes[node] = {'L':l, 'R':r}
    # print(node, l, r)

step = 0
cur = 'AAA'
pathlen = len(path)
while cur != 'ZZZ':
    cur = nodes[cur][path[step % pathlen]]
    step += 1
print(step)
