import sys

def HASH(s):
    h = 0
    for c in s:
        h += ord(c)
        h *= 17
        h %= 256
    return h

seq = sys.stdin.read().rstrip().split(',')
total = 0
for op in seq:
    h = HASH(op)
    print(op, h)
    total += h
print(total)
